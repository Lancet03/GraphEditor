#ifndef VERTEXCIRCLE_H
#define VERTEXCIRCLE_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <QPainter>

#include "GraphVertex.h"
#include "EdgeLine.h"
#include "Graph.h"
#include <memory>

class VertexCircle : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    explicit VertexCircle(qreal x, qreal y, qreal radius, Graph* graph, QGraphicsItem *parent = nullptr);
    explicit VertexCircle(GraphVertex* vertex, Graph* graph, QGraphicsItem *parent = nullptr);
    // Метод для изменения радиуса
    void setRadius(qreal radius);
    GraphVertex* vertex;
    Graph* graph;

    // Получение текущего радиуса
    qreal getRadius() const;
    void setMovable(bool movable);
    void moveTo(QPointF point);

    void addEdge(EdgeLine* edge);
    void removeEdge(EdgeLine* edge);
    const QList<EdgeLine*>& getEdges() const;

    int GetId();
    // void removeSelf();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // Начало перемещения
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override; // Завершение перемещения
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override; // Кастомная отрисовка
    // QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void SetStartParameters();


private:
    qreal m_radius; // Радиус круга
    QList<EdgeLine*> edges; // Список линий, связанных с этим кружком
};

#endif // VERTEXCIRCLE_H
