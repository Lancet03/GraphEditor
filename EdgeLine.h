#ifndef EDGELINE_H
#define EDGELINE_H

#include <QGraphicsLineItem>
#include "GraphEdge.h"
#include "Graph.h"

class VertexCircle;

class EdgeLine : public QObject, public QGraphicsLineItem
{
    Q_OBJECT

public:
    explicit EdgeLine(VertexCircle *start, VertexCircle *end, Graph* graph, QGraphicsItem *parent = nullptr);
    explicit EdgeLine(GraphEdge* edge, VertexCircle *start, VertexCircle *end,  Graph* graph, QGraphicsItem *parent = nullptr);

    // Обновить линию в соответствии с текущими позициями кружков
    void updatePosition();
    // void removeSelf(); // Удаляет связь между линией и кружками
    GraphEdge* edge;
    Graph* graph;

    VertexCircle *startVertex; // Начальный кружок
    VertexCircle *endVertex;   // Конечный кружок
    QRectF boundingRect() const override;
protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    void drawArrow(QPainter* painter); // Рисование стрелки
};

#endif // EDGELINE_H
