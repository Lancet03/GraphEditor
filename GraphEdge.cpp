#include "GraphEdge.h"
#include <memory>

GraphEdge::GraphEdge(std::shared_ptr<GraphVertex> from, std::shared_ptr<GraphVertex> to) {
    this->from = from;
    this->to =  to;
}

GraphEdge::GraphEdge(GraphEdge* edge) {
    this->from = edge->from;
    this->to = edge->to;
    this->weight = edge->weight;
}
