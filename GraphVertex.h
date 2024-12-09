#ifndef GRAPHVERTEX_H
#define GRAPHVERTEX_H
#include <string>
#include <memory>

class GraphVertex
{
public:
    // GraphVertex(double x, double y);
    GraphVertex(double x, double y);
    GraphVertex(const std::shared_ptr<GraphVertex>& vertex);

    int id;
    std::string name;
    double xPos;
    double yPos;
    double radius = 20;

    void MoveTo(double x, double y);
    bool CheckIfInRadius(double x, double y);
    void SetRadius(double r) { this-> radius = r; };
};

#endif // GRAPHVERTEX_H
