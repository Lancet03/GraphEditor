#ifndef EDGELINE_H
#define EDGELINE_H

#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include "GraphEdge.h"
#include "Graph.h"

class VertexCircle;

class EdgeLine : public QObject, public QGraphicsLineItem
{
    Q_OBJECT

public:
    explicit EdgeLine(VertexCircle *start, VertexCircle *end, Graph* graph, QGraphicsItem *parent = nullptr);
    explicit EdgeLine(GraphEdge* edge, VertexCircle *start, VertexCircle *end,  Graph* graph, QGraphicsItem *parent = nullptr);
    explicit EdgeLine(VertexCircle *start, QGraphicsItem *parent = nullptr);

    void updatePosition();
    GraphEdge* edge;
    Graph* graph;

    VertexCircle *startVertex;
    VertexCircle *endVertex;
    QRectF boundingRect() const override;

    void setName(const QString &name);
    QString getName() const;

    void SetEndPos(QPointF);
signals:
    void stateChanged();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void drawArrow(QPainter* painter);
    QGraphicsTextItem* name;
    void updateNamePosition();
    void editName();
    void SetStartParameters();

    bool isTemporary = false;
    QPointF endPos;

};

#endif // EDGELINE_H
