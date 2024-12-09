#include "VertexCircle.h"
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>


VertexCircle::VertexCircle(qreal x, qreal y, qreal radius, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), m_radius(radius)
{

    // Устанавливаем начальные параметры
    setRect(x - radius, y - radius, 2 * radius, 2 * radius);
    // Устанавливаем базовый стиль
    setPen(QPen(Qt::blue, 2));
    setBrush(QBrush(Qt::cyan));
    setFlags(QGraphicsItem::ItemIsSelectable);
}

qreal VertexCircle::getRadius() const
{
    return m_radius;
}


void VertexCircle::setRadius(qreal radius)
{
    m_radius = radius;
    // Обновляем размер круга
    setRect(rect().center().x() - radius, rect().center().y() - radius, 2 * radius, 2 * radius);
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
