#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include "GraphVertex.h"
#include <memory>
#include <string>

class GraphEdge
{
public:
    GraphEdge(std::shared_ptr<GraphVertex> from, std::shared_ptr<GraphVertex> to, std::string name = "", double weigth = 1);
    GraphEdge(GraphEdge* edge);
    // int from;
    // int to;
    std::shared_ptr<GraphVertex> from;
    std::shared_ptr<GraphVertex> to;
    std::string GetName() { return this->name; };
    void SetName(std::string name) { this->name = name; };
    std::string name;

    void SetWeight(double weight) { this->weight = weight; }
    double weight;
private:
};

#endif // GRAPHEDGE_H
