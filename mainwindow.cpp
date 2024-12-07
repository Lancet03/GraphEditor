#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>
#include <QLabel>
#include <QDebug>

#include "GraphPlane.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->graphPlane = this->findChild<GraphPlane*>("graphicsView");

    // Создаем метки для отображения данных
    mousePosLabel = new QLabel("Cursor: (0, 0)", this);
    zoomLevelLabel = new QLabel("Zoom: 1.00x", this);

    // Добавляем метки в статусную строку
    statusBar()->addWidget(mousePosLabel);
    statusBar()->addPermanentWidget(zoomLevelLabel);

    // Подключаем сигналы от GraphPlane к слотам
    connect(graphPlane, &GraphPlane::mousePositionChanged, this, &MainWindow::updateMousePosition);
    connect(graphPlane, &GraphPlane::zoomLevelChanged, this, &MainWindow::updateZoomLevel);
    // Подключение сигналов для входа/выхода курсора
    connect(graphPlane, &GraphPlane::mouseEntered, this, &MainWindow::onMouseEntered);
    connect(graphPlane, &GraphPlane::mouseLeft, this, &MainWindow::onMouseLeft);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateMousePosition(const QPointF &pos)
{
    // Форматируем координаты и обновляем метку
    mousePosLabel->setText(QString("Cursor: (%1, %2)").arg(pos.x(), 0, 'f', 2).arg(pos.y(), 0, 'f', 2));
}

void MainWindow::updateZoomLevel(qreal scale)
{
    // Форматируем масштаб и обновляем метку
    zoomLevelLabel->setText(QString("Zoom: %1x").arg(scale, 0, 'f', 2));
}

void MainWindow::onMouseEntered()
{
    this->updateZoomLevel(this->graphPlane->getCurrentScale());
}

void MainWindow::onMouseLeft()
{
    mousePosLabel->setText("");
    zoomLevelLabel->setText("");
}
