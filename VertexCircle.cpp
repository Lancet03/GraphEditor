#include "VertexCircle.h"
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QGraphicsScene>

VertexCircle::VertexCircle(qreal x, qreal y, qreal radius, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), m_radius(radius)
{
    this->vertex = new GraphVertex(x, y);
    this->vertex->radius = radius;

    // Устанавливаем начальные параметры
    setRect(x - radius, y - radius, 2 * radius, 2 * radius);
    // Устанавливаем базовый стиль
    setPen(QPen(Qt::blue, 2));
    setBrush(QBrush(Qt::cyan));
    setFlags(QGraphicsItem::ItemIsSelectable);
    this->setData(0, "VertexCircle");
}

qreal VertexCircle::getRadius() const
{
    return m_radius;
}

void VertexCircle::removeSelf() {
    // // Delete detached lines
    // for (EdgeLine* edge : this->edges) {
    //     edge->removeSelf();
    // }
    while (this->edges.size()) {
        this->edges[0]->removeSelf();
    }

    // Удаляем саму линию из сцены
    if (scene()) {
        scene()->removeItem(this);
    }
    delete this;
}


void VertexCircle::setRadius(qreal radius)
{
    qDebug() << "Vertex radius changed " << radius;
    this->vertex->radius = radius;

    m_radius = radius;
    // Обновляем размер круга
    setRect(rect().center().x() - radius, rect().center().y() - radius, 2 * radius, 2 * radius);
}

// QVariant VertexCircle::itemChange(GraphicsItemChange change, const QVariant &value)
// {
//     qDebug() << "Vertex  > value changed >" << change;
//     // Проверяем, изменяется ли позиция объекта
//     if (change == QGraphicsItem::ItemPositionChange) {
//         QPointF point = value.toPointF();
//         this->vertex->xPos = point.x();
//         this->vertex->yPos = point.y();
//         qDebug() << "Vertex > position changed > " << this->vertex->xPos << " " << this->vertex->yPos;

//         emit positionChanged(); // Излучаем сигнал с новой позицией
//     }

//     return QGraphicsEllipseItem::itemChange(change, value);
// }

void VertexCircle::addEdge(EdgeLine* edge) {
    if (!edges.contains(edge)) {
        edges.append(edge);
    }
}

void VertexCircle::removeEdge(EdgeLine* edge) {
    edges.removeAll(edge);
}

const QList<EdgeLine*>& VertexCircle::getEdges() const {
    return edges;
}

void VertexCircle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Изменяем стиль при начале перемещения
    setPen(QPen(Qt::red, 3)); // Более толстая красная обводка
    setBrush(QBrush(Qt::yellow)); // Жёлтая заливка
    QGraphicsEllipseItem::mousePressEvent(event); // Передаём управление базовому классу
}

void VertexCircle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Восстанавливаем стиль после перемещения
    setPen(QPen(Qt::blue, 2));
    setBrush(QBrush(Qt::cyan));
    QGraphicsEllipseItem::mouseReleaseEvent(event); // Передаём управление базовому классу
}

void VertexCircle::setMovable(bool movable) {
    if (movable) {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    } else {
        setFlags(QGraphicsItem::ItemIsSelectable); // Только выделение
    }
}

void VertexCircle::moveTo(QPoint point) {
    this->vertex->MoveTo(point.x(), point.y());
    for (int i = 0; i < this->edges.count(); i++) {
        this->edges[i]->updatePosition();
    }
}

void VertexCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Проверяем, является ли объект выбранным
    bool isSelected = option->state & QStyle::State_Selected;

    if (isSelected) {
        // Кастомная отрисовка для выбранного объекта
        painter->setPen(QPen(Qt::red, 3, Qt::DashLine)); // Красная пунктирная обводка
        painter->setBrush(QBrush(Qt::yellow));          // Жёлтая заливка
    } else {
        // Отрисовка для обычного состояния
        painter->setPen(QPen(Qt::blue, 2)); // Синяя обводка
        painter->setBrush(QBrush(Qt::cyan)); // Голубая заливка
    }

    // Рисуем эллипс
    painter->drawEllipse(rect());
}
