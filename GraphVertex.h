#ifndef GRAPHVERTEX_H
#define GRAPHVERTEX_H
#include <string>
#include <memory>

class GraphVertex
{
public:
    GraphVertex(double x, double y, int id, int radius = 20, std::string name = "");
    GraphVertex(const std::shared_ptr<GraphVertex>& vertex);
    GraphVertex(GraphVertex* vertex);

    int id;

    void SetName(std::string name) { this->name = name; };
    std::string GetName() { return this->name; };
    std::string name;
    double xPos;
    double yPos;
    double radius;

    void MoveTo(double x, double y);
    bool CheckIfInRadius(double x, double y);
    void SetRadius(double r);
};

#endif // GRAPHVERTEX_H
