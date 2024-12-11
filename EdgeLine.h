#ifndef EDGELINE_H
#define EDGELINE_H

#include <QGraphicsLineItem>
#include "VertexCircle.h" // Класс кружка

class EdgeLine : public QObject, public QGraphicsLineItem
{
    Q_OBJECT

public:
    EdgeLine(VertexCircle *start, VertexCircle *end, QGraphicsItem *parent = nullptr);

    // Обновить линию в соответствии с текущими позициями кружков
    void updatePosition();

private:
    VertexCircle *startVertex; // Начальный кружок
    VertexCircle *endVertex;   // Конечный кружок
};

#endif // EDGELINE_H
