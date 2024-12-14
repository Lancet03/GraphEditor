#include "EdgeLine.h"
#include "VertexCircle.h"
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <QDebug>

#include <memory>
#include <cmath>

EdgeLine::EdgeLine(VertexCircle *start, VertexCircle *end, Graph* graph, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), startVertex(start), endVertex(end)
{
    this->graph = graph;

    this->edge = new GraphEdge(std::shared_ptr<GraphVertex>(start->vertex), std::shared_ptr<GraphVertex>(end->vertex));

    // Устанавливаем стиль линии
    setPen(QPen(Qt::black, 2));

    // Создаём текстовый элемент
    this->name = new QGraphicsTextItem(this);
    this->name->setPlainText(""); // По умолчанию текст пуст
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

    // Создаём текстовый элемент
    this->name = new QGraphicsTextItem(this);
    this->name->setPlainText(QString::fromStdString(edge->GetName()));
    // Обновляем положение линии
    updatePosition();

    // Добавляем линию в списки кружков
    startVertex->addEdge(this);
    endVertex->addEdge(this);
}

void EdgeLine::updatePosition()
{
    this->prepareGeometryChange();
    // Координаты центров вершин
    QPointF startCenter = startVertex->sceneBoundingRect().center();
    QPointF endCenter = endVertex->sceneBoundingRect().center();

    // Вектор от начала к концу
    QLineF line(startCenter, endCenter);

    // Радиусы кружков
    qreal startRadius = startVertex->getRadius();
    qreal endRadius = endVertex->getRadius();

    // Пересечение линии с окружностью начальной вершины
    QPointF startPoint = line.pointAt(startRadius / line.length());

    // Пересечение линии с окружностью конечной вершины
    QPointF endPoint = line.pointAt(1.0 - (endRadius / line.length()));

    // Устанавливаем линию
    setLine(QLineF(startPoint, endPoint));

    this->updateNamePosition();
    this->update();
}

void EdgeLine::setName(const QString &name) {
    this->name->setPlainText(name);
    this->edge->SetName(name.toStdString());
    updateNamePosition(); // Обновляем позицию текста

    emit stateChanged();
}

QString EdgeLine::getName() const {
    return this->name->toPlainText();
}

void EdgeLine::updateNamePosition() {
    // Располагаем текст посередине линии
    QLineF line = this->line();
    QPointF midpoint = line.pointAt(0.5); // Середина линии
    this->name->setPos(midpoint.x() - this->name->boundingRect().width() / 2,
                     midpoint.y() - this->name->boundingRect().height() / 2);
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

void EdgeLine::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsLineItem::mouseDoubleClickEvent(event);
    editName();
}

void EdgeLine::editName() {
    if (!scene()) {
        qWarning() << "The item is not part of a scene.";
        return;
    }

    // Создаём текстовое поле для редактирования
    QLineEdit* lineEdit = new QLineEdit(this->name->toPlainText());
    lineEdit->setAlignment(Qt::AlignCenter);

    // Добавляем текстовое поле в сцену через ProxyWidget
    QGraphicsProxyWidget* proxy = scene()->addWidget(lineEdit);
    proxy->setPos(this->name->scenePos());

    // Устанавливаем фокус на поле ввода
    lineEdit->setFocus();

    // Сигнал на завершение редактирования
    connect(lineEdit, &QLineEdit::editingFinished, [this, lineEdit, proxy]() {
        setName(lineEdit->text());
        proxy->deleteLater(); // Корректно удаляем ProxyWidget
    });
}
