#include "VertexCircle.h"
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QGraphicsProxyWidget>

VertexCircle::VertexCircle(qreal x, qreal y, qreal radius, Graph* graph, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), m_radius(radius)
{
    this->vertex = new GraphVertex(x, y, graph->calcUniqueVertexId());
    this->vertex->radius = radius;
    this->m_radius = radius;
    this->graph = graph;


    this->SetStartParameters();

    // Создаём текстовый элемент
    this->name = new QGraphicsTextItem(this);

    this->name->setParentItem(this);
    // name->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true); // Полностью прозрачный для событий
    this->name->setAcceptedMouseButtons(Qt::NoButton);
    // name->setZValue(-1);
    this->name->setPlainText(""); // По умолчанию текст пуст
    this->updateNamePosition();
}

VertexCircle::VertexCircle(GraphVertex* vertex, Graph* graph, QGraphicsItem *parent) {
    this->vertex = vertex;
    this->graph = graph;
    this->m_radius = this->vertex->radius;

    this->SetStartParameters();

    // Создаём текстовый элемент
    this->name = new QGraphicsTextItem(this);
    this->name->setAcceptedMouseButtons(Qt::NoButton);
    name->setPlainText(QString::fromStdString(vertex->GetName()));
    this->updateNamePosition();
}

void VertexCircle::SetStartParameters() {
    // Устанавливаем начальные параметры
    setRect(this->vertex->xPos - this->m_radius, this->vertex->yPos - this->m_radius, 2 * this->m_radius, 2 * this->m_radius);
    // Устанавливаем базовый стиль
    setPen(QPen(Qt::blue, 2));
    setBrush(QBrush(Qt::cyan));
    setFlags(QGraphicsItem::ItemIsSelectable);
    this->setData(0, "VertexCircle");
}

qreal VertexCircle::getRadius() const
{
    return m_radius;
}

void VertexCircle::setName(const QString &name) {
    this->name->setPlainText(name);
    this->vertex->name = name.toStdString();
    updateNamePosition(); // Обновляем позицию текста
}

QString VertexCircle::getName() const {
    return name->toPlainText();
}

void VertexCircle::updateNamePosition() {
    // Располагаем текст в центре вершины
    QPointF center = boundingRect().center();
    // QPointF center = rect().center();
    this->name->setPos(center.x() - this->name->boundingRect().width() / 2,
                     center.y() - this->name->boundingRect().height() / 2);
}

void VertexCircle::setRadius(qreal radius)
{
    qDebug() << "Vertex radius changed " << radius;
    // this->vertex->radius = radius;
    this->vertex->SetRadius(radius);

    this->m_radius = radius;
    // Обновляем размер круга
    setRect(rect().center().x() - radius, rect().center().y() - radius, 2 * radius, 2 * radius);
}

void VertexCircle::addEdge(EdgeLine* edge) {
    if (!edges.contains(edge)) {
        edges.append(edge);
    }
}

void VertexCircle::removeEdge(EdgeLine* edge) {
    edges.removeAll(edge);
}

const QList<EdgeLine*>& VertexCircle::getEdges() const {
    return edges;
}

int VertexCircle::GetId() {
    return this->vertex->id;
}

void VertexCircle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Изменяем стиль при начале перемещения
    setPen(QPen(Qt::red, 3)); // Более толстая красная обводка
    setBrush(QBrush(Qt::yellow)); // Жёлтая заливка
    QGraphicsEllipseItem::mousePressEvent(event); // Передаём управление базовому классу
}

void VertexCircle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Восстанавливаем стиль после перемещения
    setPen(QPen(Qt::blue, 2));
    setBrush(QBrush(Qt::cyan));
    QGraphicsEllipseItem::mouseReleaseEvent(event); // Передаём управление базовому классу
}

void VertexCircle::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    this->editName();
    QGraphicsEllipseItem::mouseDoubleClickEvent(event);
}

void VertexCircle::editName() {
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

void VertexCircle::setMovable(bool movable) {
    if (movable) {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    } else {
        setFlags(QGraphicsItem::ItemIsSelectable); // Только выделение
    }
}

void VertexCircle::moveTo(QPointF point) {

    this->vertex->MoveTo(point.x(), point.y());
    for (int i = 0; i < this->edges.count(); i++) {
        this->edges[i]->updatePosition();
    }
}

void VertexCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Проверяем, является ли объект выбранным
    bool isSelected = option->state & QStyle::State_Selected;

    if (isSelected) {
        // Кастомная отрисовка для выбранного объекта
        painter->setPen(QPen(Qt::red, 3, Qt::DashLine)); // Красная пунктирная обводка
        painter->setBrush(QBrush(Qt::yellow));          // Жёлтая заливка
    } else {
        // Отрисовка для обычного состояния
        painter->setPen(QPen(Qt::blue, 2)); // Синяя обводка
        painter->setBrush(QBrush(Qt::cyan)); // Голубая заливка
    }

    // Рисуем эллипс
    painter->drawEllipse(rect());
}
