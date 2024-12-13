#include "EdgeLine.h"
#include "VertexCircle.h"
#include <QPen>
#include <QGraphicsScene>

#include <memory>
#include <cmath>

EdgeLine::EdgeLine(VertexCircle *start, VertexCircle *end, Graph* graph, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), startVertex(start), endVertex(end)
{
    this->graph = graph;
    this->edge = new GraphEdge(std::make_shared<GraphVertex>(start->vertex), std::make_shared<GraphVertex>(end->vertex));

    // Устанавливаем стиль линии
    setPen(QPen(Qt::black, 2));

    // Обновляем положение линии
    updatePosition();

    // Добавляем линию в списки кружков
    startVertex->addEdge(this);
    endVertex->addEdge(this);
}

EdgeLine::EdgeLine(GraphEdge* edge, VertexCircle *start, VertexCircle *end, Graph* graph, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), startVertex(start), endVertex(end)
{
    this->graph = graph;
    this->edge = edge;

    // Устанавливаем стиль линии
    setPen(QPen(Qt::black, 2));

    // Обновляем положение линии
    updatePosition();

    // Добавляем линию в списки кружков
    startVertex->addEdge(this);
    endVertex->addEdge(this);
}

void EdgeLine::updatePosition()
{
    this->prepareGeometryChange();
    // Устанавливаем новую линию между центрами кружков
    setLine(QLineF(startVertex->sceneBoundingRect().center(),
                   endVertex->sceneBoundingRect().center()));
    this->update();
}

QRectF EdgeLine::boundingRect() const {
    qreal extra = 10.0; // Дополнительное пространство для стрелки
    QRectF lineRect = QGraphicsLineItem::boundingRect();
    return lineRect.adjusted(-extra, -extra, extra, extra); // Расширяем границы
}

void EdgeLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    // Рисуем линию
    QGraphicsLineItem::paint(painter, option, widget);

    // Если ребро ориентированное, рисуем стрелку
    drawArrow(painter);
}

void EdgeLine::drawArrow(QPainter* painter) {
    const qreal arrowSize = 10.0;

    QLineF line = this->line();
    QPointF p1 = line.p1();
    QPointF p2 = line.p2();

    // Вычисляем направление стрелки
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF arrowP1 = p2 + QPointF(std::sin(angle - M_PI / 3) * arrowSize,
                                   std::cos(angle - M_PI / 3) * arrowSize);
    QPointF arrowP2 = p2 + QPointF(std::sin(angle - M_PI + M_PI / 3) * arrowSize,
                                   std::cos(angle - M_PI + M_PI / 3) * arrowSize);

    // Рисуем стрелку
    QPolygonF arrowHead;
    arrowHead << p2 << arrowP1 << arrowP2;
    painter->setBrush(Qt::black);
    painter->drawPolygon(arrowHead);

}
