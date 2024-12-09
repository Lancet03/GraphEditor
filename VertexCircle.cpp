#include "VertexCircle.h"
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>


VertexCircle::VertexCircle(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsEllipseItem(x, y, width, height, parent)
{
    // Устанавливаем базовый стиль
    setPen(QPen(Qt::blue, 2));
    setBrush(QBrush(Qt::cyan));
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
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
