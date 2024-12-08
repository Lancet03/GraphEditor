#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include "GraphVertex.h"
#include <memory>

class GraphEdge
{
public:
    GraphEdge();
    // int from;
    // int to;
    std::shared_ptr<GraphVertex> from;
    std::shared_ptr<GraphVertex> to;
    double weight;
private:

};

#endif // GRAPHEDGE_H
