#include "VertexCircle.h"
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>
#include <QStyleOptionGraphicsItem>

VertexCircle::VertexCircle(qreal x, qreal y, qreal radius, Graph *graph,
                           QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), m_radius(radius) {
    this->vertex = new GraphVertex(x, y, graph->calcUniqueVertexId());
    this->vertex->radius = radius;
    this->m_radius = radius;
    this->graph = graph;

    this->SetStartParameters();

    this->name = new QGraphicsTextItem(this);

    this->name->setParentItem(this);
    this->name->setAcceptedMouseButtons(Qt::NoButton);
    this->name->setPlainText("");
    this->updateNamePosition();
}

VertexCircle::VertexCircle(GraphVertex *vertex, Graph *graph,
                           QGraphicsItem *parent) {
    this->vertex = vertex;
    this->graph = graph;
    this->m_radius = this->vertex->radius;

    this->SetStartParameters();

    this->name = new QGraphicsTextItem(this);
    this->name->setAcceptedMouseButtons(Qt::NoButton);
    name->setPlainText(QString::fromStdString(vertex->GetName()));
    this->updateNamePosition();
}

void VertexCircle::SetStartParameters() {
    setRect(this->vertex->xPos - this->m_radius,
            this->vertex->yPos - this->m_radius, 2 * this->m_radius,
            2 * this->m_radius);
    setPen(QPen(Qt::blue, 2));
    setBrush(QBrush(Qt::cyan));
    setFlags(QGraphicsItem::ItemIsSelectable);
    this->setData(0, "VertexCircle");
}

qreal VertexCircle::getRadius() const { return m_radius; }

void VertexCircle::setName(const QString &name) {
    this->name->setPlainText(name);
    this->vertex->SetName(name.toStdString());
    updateNamePosition();

    emit stateChanged();
}

QString VertexCircle::getName() const { return name->toPlainText(); }

void VertexCircle::updateNamePosition() {
    QPointF center = boundingRect().center();
    this->name->setPos(center.x() - this->name->boundingRect().width() / 2,
                     center.y() - this->name->boundingRect().height() / 2);
}

void VertexCircle::setRadius(qreal radius) {
    qDebug() << "Vertex radius changed " << radius;
    this->vertex->SetRadius(radius);

    this->m_radius = radius;
    setRect(rect().center().x() - radius, rect().center().y() - radius,
            2 * radius, 2 * radius);
}

void VertexCircle::addEdge(EdgeLine *edge) {
    if (!edges.contains(edge)) {
        edges.append(edge);
    }
}

void VertexCircle::removeEdge(EdgeLine *edge) { edges.removeAll(edge); }

const QList<EdgeLine *> &VertexCircle::getEdges() const { return edges; }

int VertexCircle::GetId() { return this->vertex->id; }

void VertexCircle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    setPen(QPen(Qt::red, 3));
    setBrush(QBrush(Qt::yellow));
    QGraphicsEllipseItem::mousePressEvent(
        event);
}

void VertexCircle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    setPen(QPen(Qt::blue, 2));
    setBrush(QBrush(Qt::cyan));
    QGraphicsEllipseItem::mouseReleaseEvent(
        event);
}

void VertexCircle::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    this->editName();
    QGraphicsEllipseItem::mouseDoubleClickEvent(event);
}

void VertexCircle::editName() {
    if (!scene()) {
        qWarning() << "The item is not part of a scene.";
        return;
    }

    QLineEdit *lineEdit = new QLineEdit(this->name->toPlainText());
    lineEdit->setAlignment(Qt::AlignCenter);

    // Add text field in scene through ProxyWidget
    QGraphicsProxyWidget *proxy = scene()->addWidget(lineEdit);
    proxy->setPos(this->name->scenePos());

    lineEdit->setFocus();

    connect(lineEdit, &QLineEdit::editingFinished, [this, lineEdit, proxy]() {
        setName(lineEdit->text());
        proxy->deleteLater(); // correctly delete ProxyWidget
    });
}

void VertexCircle::setMovable(bool movable) {
    if (movable) {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    } else {
        setFlags(QGraphicsItem::ItemIsSelectable);
    }
}

void VertexCircle::moveTo(QPointF point) {

    this->vertex->MoveTo(point.x(), point.y());
    for (int i = 0; i < this->edges.count(); i++) {
        this->edges[i]->updatePosition();
    }
}

void VertexCircle::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget) {
    bool isSelected = option->state & QStyle::State_Selected;

    if (isSelected) {
        painter->setPen(
            QPen(Qt::red, 3, Qt::DashLine));
        painter->setBrush(QBrush(Qt::yellow));
    } else {
        painter->setPen(QPen(Qt::blue, 2));
        painter->setBrush(QBrush(Qt::cyan));
    }

    painter->drawEllipse(rect());
}
