#include "EdgeLine.h"
#include "VertexCircle.h"
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>
#include <QPen>

#include <cmath>
#include <memory>

EdgeLine::EdgeLine(VertexCircle *start, VertexCircle *end, Graph *graph,
                   QGraphicsItem *parent)
    : QGraphicsLineItem(parent), startVertex(start), endVertex(end) {
    this->graph = graph;

    this->edge = new GraphEdge(std::shared_ptr<GraphVertex>(start->vertex),
                               std::shared_ptr<GraphVertex>(end->vertex));

    this->SetStartParameters();

    this->name = new QGraphicsTextItem(this);
    this->name->setDefaultTextColor(Qt::black);
    this->name->setFont(QFont("Arial", 12, QFont::Bold, true));
    this->name->setPlainText("");

    updatePosition();

    startVertex->addEdge(this);
    endVertex->addEdge(this);
}

EdgeLine::EdgeLine(GraphEdge *edge, VertexCircle *start, VertexCircle *end,
                   Graph *graph, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), startVertex(start), endVertex(end) {
    this->graph = graph;
    this->edge = edge;

    this->SetStartParameters();

    this->name = new QGraphicsTextItem(this);
    this->name->setDefaultTextColor(Qt::black);
    this->name->setFont(QFont("Arial", 12, QFont::Bold, true));
    this->name->setPlainText(QString::fromStdString(edge->GetName()));

    updatePosition();

    startVertex->addEdge(this);
    endVertex->addEdge(this);
}

EdgeLine::EdgeLine(VertexCircle *start, QGraphicsItem *parent): QGraphicsLineItem(parent), startVertex(start) {
    this->SetStartParameters();
    this->endPos = start->boundingRect().center();
    this->isTemporary = true;
    this->setData(0, "TempEdgeLine");
    updatePosition();
}

void EdgeLine::SetStartParameters() {
    setPen(QPen(Qt::black, 2));
    this->setData(0, "EdgeLine");
}

void EdgeLine::updatePosition() {
    this->prepareGeometryChange();
    QPointF endCenter;
    qreal endRadius;
    QPointF endPoint;
    QLineF line;

    QPointF startCenter = startVertex->sceneBoundingRect().center();
    qreal startRadius = startVertex->getRadius();
    if (!this->isTemporary) {
        endCenter = endVertex->sceneBoundingRect().center();
        endRadius = endVertex->getRadius();
        line = QLineF(startCenter, endCenter);
        endPoint = line.pointAt(1.0 - (endRadius / line.length()));
    } else {
        line = QLineF(startCenter, endPos);
        endPoint = line.pointAt(1.0 - (3 / line.length()));
    }

    QPointF startPoint = line.pointAt(startRadius / line.length());

    setLine(QLineF(startPoint, endPoint));

    if (!this->isTemporary) {
        this->updateNamePosition();
    }

    this->update();
}

void EdgeLine::setName(const QString &name) {
    this->name->setPlainText(name);
    this->edge->SetName(name.toStdString());
    updateNamePosition();

    emit stateChanged();
}

QString EdgeLine::getName() const { return this->name->toPlainText(); }

void EdgeLine::SetEndPos(QPointF point) {
    this->endPos = point;
    this->updatePosition();
}

void EdgeLine::updateNamePosition() {
    QLineF line = this->line();
    QPointF midpoint = line.pointAt(0.5);
    this->name->setPos(midpoint.x() - this->name->boundingRect().width() / 2,
                     midpoint.y() - this->name->boundingRect().height() / 2);
}

QRectF EdgeLine::boundingRect() const {
    qreal extra = 10.0; // Дополнительное пространство для стрелки // Extra space for arrow
    QRectF lineRect = QGraphicsLineItem::boundingRect();
    return lineRect.adjusted(-extra, -extra, extra, extra); // Expand boundaries
}

void EdgeLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) {
    QGraphicsLineItem::paint(painter, option, widget);

    drawArrow(painter);
}

void EdgeLine::drawArrow(QPainter *painter) {
    const qreal arrowSize = 10.0;

    QLineF line = this->line();
    QPointF p1 = line.p1();
    QPointF p2 = line.p2();

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

void EdgeLine::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsLineItem::mouseDoubleClickEvent(event);
    editName();
}

void EdgeLine::editName() {
    if (!scene()) {
        qWarning() << "The item is not part of a scene.";
        return;
    }

    QLineEdit *lineEdit = new QLineEdit(this->name->toPlainText());
    lineEdit->setAlignment(Qt::AlignCenter);

    // Add text field in scene through ProxyWidget
    QGraphicsProxyWidget *proxy = scene()->addWidget(lineEdit);
    proxy->setPos(this->name->scenePos());

    lineEdit->setFocus();

    connect(lineEdit, &QLineEdit::editingFinished, [this, lineEdit, proxy]() {
        setName(lineEdit->text());
        proxy->deleteLater(); // Correctly delete ProxyWidget
    });
}
