#include "Graph.h"
#include "GraphVertex.h"
#include <iostream>
#include <iomanip>

    // int id = this->calcUniqueVertexId();
std::shared_ptr<GraphVertex> Graph::addVertex(double xPos, double yPos, int id, int radius) {
    int graphId = id;
    if (id < 0) {
        graphId = this->calcUniqueVertexId();
    }
    std::shared_ptr<GraphVertex> vetrex = std::make_shared<GraphVertex>(xPos, yPos, graphId);
    this->vertexes.push_back(vetrex);

    return vetrex;
}

std::shared_ptr<GraphVertex> Graph::addVertex(GraphVertex* vertex) {
    std::shared_ptr<GraphVertex> vertex_sptr(vertex);
    this->vertexes.push_back(vertex_sptr);

    return vertex_sptr;
}

void Graph::removeVertex(GraphVertex* vertex) {    
    for (int i = 0; i < this->vertexes.size(); i++) {
        if (vertex->id == this->vertexes[i]->id) {
            this->vertexes.erase(this->vertexes.begin() + i);
            return;
        }
    }
}


std::shared_ptr<GraphEdge> Graph::addEdge(int from_vertex_id, int to_vertex_id) {
    std::shared_ptr<GraphVertex> from = this->getVertexById(from_vertex_id);
    std::shared_ptr<GraphVertex> to = this->getVertexById(to_vertex_id);

    std::shared_ptr<GraphEdge> edge = std::make_shared<GraphEdge>(from, to);
    this->edges.push_back(edge);
    return edge;
}

std::shared_ptr<GraphEdge> Graph::addEdge(GraphEdge* edge) {
    std::shared_ptr<GraphEdge> edge_sptr = std::make_shared<GraphEdge>(edge);
    this->edges.push_back(edge_sptr);
    return edge_sptr;
}

void Graph::removeEdge(GraphEdge *edge) {
    for (int i = 0; i < this->edges.size(); i++) {
        if (this->edges[i]->from->id == edge->from->id &&
            this->edges[i]->to->id == edge->to->id) {
            this->edges.erase(this->edges.begin() + i);
            return;
        }
    }
}

int Graph::calcUniqueVertexId() {
    int id = this->vertexes.size();
    while (this->getVertexById(id) != nullptr) {
        id++;
    }

    return id;
}

void Graph::PrintCorrespMatrix() {
    std::vector<std::vector<double>> matrix = this->BuildCorrespMatrix();

    if (this->name.length()) {
        std::cout << "Graph: " << this->name << std::endl;
    }

    std::cout << "  ";
    for (int i = 0; i < (int)matrix.size(); i++) {

        std::cout << std::setw(6) << std::setprecision(2) << this->vertexes[i]->name ;
    }
    std::cout << std::endl;

    for (int i = 0; i < (int)matrix.size(); i++) {
        std::cout << this->vertexes[i]->name << " ";
        for (int j = 0; j < (int)matrix[i].size(); j++) {
            std::cout << std::setw(6) << std::setprecision(2) << matrix[i][j];
        }
        std::cout << std::endl;
    }
}


std::vector<std::vector<double>> Graph::BuildCorrespMatrix() {
    int matrixSize = this->vertexes.size();
    std::vector<std::vector<double>> matrix;

    for (int i = 0; i < matrixSize; i++) {
        matrix.push_back({});
        for (int j = 0; j < matrixSize; j++) {
            matrix[i].push_back(0);
        }
    }

    for (int i = 0; i < matrixSize; i++) {
        std::shared_ptr<GraphVertex> vertex_i = this->vertexes[i];

        for (int j = 0; j < matrixSize; j++) {
            std::shared_ptr<GraphVertex> vertex_j = this->vertexes[j];
            matrix[i][j] = this->GetEdgeWeigth(std::make_shared<GraphVertex>(this->vertexes[i]), std::make_shared<GraphVertex>(this->vertexes[j]));
        }
    }

    return matrix;
}

double Graph::GetEdgeWeigth(std::shared_ptr<GraphVertex> from, std::shared_ptr<GraphVertex> to) {
    for (int i = 0; i < (int)this->edges.size(); i++) {
        if (from == this->edges[i]->from && to == this->edges[i]->to) {
            return this->edges[i]->weight;
        }
    }

    return 0;
}

std::shared_ptr<GraphVertex> Graph::getVertexById(int id) {
    for (int i = 0; i < this->vertexes.size(); i++) {
        if (this->vertexes[i]->id == id) {
            return this->vertexes[i];
        }
    }

    return nullptr;
}

