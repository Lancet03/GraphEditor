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

    // std::vector<VertexCircle> vertexCircles;
    // std::vector<EdgeLine> edgeLines;
    QList<VertexCircle*> vertexCircles;
    VertexCircle* AddVertex(QPointF vertex_center);
    VertexCircle* AddVertex(std::shared_ptr<GraphVertex> vertex);
    void RemoveVertex(VertexCircle* vertex);

    QList<EdgeLine*> edgeLines;
    EdgeLine* AddEdge(VertexCircle *start, VertexCircle *end);
    EdgeLine* AddEdge(std::shared_ptr<GraphEdge> edge);
    void RemoveEdge(EdgeLine* edge);


    // Получение текущего масштаба
    qreal getCurrentScale() const;
    Graph* graph;
    Graph* SetGraph(Graph* g);
    void SetMode(Mode m) { this->mode = m; };
    Mode GetMode(Mode m) { return this->mode; };

    VertexCircle* GetVertexById(int id);
    void displayAdjacencyMatrix();

    void ClearGraph();
signals:
    void mousePositionChanged(const QPointF &scenePos); // Сигнал для обновления координат
    void zoomLevelChanged(qreal scale);                // Сигнал для изменения масштаба
    void mouseEntered();  // Сигнал, когда курсор входит в область
    void mouseLeft();     // Сигнал, когда курсор покидает область
    void graphChanged(Graph* graph);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override; // Отрисовка координатной сетки
    void wheelEvent(QWheelEvent *event) override;                       // Масштабирование колесом мыши

    void enterEvent(QEvent *event) override;  // Событие входа курсора в область
    void leaveEvent(QEvent *event) override;  // Событие выхода курсора из област
    void mousePressEvent(QMouseEvent *event) override; // Обработка кликов мыши
    void mouseMoveEvent(QMouseEvent *event) override;                   // Отслеживание движения мыши
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    qreal currentScale; // Текущий уровень масштаба
    Mode mode;
    QGraphicsScene *scene; // Сцена для управления объектами
    VertexCircle *firstSelectedCircle = nullptr;  // Первый выбранный кружок
    VertexCircle *secondSelectedCircle = nullptr; // Второй выбранный кружок
    QGraphicsLineItem *tempLine = nullptr;        // Временная линия

    VertexCircle* selectedCircle = nullptr;

    bool isDragging = false;         // Флаг режима перетаскивания
    QPoint lastMousePos;             // Последняя позиция мыши

    // void connectCircles(VertexCircle *start, VertexCircle *end); // Метод для соединения кружков линией
    void updateEdge(GraphEdge &edge); // Метод для обновления линии
};

#endif // GRAPHPLANE_H
