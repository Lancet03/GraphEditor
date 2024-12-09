#ifndef VERTEXCIRCLE_H
#define VERTEXCIRCLE_H

#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <QPainter>

class VertexCircle : public QGraphicsEllipseItem
{
public:
    explicit VertexCircle(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // Начало перемещения
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override; // Завершение перемещения
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override; // Кастомная отрисовка

};

#endif // VERTEXCIRCLE_H
