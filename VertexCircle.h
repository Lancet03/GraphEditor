#ifndef VERTEXCIRCLE_H
#define VERTEXCIRCLE_H

#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <QPainter>

class VertexCircle : public QGraphicsEllipseItem
{
public:
    explicit VertexCircle(qreal x, qreal y, qreal radius, QGraphicsItem *parent = nullptr);
    // Метод для изменения радиуса
    void setRadius(qreal radius);

    // Получение текущего радиуса
    qreal getRadius() const;
    void setMovable(bool movable);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // Начало перемещения
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override; // Завершение перемещения
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override; // Кастомная отрисовка

private:
    qreal m_radius; // Радиус круга

};

#endif // VERTEXCIRCLE_H
