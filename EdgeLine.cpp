#include "EdgeLine.h"
#include <QPen>

EdgeLine::EdgeLine(VertexCircle *start, VertexCircle *end, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), startVertex(start), endVertex(end)
{
    // Устанавливаем стиль линии
    setPen(QPen(Qt::black, 2));

    // Обновляем положение линии
    updatePosition();

    // Подключаем сигналы изменения позиции кружков
    connect(startVertex, &VertexCircle::positionChanged, this, &EdgeLine::updatePosition);
    connect(endVertex, &VertexCircle::positionChanged, this, &EdgeLine::updatePosition);
}

void EdgeLine::updatePosition()
{
    // Устанавливаем новую линию между центрами кружков
    setLine(QLineF(startVertex->sceneBoundingRect().center(),
                   endVertex->sceneBoundingRect().center()));
}
