#include "GraphEdge.h"
#include <memory>

GraphEdge::GraphEdge(std::shared_ptr<GraphVertex> from, std::shared_ptr<GraphVertex> to, std::string name, double weight) {
    this->from = from;
    this->to =  to;
    this->SetName(name);
    this->SetWeight(weight);
}

GraphEdge::GraphEdge(GraphEdge* edge) {
    this->from = edge->from;
    this->to = edge->to;
    this->SetName(edge->name);
    this->SetWeight(edge->weight);
}
