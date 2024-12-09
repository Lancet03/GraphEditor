#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
#include <QButtonGroup>

#include "GraphPlane.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->graphPlane = this->findChild<GraphPlane*>("graphicsView");

    QPushButton* addVertexesModeBtn = this->findChild<QPushButton*>("AddVertexes");
    QPushButton* addEdgesModeBtn = this->findChild<QPushButton*>("AddEdges");
    QPushButton* viewModeBtn = this->findChild<QPushButton*>("ViewMode");
    QPushButton* editModeBtn = this->findChild<QPushButton*>("EditMode");
    QPushButton* moveModeBtn = this->findChild<QPushButton*>("MoveMode");

    // Создаем QButtonGroup и связываем кнопки
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(addVertexesModeBtn, 1);
    buttonGroup->addButton(addEdgesModeBtn, 2);
    buttonGroup->addButton(viewModeBtn, 3);
    buttonGroup->addButton(editModeBtn, 4);
    buttonGroup->addButton(moveModeBtn, 5);


    // Устанавливаем checkable для кнопок
    addVertexesModeBtn->setCheckable(true);
    addEdgesModeBtn->setCheckable(true);
    viewModeBtn->setCheckable(true);
    editModeBtn->setCheckable(true);
    moveModeBtn->setCheckable(true);

    viewModeBtn->setChecked(true);

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

void MainWindow::on_ViewMode_toggled(bool checked)
{
    qDebug() << "ViewMode";
    this->graphPlane->SetMode(Mode::VIEW);
}


void MainWindow::on_AddVertexes_toggled(bool checked)
{
    qDebug() << "AddVertexMode";
    this->graphPlane->SetMode(Mode::ADD_VERTEXES);
}


void MainWindow::on_AddEdges_toggled(bool checked)
{
    qDebug() << "AddEdgeMode";
    this->graphPlane->SetMode(Mode::ADD_EDGES);
}



void MainWindow::on_EditMode_toggled(bool checked)
{
    qDebug() << "EditMode";
    this->graphPlane->SetMode(Mode::EDIT);
}


void MainWindow::on_MoveMode_toggled(bool checked)
{
    qDebug() << "Move";
    this->graphPlane->SetMode(Mode::MOVE);
}

