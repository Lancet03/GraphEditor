#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include "GraphVertex.h"
#include <memory>

class GraphEdge
{
public:
    GraphEdge(std::shared_ptr<GraphVertex> from, std::shared_ptr<GraphVertex> to);
    GraphEdge(GraphEdge* edge);
    // int from;
    // int to;
    std::shared_ptr<GraphVertex> from;
    std::shared_ptr<GraphVertex> to;

    void SetWeight(double weight) { this->weight = weight; }
    double weight;
private:
};

#endif // GRAPHEDGE_H
