#ifndef GRAPHPLANE_H
#define GRAPHPLANE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPointF>

class GraphPlane : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphPlane(QWidget *parent = nullptr);

    // Получение текущего масштаба
    qreal getCurrentScale() const;

signals:
    void mousePositionChanged(const QPointF &scenePos); // Сигнал для обновления координат
    void zoomLevelChanged(qreal scale);                // Сигнал для изменения масштаба
    void mouseEntered();  // Сигнал, когда курсор входит в область
    void mouseLeft();     // Сигнал, когда курсор покидает область


protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override; // Отрисовка координатной сетки
    void wheelEvent(QWheelEvent *event) override;                       // Масштабирование колесом мыши
    void mouseMoveEvent(QMouseEvent *event) override;                   // Отслеживание движения мыши
    void enterEvent(QEvent *event) override;  // Событие входа курсора в область
    void leaveEvent(QEvent *event) override;  // Событие выхода курсора из области

private:
    qreal currentScale; // Текущий уровень масштаба
};

#endif // GRAPHPLANE_H
