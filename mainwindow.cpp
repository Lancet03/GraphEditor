#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
#include <QButtonGroup>
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <memory>

#include "GraphPlane.h"
#include "Graph.h"

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
    mousePosLabel->setText(QString("Cursor: (%1, %2)").arg(pos.x(), 0, 'f', 2).arg(-pos.y(), 0, 'f', 2));
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

void MainWindow::on_save_triggered()
{
    this->saveGraph();
}


void MainWindow::on_open_triggered()
{
    this->loadGraphFromJson();
}


void MainWindow::saveGraph() {
    // Открываем диалог для выбора файла
    QString fileName = QFileDialog::getSaveFileName(
        nullptr,
        "Сохранить граф",           // Заголовок окна
        "",                     // Начальная папка
        "JSON Files (*.json)"   // Фильтры типов файлов
        );

    // Проверяем, был ли файл выбран
    if (fileName.isEmpty()) {
        QMessageBox::information(nullptr, "Save Graph", "Файл не выбран!");
        return;
    }

    // Добавляем расширение, если его нет
    if (!fileName.endsWith(".json", Qt::CaseInsensitive)) {
        fileName += ".json";
    }

    // Пример данных для сохранения
    QJsonObject graphJson;

    QJsonArray vertexesArray;
    for (const auto &vertex : this->graphPlane->graph->vertexes) {
        QJsonObject vertexJson;
        vertexJson["id"] = vertex->id;
        vertexJson["x"] = vertex->xPos;
        vertexJson["y"] = vertex->yPos;
        vertexJson["radius"] = vertex->radius;
        vertexJson["name"] = QString::fromStdString(vertex->GetName());
        qDebug()  << "Json vertex to save " << vertexJson;
        qDebug() << "Json pos ot save " << vertex->xPos << " " << vertex->yPos;

        vertexesArray.append(vertexJson);
    }
    graphJson["vertexes"] = vertexesArray;

    // Сериализация рёбер
    QJsonArray edgesArray;
    for (const auto &edge : this->graphPlane->graph->edges) {
        QJsonObject edgeJson;
        edgeJson["from"] = edge->from->id;
        edgeJson["to"] = edge->to->id;
        edgeJson["weight"] = edge->weight; // Если есть флаг ориентированности
        edgeJson["name"] = QString::fromStdString(edge->GetName());
        edgesArray.append(edgeJson);
    }
    graphJson["edges"] = edgesArray;

    // Конвертируем JSON-объект в строку
    QJsonDocument jsonDoc(graphJson);

    // Открываем файл для записи
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(nullptr, "Error", "Cannot open file for writing!");
        return;
    }

    // Записываем JSON в файл
    file.write(jsonDoc.toJson(QJsonDocument::Indented)); // Читаемый формат
    file.close();

    QMessageBox::information(nullptr, "Save Graph", "Graph saved successfully!");
}

void MainWindow::loadGraphFromJson() {
    // Открываем диалог для выбора файла
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        "Load Graph",              // Заголовок окна
        "",                        // Начальная папка
        "JSON Files (*.json)"      // Фильтр типов файлов
        );

    // Проверяем, был ли файл выбран
    if (fileName.isEmpty()) {
        QMessageBox::information(nullptr, "Load Graph", "No file selected!");
        return;
    }

    // Открываем файл для чтения
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Error", "Cannot open file for reading!");
        return;
    }

    // Считываем содержимое файла
    QByteArray data = file.readAll();
    file.close();

    // Парсим JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        QMessageBox::critical(nullptr, "Error", "Invalid JSON format!");
        return;
    }

    QJsonObject graphJson = jsonDoc.object();

    // this->graphPlane->ClearGraph();
    Graph* g = new Graph();

    // Десериализация вершин
    QJsonArray vertexesArray = graphJson["vertexes"].toArray();
    QMap<int, GraphVertex*> idToVertexMap;
    for (const QJsonValue &value : vertexesArray) {
        QJsonObject vertexJson = value.toObject();
        int id = vertexJson["id"].toInt();
        qreal x = vertexJson["x"].toDouble();
        qreal y = vertexJson["y"].toDouble();
        qreal radius = vertexJson["radius"].toDouble();
        QString name = vertexJson["name"].toString();

        // Создаём вершину
        GraphVertex* vertex = new GraphVertex(x, y, id, radius);
        vertex->SetName(name.toStdString());
        idToVertexMap[id] = vertex;
        g->addVertex(vertex);
    }

    // Десериализация рёбер
    QJsonArray edgesArray = graphJson["edges"].toArray();
    for (const QJsonValue &value : edgesArray) {
        QJsonObject edgeJson = value.toObject();
        int startVertexId = edgeJson["from"].toInt();
        int endVertexId = edgeJson["to"].toInt();
        double weight = edgeJson["weight"].toDouble();
        QString name = edgeJson["name"].toString();

        // Получаем ссылки на начальную и конечную вершины
        GraphVertex* startVertex = idToVertexMap.value(startVertexId, nullptr);
        GraphVertex* endVertex = idToVertexMap.value(endVertexId, nullptr);

        if (startVertex && endVertex) {
            // Создаём ребро
            GraphEdge* edge = new GraphEdge(std::make_shared<GraphVertex>(startVertex), std::make_shared<GraphVertex>(endVertex));
            edge->SetName(name.toStdString());
            edge->SetWeight(weight);
            g->addEdge(edge);
        } else {
            qWarning() << "Edge references invalid vertex IDs:" << startVertexId << "->" << endVertexId;
        }
    }

    this->graphPlane->SetGraph(g);
    QMessageBox::information(nullptr, "Load Graph", "Graph loaded successfully!");
}

void MainWindow::on_new_graph_triggered()
{
    this->graphPlane->ClearGraph();
}


void MainWindow::on_CalcAdjacencyMatrix_clicked()
{
    if (!this->matrixWindow) {
        this->matrixWindow = new AdjacencyMatrix(this);
    }
    this->matrixWindow->show();
    this->graphPlane = this->findChild<GraphPlane*>("graphicsView");
    this->matrixWindow->updateMatrix(this->graphPlane->graph);
    QObject::connect(this->graphPlane, &GraphPlane::graphChanged, matrixWindow, &AdjacencyMatrix::updateMatrix);

     // this->graphPlane->displayAdjacencyMatrix();
}

