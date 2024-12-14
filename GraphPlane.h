#ifndef GRAPHPLANE_H
#define GRAPHPLANE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPointF>

#include "Graph.h"
#include "Mode.h"
#include "VertexCircle.h"
#include "EdgeLine.h"
#include <memory>

class GraphPlane : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphPlane(QWidget *parent = nullptr);

    QList<VertexCircle*> vertexCircles;
    VertexCircle* AddVertex(QPointF vertex_center);
    VertexCircle* AddVertex(std::shared_ptr<GraphVertex> vertex);
    VertexCircle* RegisterVertex(VertexCircle* circle);
    void RemoveVertex(VertexCircle* vertex);

    QList<EdgeLine*> edgeLines;
    EdgeLine* AddEdge(VertexCircle *start, VertexCircle *end);
    EdgeLine* AddEdge(std::shared_ptr<GraphEdge> edge);
    EdgeLine* RegisterEdge(EdgeLine* edgeLine);
    void RemoveEdge(EdgeLine* edge);

    qreal getCurrentScale() const;
    Graph* graph;
    Graph* SetGraph(Graph* g);
    void SetMode(Mode m) { this->mode = m; };
    Mode GetMode(Mode m) { return this->mode; };

    VertexCircle* GetVertexById(int id);

    void ClearGraph();
signals:
    void mousePositionChanged(const QPointF &scenePos);
    void zoomLevelChanged(qreal scale);
    void mouseEntered();
    void mouseLeft();
    void graphChanged(Graph* graph);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void wheelEvent(QWheelEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    qreal currentScale;
    Mode mode;
    QGraphicsScene *scene;
    VertexCircle *firstSelectedCircle = nullptr;
    VertexCircle *secondSelectedCircle = nullptr;
    QGraphicsLineItem *tempLine = nullptr;

    VertexCircle* selectedCircle = nullptr;

    bool isDragging = false;
    QPoint lastMousePos;
};

#endif // GRAPHPLANE_H
