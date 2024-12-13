#include "GraphVertex.h"
#include <QDebug>


GraphVertex::GraphVertex(double x, double y, int id, int radius, std::string name) {
    this->xPos = x;
    this->yPos = y;
    this->id = id;
    this->SetRadius(radius);
    this->SetName(name);
}

GraphVertex::GraphVertex(const std::shared_ptr<GraphVertex>& vertex) {
    this->xPos = vertex->xPos;
    this->yPos = vertex->yPos;
    this->id = vertex->id;
    this->SetName(vertex->name);
    this->SetRadius(vertex->radius);
}

GraphVertex::GraphVertex(GraphVertex* vertex) {
    this->xPos = vertex->xPos;
    this->yPos = vertex->yPos;
    this->id = vertex->id;
    this->SetName(vertex->name);
    this->SetRadius(vertex->radius);
}

void GraphVertex::MoveTo(double x, double y) {
    this->xPos = x;
    this->yPos = y;
    qDebug() << "Object Vertex moved to " << x << " " << y;
}

bool GraphVertex::CheckIfInRadius(double x, double y) {
    double dx = x - this->xPos;
    double dy = y - this->yPos;
    return dx*dx + dy*dy <= this->radius * this->radius;
}

void GraphVertex::SetRadius(double r) {
    this->radius = r;
}
