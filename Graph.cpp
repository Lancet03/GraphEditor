#include "Graph.h"
#include "GraphVertex.h"
#include <iostream>
#include <iomanip>

std::shared_ptr<GraphVertex> Graph::addVertex(double xPos, double yPos) {
     std::shared_ptr<GraphVertex> vetrex = std::make_shared<GraphVertex>(xPos, yPos);
    this->vertexes.push_back(vetrex);

    return vetrex;
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
