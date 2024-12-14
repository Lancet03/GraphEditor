#ifndef VERTEXCIRCLE_H
#define VERTEXCIRCLE_H

#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QObject>
#include <QPainter>
#include <QPen>

#include "EdgeLine.h"
#include "Graph.h"
#include "GraphVertex.h"

class VertexCircle : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    explicit VertexCircle(qreal x, qreal y, qreal radius, Graph *graph,
                          QGraphicsItem *parent = nullptr);
    explicit VertexCircle(GraphVertex *vertex, Graph *graph,
                          QGraphicsItem *parent = nullptr);

    void setRadius(qreal radius);
    GraphVertex *vertex;
    Graph *graph;

    qreal getRadius() const;
    void setMovable(bool movable);
    void moveTo(QPointF point);

    void addEdge(EdgeLine *edge);
    void removeEdge(EdgeLine *edge);
    const QList<EdgeLine *> &getEdges() const;

    int GetId();

    void setName(const QString &name);
    QString getName() const;
signals:
    void stateChanged();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    qreal m_radius;
    QList<EdgeLine *> edges;
    QGraphicsTextItem *name;
    void updateNamePosition();
    void SetStartParameters();
    void editName();
};

#endif // VERTEXCIRCLE_H
