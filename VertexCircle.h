#ifndef VERTEXCIRCLE_H
#define VERTEXCIRCLE_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

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

    void setName(const QString &name);
    QString getName() const;
    // void removeSelf();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // Начало перемещения
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override; // Завершение перемещения
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override; // Кастомная отрисовка
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    qreal m_radius; // Радиус круга
    QList<EdgeLine*> edges; // Список линий, связанных с этим кружком
    QGraphicsTextItem* name; // Текстовый элемент для имени вершины
    void updateNamePosition();
    void SetStartParameters();
    void editName();
};

#endif // VERTEXCIRCLE_H
