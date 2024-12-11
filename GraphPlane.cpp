#include "GraphPlane.h"
#include "VertexCircle.h"
#include "EdgeLine.h"
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QWheelEvent>
#include <QDebug>

#include <cmath>
#include <typeinfo>


GraphPlane::GraphPlane(QWidget *parent) : QGraphicsView(parent), currentScale(1.0)
{
    // Создаем сцену (можно задать очень большой прямоугольник, но это не обязательно)
    this->scene = new QGraphicsScene(this);
    this->scene->setSceneRect(-100000, -100000, 200000, 200000); // Огромная сцена
    setScene(this->scene);

    // Настройки для прокрутки и масштабирования
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::NoDrag);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



    this->graph = new Graph();
}

Graph* GraphPlane::SetGraph(Graph* g) {
    this->graph = g;
    return g;
}

void GraphPlane::mousePressEvent(QMouseEvent *event)
{
    // Преобразуем координаты клика в координаты сцены
    QPointF scenePos = mapToScene(event->pos());

    if (event->button() == Qt::LeftButton) {
        QGraphicsItem *item = scene->itemAt(scenePos, QTransform());

        if (item && item->data(0).toString() == "VertexCircle") {
            VertexCircle* selectedVertex = dynamic_cast<VertexCircle*>(item);
            if (this->mode == Mode::MOVE) {
                selectedVertex->setMovable(true);
                this->selectedCircle = selectedVertex;
            } else if (this->mode == Mode::EDIT) {
                // Включаем режим изменения радиуса
                qDebug() << "Edit";
                this->selectedCircle = selectedVertex;
            } else if (this->mode == Mode::ADD_EDGES) {
                if (!this->firstSelectedCircle) {
                    firstSelectedCircle = selectedVertex;
                    firstSelectedCircle->setPen(QPen(Qt::green, 3));
                } else if (!secondSelectedCircle) {
                    secondSelectedCircle = selectedVertex;
                    secondSelectedCircle->setPen(QPen(Qt::green, 3)); // Указываем, что кружок выбран

                    // Соединяем два выбранных кружка
                    connectCircles(firstSelectedCircle, secondSelectedCircle);

                    // Сбрасываем выбор
                    firstSelectedCircle->setPen(QPen(Qt::blue, 2));
                    secondSelectedCircle->setPen(QPen(Qt::blue, 2));
                    firstSelectedCircle = nullptr;
                    secondSelectedCircle = nullptr;
                }
            }
        } else if (this->mode == Mode::ADD_VERTEXES) {
            qDebug() << "Add vertex";
            VertexCircle *circle = new VertexCircle(scenePos.x() - 10, scenePos.y() - 10, 20);
            circle->setData(0, "VertexCircle");
            scene->addItem(circle); // Добавляем в сцену
            this->graph->addVertex(circle->vertex);
        }
    } else if (event->button() == Qt::RightButton) {
        // Проверяем, есть ли объект под курсором
        QGraphicsItem *item = scene->itemAt(scenePos, QTransform());
        if (item->data(0).toString() == "VertexCircle") {
            scene->removeItem(item);
            delete item;
        }
    } else if (event->button() == Qt::MiddleButton) {
        this->isDragging = true;
        // по нажатию средней кнопки мыши создаем событие ее отпускания выставляем моду перетаскивания и создаем событие зажатой левой кнопки мыши
        QMouseEvent releaseEvent(QEvent::MouseButtonRelease,
                                 event->localPos(),
                                 event->screenPos(),
                                 event->windowPos(),
                                 Qt::LeftButton, nullptr,
                                 event->modifiers());

        QGraphicsView::mouseReleaseEvent(&releaseEvent);

        setDragMode(QGraphicsView::ScrollHandDrag);

        QMouseEvent fakeEvent(event->type(), event->localPos(), event->screenPos(),event->windowPos(),Qt::LeftButton, event->buttons() | Qt::LeftButton, event->modifiers());

        QGraphicsView::mousePressEvent(&fakeEvent);
    }

    // Передаем событие базовому классу
    QGraphicsView::mousePressEvent(event);
}

void GraphPlane::connectCircles(VertexCircle *start, VertexCircle *end)
{
    // if (firstSelectedCircle && secondSelectedCircle) {
    //     // Получаем центры кружков
    //     QPointF firstCenter = firstSelectedCircle->sceneBoundingRect().center();
    //     QPointF secondCenter = secondSelectedCircle->sceneBoundingRect().center();

    //     // Создаем линию между центрами кружков
    //     QGraphicsLineItem *line = scene->addLine(QLineF(firstCenter, secondCenter), QPen(Qt::black, 2));

    //     // Линия добавлена в сцену
    // }
    // Создаем объект линии
    EdgeLine *edge = new EdgeLine(start, end);

    // Добавляем линию на сцену
    scene->addItem(edge);
}

void GraphPlane::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();

    // Получаем текущий масштаб
    qreal scale = transform().m11(); // m11 — это масштаб по оси X

    // Размер базовой клетки сетки
    int baseGridSize = 20;

    // Рассчитываем шаг сетки с учетом текущего масштаба
    int gridSize = baseGridSize;
    while (gridSize * scale < 10) {
        gridSize *= 2; // Увеличиваем шаг, чтобы мелкая сетка исчезала
    }
    while (gridSize * scale > 100) {
        gridSize /= 2; // Уменьшаем шаг, чтобы сетка не была слишком редкой
    }

    // Вычисляем границы видимой области
    double left = std::floor(rect.left() / gridSize) * gridSize;
    double right = std::ceil(rect.right() / gridSize) * gridSize;
    double top = std::floor(rect.top() / gridSize) * gridSize;
    double bottom = std::ceil(rect.bottom() / gridSize) * gridSize;

    // Рисуем линии сетки
    QPen gridPen(Qt::gray, 0); // Линии сетки
    painter->setPen(gridPen);

    // Рисуем вертикальные линии
    for (double x = left; x <= right; x += gridSize) {
        painter->drawLine(QLineF(x, top, x, bottom));
    }

    // Рисуем горизонтальные линии
    for (double y = top; y <= bottom; y += gridSize) {
        painter->drawLine(QLineF(left, y, right, y));
    }

    // Рисуем оси X и Y
    QPen axisPen(Qt::black, 2);
    painter->setPen(axisPen);
    painter->drawLine(QLineF(left, 0, right, 0)); // Ось X
    painter->drawLine(QLineF(0, top, 0, bottom)); // Ось Y

    painter->restore();
}

void GraphPlane::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
        currentScale *= scaleFactor;
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        currentScale /= scaleFactor;
    }

    // Излучаем сигнал об изменении масштаба
    emit zoomLevelChanged(currentScale);

    // Перерисовываем сцену, чтобы обновить сетку
    viewport()->update();
}

void GraphPlane::mouseMoveEvent(QMouseEvent *event)
{
    // Получаем позицию курсора в координатах сцены
    QPointF scenePos = mapToScene(event->pos());

    // Излучаем сигнал с обновленными координатами
    emit mousePositionChanged(scenePos);

    if (this->selectedCircle) {
        if (this->mode == Mode::EDIT) {
            QPointF scenePos = mapToScene(event->pos());

            // Вычисляем новый радиус как расстояние между центром и текущей позицией мыши
            QPointF center = this->selectedCircle->sceneBoundingRect().center();
            qreal newRadius = std::hypot(scenePos.x() - center.x(), scenePos.y() - center.y());

            // Устанавливаем минимальный радиус, чтобы вершина не исчезала
            newRadius = std::max(newRadius, 5.0);

            // Обновляем радиус вершины
            this->selectedCircle->setRadius(newRadius);
        }
        else if (this->mode == Mode::MOVE) {
            QPoint p = event->pos();
            // this->selectedCircle->vertex->MoveTo(p.x(), p.y());
            this->selectedCircle->moveTo(p);
        }

    }


    QGraphicsView::mouseMoveEvent(event);
}

qreal GraphPlane::getCurrentScale() const
{
    return this->currentScale;
}

void GraphPlane::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        //отпускаем левую кнопку мыши которую виртуально зажали в mousePressEvent
        QMouseEvent fakeEvent(event->type(), event->localPos(), event->screenPos(),
                              event->windowPos(),
                              Qt::LeftButton,
                              event->buttons() & ~Qt::LeftButton,
                              event->modifiers());

        QGraphicsView::mouseReleaseEvent(&fakeEvent);

        setDragMode(QGraphicsView::NoDrag);
    } else if (event->button() == Qt::LeftButton && this->selectedCircle) {
        if (this->mode == Mode::MOVE) {
            QPoint p = event->pos();
            this->selectedCircle->vertex->MoveTo(p.x(), p.y());
            this->selectedCircle->setMovable(false);
        }

        this->selectedCircle = nullptr;
    }

    QGraphicsView::mouseReleaseEvent(event); // Передаём событие базовому классу
}

void GraphPlane::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
    emit mouseEntered(); // Излучаем сигнал о входе
}

void GraphPlane::leaveEvent(QEvent *event)
{
    QGraphicsView::leaveEvent(event);
    emit mouseLeft(); // Излучаем сигнал о выходе
}
