#include "EdgeLine.h"
#include "VertexCircle.h"
#include <QPen>
#include <QGraphicsScene>

EdgeLine::EdgeLine(VertexCircle *start, VertexCircle *end, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), startVertex(start), endVertex(end)
{
    // Устанавливаем стиль линии
    setPen(QPen(Qt::black, 2));

    // Обновляем положение линии
    updatePosition();

    // Добавляем линию в списки кружков
    startVertex->addEdge(this);
    endVertex->addEdge(this);
}

void EdgeLine::updatePosition()
{
    // Устанавливаем новую линию между центрами кружков
    setLine(QLineF(startVertex->sceneBoundingRect().center(),
                   endVertex->sceneBoundingRect().center()));
}

void EdgeLine::removeSelf() {
    if (startVertex) {
        startVertex->removeEdge(this);
    }
    if (endVertex) {
        endVertex->removeEdge(this);
    }
    // Удаляем саму линию из сцены
    if (scene()) {
        scene()->removeItem(this);
    }

    delete this;
}
