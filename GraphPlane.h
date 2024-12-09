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

class GraphPlane : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphPlane(QWidget *parent = nullptr);

    // Получение текущего масштаба
    qreal getCurrentScale() const;
    Graph* graph;
    Graph* SetGraph(Graph* g);
    void SetMode(Mode m) { this->mode = m; };
    Mode GetMode(Mode m) { return this->mode; };
signals:
    void mousePositionChanged(const QPointF &scenePos); // Сигнал для обновления координат
    void zoomLevelChanged(qreal scale);                // Сигнал для изменения масштаба
    void mouseEntered();  // Сигнал, когда курсор входит в область
    void mouseLeft();     // Сигнал, когда курсор покидает область


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

    VertexCircle* selectedCircle = nullptr;

    bool isDragging = false;         // Флаг режима перетаскивания
    QPoint lastMousePos;             // Последняя позиция мыши

    void connectCircles(); // Метод для соединения кружков линией
};

#endif // GRAPHPLANE_H
