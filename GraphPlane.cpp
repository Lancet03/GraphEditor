#include "GraphPlane.h"
#include "EdgeLine.h"
#include "VertexCircle.h"
#include <QDebug>
#include <QDialog>
#include <QGraphicsEllipseItem>
#include <QHeaderView>
#include <QPainter>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWheelEvent>

#include <cmath>
#include <typeinfo>

GraphPlane::GraphPlane(QWidget *parent)
    : QGraphicsView(parent), currentScale(1.0) {
    this->scene = new QGraphicsScene(this);
    this->scene->setSceneRect(-100000, -100000, 200000, 200000);
    setScene(this->scene);

    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::NoDrag);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->graph = new Graph();
}

Graph *GraphPlane::SetGraph(Graph *g) {
    this->ClearGraph();

    this->graph = g;
    for (int i = 0; i < g->vertexes.size(); i++) {
        this->AddVertex(g->vertexes[i]);
    }
    for (int i = 0; i < g->edges.size(); i++) {
        this->AddEdge(g->edges[i]);
    }

    return g;
}

void GraphPlane::ClearGraph() {
    for (auto edge : this->edgeLines) {
        scene->removeItem(edge);
        delete edge;
    }
    this->edgeLines.clear();

    for (auto vertex : this->vertexCircles) {
        this->scene->removeItem(vertex);
        delete vertex;
    }
    this->vertexCircles.clear();

    this->graph = new Graph();

    emit graphChanged(this->graph);
}

VertexCircle *GraphPlane::GetVertexById(int id) {
    for (int i = 0; i < this->vertexCircles.size(); i++) {
        int vertexId = this->vertexCircles[i]->GetId();
        if (id == vertexId) {
            return this->vertexCircles[i];
        }
    }
    return nullptr;
}

void GraphPlane::SetMode(Mode m) {
    this->mode = m;
    this->selectedCircle = nullptr;
    this->firstSelectedCircle = nullptr;
    this->secondSelectedCircle = nullptr;
}

void GraphPlane::mousePressEvent(QMouseEvent *event) {
    // Преобразуем координаты клика в координаты сцены
    QPointF scenePos = mapToScene(event->pos());

    if (event->button() == Qt::LeftButton) {
        QGraphicsItem *item = scene->itemAt(scenePos, QTransform());

        if (item && item->parentItem() &&
            item->parentItem()->data(0).toString() == "VertexCircle") {
            item = item->parentItem();
        }

        if (item && item->data(0).toString() == "VertexCircle") {
            VertexCircle *selectedVertex = dynamic_cast<VertexCircle *>(item);
            if (this->mode == Mode::MOVE) {
                selectedVertex->setMovable(true);
                this->selectedCircle = selectedVertex;
            } else if (this->mode == Mode::EDIT) {
                // Включаем режим изменения радиуса
                qDebug() << "Edit";
                this->selectedCircle = selectedVertex;
            } else if (this->mode == Mode::ADD_EDGES) {
                qDebug() << "Add edges ";
                if (!this->firstSelectedCircle) {
                    firstSelectedCircle = selectedVertex;
                    firstSelectedCircle->setPen(QPen(Qt::green, 3));
                    this->AddTempLine(selectedVertex);
                } else if (!secondSelectedCircle) {
                    if (firstSelectedCircle->vertex->id == selectedVertex->vertex->id) {
                        firstSelectedCircle = nullptr;
                        secondSelectedCircle = nullptr;
                        this->RemoveTempLine();
                        QGraphicsView::mousePressEvent(event);
                        return;
                    }
                    secondSelectedCircle = selectedVertex;

                    // Соединяем два выбранных кружка
                    this->AddEdge(firstSelectedCircle, secondSelectedCircle);

                    firstSelectedCircle = nullptr;
                    secondSelectedCircle = nullptr;
                } else {
                    firstSelectedCircle = nullptr;
                    secondSelectedCircle = nullptr;
                    this->RemoveTempLine();
                }
            }
        } else if (this->mode == Mode::ADD_VERTEXES) {
            this->AddVertex(scenePos);
        } else if (this->mode == Mode::ADD_EDGES) {
            this->RemoveTempLine();
            this->firstSelectedCircle = nullptr;
        }
    } else if (event->button() == Qt::RightButton) {
        // Проверяем, есть ли объект под курсором
        QGraphicsItem *item = scene->itemAt(scenePos, QTransform());
        if (item && item->parentItem() &&
            item->parentItem()->data(0).toString() == "VertexCircle") {
            item = item->parentItem();
        }
        if (item->data(0).toString() == "VertexCircle") {
            VertexCircle *vertex = dynamic_cast<VertexCircle *>(item);
            this->RemoveVertex(vertex);
        }

        if (item && item->parentItem() &&
            item->parentItem()->data(0).toString() == "EdgeLine") {
            item = item->parentItem();
        }
        if (item->data(0).toString() == "EdgeLine") {
            EdgeLine *edge = dynamic_cast<EdgeLine *>(item);
            this->RemoveEdge(edge);
        }
    } else if (event->button() == Qt::MiddleButton) {
        this->isDragging = true;
        // по нажатию средней кнопки мыши создаем событие ее отпускания выставляем
        // моду перетаскивания и создаем событие зажатой левой кнопки мыши
        QMouseEvent releaseEvent(
            QEvent::MouseButtonPress,
            event->position(),
            event->globalPosition(),
            event->scenePosition(),
            Qt::LeftButton,
            event->buttons() | Qt::LeftButton,
            event->modifiers(),
            event->pointingDevice()
            );

        QGraphicsView::mouseReleaseEvent(&releaseEvent);

        setDragMode(QGraphicsView::ScrollHandDrag);

        QMouseEvent fakeEvent(event->type(), event->localPos(), event->screenPos(),
                              event->windowPos(), Qt::LeftButton,
                              event->buttons() | Qt::LeftButton,
                              event->modifiers());

        QGraphicsView::mousePressEvent(&fakeEvent);
    }

    QGraphicsView::mousePressEvent(event);
}

VertexCircle *GraphPlane::AddVertex(QPointF vertex_center) {
    VertexCircle *circle = new VertexCircle(
        vertex_center.x() - 10, vertex_center.y() - 10, 20, this->graph);
    this->graph->addVertex(circle->vertex);

    return this->RegisterVertex(circle);
}

VertexCircle *GraphPlane::AddVertex(std::shared_ptr<GraphVertex> vertex) {
    VertexCircle *circle = new VertexCircle(vertex.get(), this->graph);

    return this->RegisterVertex(circle);
}

VertexCircle *GraphPlane::RegisterVertex(VertexCircle *circle) {
    scene->addItem(circle);
    this->vertexCircles.push_back(circle);
    connect(circle, &VertexCircle::stateChanged, this,
            [this]() { emit graphChanged(this->graph); });
    emit graphChanged(this->graph);

    return circle;
}

void GraphPlane::RemoveVertex(VertexCircle *vertex) {
    for (int i = 0; i < this->vertexCircles.count(); i++) {
        if (this->vertexCircles[i] == vertex) {
            this->vertexCircles.removeAt(i);
        }
    }

    while (vertex->getEdges().size()) {
        EdgeLine *edge = vertex->getEdges()[0];
        this->RemoveEdge(edge);
    }
    this->scene->removeItem(vertex);
    this->graph->removeVertex(vertex->vertex);

    emit graphChanged(this->graph);
}

EdgeLine *GraphPlane::AddEdge(VertexCircle *start, VertexCircle *end) {
    if (this->tempLine) {
        this->scene->removeItem(this->tempLine);
        this->tempLine = nullptr;
    }
    std::shared_ptr<GraphEdge> graphEdge =
        this->graph->addEdge(start->vertex->id, end->vertex->id);

    EdgeLine *edge = new EdgeLine(graphEdge.get(), start, end, this->graph);

    return this->RegisterEdge(edge);
}

EdgeLine *GraphPlane::AddEdge(std::shared_ptr<GraphEdge> edge) {
    if (this->tempLine) {
        this->scene->removeItem(this->tempLine);
        this->tempLine = nullptr;
    }
    GraphVertex *from = edge->from.get();
    GraphVertex *to = edge->to.get();

    VertexCircle *start = this->GetVertexById(from->id);
    VertexCircle *end = this->GetVertexById(to->id);

    EdgeLine *edgeLine = new EdgeLine(edge.get(), start, end, this->graph);

    return this->RegisterEdge(edgeLine);
}

EdgeLine* GraphPlane::AddTempLine(VertexCircle * start) {
    this->tempLine = new EdgeLine(start);
    this->scene->addItem(tempLine);
    return this->tempLine;
}

void GraphPlane::RemoveTempLine() {
    this->scene->removeItem(this->tempLine);
    this->tempLine = nullptr;
}

EdgeLine *GraphPlane::RegisterEdge(EdgeLine *edgeLine) {
    this->edgeLines.push_back(edgeLine);
    this->scene->addItem(edgeLine);
    connect(edgeLine, &EdgeLine::stateChanged, this,
            [this]() { emit graphChanged(this->graph); });
    emit graphChanged(this->graph);
    return edgeLine;
}

void GraphPlane::RemoveEdge(EdgeLine *edge) {
    if (edge->startVertex) {
        edge->startVertex->removeEdge(edge);
    }
    if (edge->endVertex) {
        edge->endVertex->removeEdge(edge);
    }
    this->scene->removeItem(edge);
    if (edge->edge) {
        this->graph->removeEdge(edge->edge);
    }
    emit graphChanged(this->graph);
}

void GraphPlane::drawBackground(QPainter *painter, const QRectF &rect) {
    painter->save();
    qreal scaleX = transform().m11();
    int baseGridSize = 20;

    // Calc grid step
    int gridSize = baseGridSize;
    while (gridSize * scaleX < 10) {
        gridSize *= 2;
    }
    while (gridSize * scaleX > 100) {
        gridSize /= 2;
    }

    // Calc the boundaries of the visible area
    double left = std::floor(rect.left() / gridSize) * gridSize;
    double right = std::ceil(rect.right() / gridSize) * gridSize;
    double top = std::floor(rect.top() / gridSize) * gridSize;
    double bottom = std::ceil(rect.bottom() / gridSize) * gridSize;

    QPen gridPen(Qt::gray, 0);
    painter->setPen(gridPen);
    for (double x = left; x <= right; x += gridSize) {
        painter->drawLine(QLineF(x, top, x, bottom));
    }
    for (double y = top; y <= bottom; y += gridSize) {
        painter->drawLine(QLineF(left, y, right, y));
    }

    // Draw axis X and Y
    QPen axisPen(Qt::black, 2);
    painter->setPen(axisPen);
    painter->drawLine(QLineF(left, 0, right, 0));
    painter->drawLine(QLineF(0, top, 0, bottom));

    painter->restore();
}

void GraphPlane::wheelEvent(QWheelEvent *event) {
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
        currentScale *= scaleFactor;
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        currentScale /= scaleFactor;
    }

    emit zoomLevelChanged(currentScale);

    // update scene
    viewport()->update();
}

void GraphPlane::mouseMoveEvent(QMouseEvent *event) {
    QPointF scenePos = mapToScene(event->pos());
    emit mousePositionChanged(scenePos);

    if (this->selectedCircle) {
        if (this->mode == Mode::EDIT) {
            QPointF scenePos = mapToScene(event->pos());

            QPointF center = this->selectedCircle->sceneBoundingRect().center();
            qreal newRadius =
                std::hypot(scenePos.x() - center.x(), scenePos.y() - center.y());

            newRadius = std::max(newRadius, 5.0);

            this->selectedCircle->setRadius(newRadius);
            for (EdgeLine *edge : this->selectedCircle->getEdges()) {
                edge->updatePosition();
            }
        } else if (this->mode == Mode::MOVE) {
            QPointF p = mapToScene(event->pos());
            this->selectedCircle->moveTo(p);
        }
    }
    if (this->mode == Mode::ADD_EDGES && this->tempLine) {
        this->tempLine->SetEndPos(mapToScene(event->pos()));
    }

    QGraphicsView::mouseMoveEvent(event);
}

qreal GraphPlane::getCurrentScale() const { return this->currentScale; }

void GraphPlane::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        // отпускаем левую кнопку мыши которую виртуально зажали в mousePressEvent
        QMouseEvent fakeEvent(event->type(), event->localPos(), event->screenPos(),
                              event->windowPos(), Qt::LeftButton,
                              event->buttons() & ~Qt::LeftButton,
                              event->modifiers());

        QGraphicsView::mouseReleaseEvent(&fakeEvent);

        setDragMode(QGraphicsView::NoDrag);
    } else if (event->button() == Qt::LeftButton && this->selectedCircle) {
        if (this->mode == Mode::MOVE) {
            QPointF p = mapToScene(event->pos());
            this->selectedCircle->vertex->MoveTo(p.x(), p.y());
            this->selectedCircle->setMovable(false);
        }

        this->selectedCircle = nullptr;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void GraphPlane::enterEvent(QEnterEvent *event) {
    QGraphicsView::enterEvent(event);
    emit mouseEntered();
}

void GraphPlane::leaveEvent(QEvent *event) {
    QGraphicsView::leaveEvent(event);
    emit mouseLeft();
}
