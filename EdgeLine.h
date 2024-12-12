#ifndef EDGELINE_H
#define EDGELINE_H

#include <QGraphicsLineItem>
#include "GraphEdge.h"

class VertexCircle;

class EdgeLine : public QObject, public QGraphicsLineItem
{
    Q_OBJECT

public:
    EdgeLine(VertexCircle *start, VertexCircle *end, QGraphicsItem *parent = nullptr);

    // Обновить линию в соответствии с текущими позициями кружков
    void updatePosition();
    void removeSelf(); // Удаляет связь между линией и кружками
    GraphEdge* edge;

private:
    VertexCircle *startVertex; // Начальный кружок
    VertexCircle *endVertex;   // Конечный кружок
};

#endif // EDGELINE_H
