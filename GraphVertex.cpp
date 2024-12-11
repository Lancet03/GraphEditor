#include "GraphVertex.h"
#include <QDebug>


GraphVertex::GraphVertex(double x, double y) {
    this->xPos = x;
    this->yPos = y;
}

GraphVertex::GraphVertex(const std::shared_ptr<GraphVertex>& vertex) {
    this->xPos = vertex->xPos;
    this->yPos = vertex->yPos;
    this->id = vertex->id;
    this->name = vertex->name;
    this->radius = vertex->radius;
}

void GraphVertex::MoveTo(double x, double y) {
    this->xPos = x;
    this->yPos = y;
    // qDebug() << "Object Vertex moved to " << x << " " << y;
}

bool GraphVertex::CheckIfInRadius(double x, double y) {
    double dx = x - this->xPos;
    double dy = y - this->yPos;
    return dx*dx + dy*dy <= this->radius * this->radius;
}
