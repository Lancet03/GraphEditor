#include "EdgeLine.h"
#include "VertexCircle.h"
#include <QPen>
#include <QGraphicsScene>

#include <memory>

EdgeLine::EdgeLine(VertexCircle *start, VertexCircle *end, Graph* graph, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), startVertex(start), endVertex(end)
{
    this->graph = graph;
    this->edge = new GraphEdge(std::make_shared<GraphVertex>(start->vertex), std::make_shared<GraphVertex>(end->vertex));

    // Устанавливаем стиль линии
    setPen(QPen(Qt::black, 2));

    // Обновляем положение линии
    updatePosition();

    // Добавляем линию в списки кружков
    startVertex->addEdge(this);
    endVertex->addEdge(this);
}

EdgeLine::EdgeLine(GraphEdge* edge, VertexCircle *start, VertexCircle *end, Graph* graph, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), startVertex(start), endVertex(end)
{
    this->graph = graph;
    this->edge = edge;

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

// void EdgeLine::removeSelf() {
//     if (startVertex) {
//         startVertex->removeEdge(this);
//     }
//     if (endVertex) {
//         endVertex->removeEdge(this);
//     }
//     // Удаляем саму линию из сцены
//     if (scene()) {
//         scene()->removeItem(this);
//     }

//     graph->removeEdge(this->edge);

//     delete this;
// }
