#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

#include "GraphEdge.h"
#include "GraphVertex.h"

class Graph {
public:
    std::vector<std::shared_ptr<GraphEdge>> edges;
    std::vector<std::shared_ptr<GraphVertex>> vertexes;
    std::string name;

    virtual ~Graph() {};
    void PrintCorrespMatrix();
    std::vector<std::vector<double>> BuildCorrespMatrix();
    std::shared_ptr<GraphVertex> getVertexByPos(double x, double y);
    std::shared_ptr<GraphVertex> addVertex(double xPos, double yPos);

private:
    double GetEdgeWeigth(std::shared_ptr<GraphVertex> from, std::shared_ptr<GraphVertex> to);
};

#endif // GRAPH_H
