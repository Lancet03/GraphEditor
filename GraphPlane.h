#ifndef GRAPHPLANE_H
#define GRAPHPLANE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QWheelEvent>
#include <QMouseEvent>

class GraphPlane : public QGraphicsView
{
    Q_OBJECT

public:
    GraphPlane(QWidget *parent = nullptr);

protected:
    // Функция для рисования сетки
    void drawGrid();

    // Реализация масштабирования при прокрутке колеса мыши
    void wheelEvent(QWheelEvent *event) override;
};

#endif // GRAPHPLANE_H



