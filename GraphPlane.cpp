#include "GraphPlane.h"

GraphPlane::GraphPlane(QWidget *parent) : QGraphicsView(parent)
{
    // Создаем сцену и устанавливаем её в QGraphicsView
    QGraphicsScene *scene = new QGraphicsScene(this);
    setScene(scene);

    // Настройки для прокрутки и масштабирования
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Рисуем координатную сетку
    drawGrid();
}

void GraphPlane::drawGrid() {
    int gridSize = 20; // Размер клетки в пикселях
    int gridCount = 50; // Количество клеток (по полю 1000x1000)

    // Горизонтальные линии
    for (int i = -gridCount; i <= gridCount; ++i) {
        scene()->addLine(-gridCount * gridSize, i * gridSize, gridCount * gridSize, i * gridSize, QPen(Qt::gray));
    }
    // Вертикальные линии
    for (int i = -gridCount; i <= gridCount; ++i) {
        scene()->addLine(i * gridSize, -gridCount * gridSize, i * gridSize, gridCount * gridSize, QPen(Qt::gray));
    }

    // Оси X и Y
    scene()->addLine(-gridCount * gridSize, 0, gridCount * gridSize, 0, QPen(Qt::black, 2));
    scene()->addLine(0, -gridCount * gridSize, 0, gridCount * gridSize, QPen(Qt::black, 2));
}

void GraphPlane::wheelEvent (QWheelEvent *event){
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}
