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
#include <QRegularExpression>
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

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(addVertexesModeBtn, 1);
    buttonGroup->addButton(addEdgesModeBtn, 2);
    buttonGroup->addButton(viewModeBtn, 3);
    buttonGroup->addButton(editModeBtn, 4);
    buttonGroup->addButton(moveModeBtn, 5);


    addVertexesModeBtn->setCheckable(true);
    addEdgesModeBtn->setCheckable(true);
    viewModeBtn->setCheckable(true);
    editModeBtn->setCheckable(true);
    moveModeBtn->setCheckable(true);

    viewModeBtn->setChecked(true);

    mousePosLabel = new QLabel("Cursor: (0, 0)", this);
    zoomLevelLabel = new QLabel("Zoom: 1.00x", this);

    statusBar()->addWidget(mousePosLabel);
    statusBar()->addPermanentWidget(zoomLevelLabel);


    connect(graphPlane, &GraphPlane::mousePositionChanged, this, &MainWindow::updateMousePosition);
    connect(graphPlane, &GraphPlane::zoomLevelChanged, this, &MainWindow::updateZoomLevel);

    connect(graphPlane, &GraphPlane::mouseEntered, this, &MainWindow::onMouseEntered);
    connect(graphPlane, &GraphPlane::mouseLeft, this, &MainWindow::onMouseLeft);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateMousePosition(const QPointF &pos)
{
    mousePosLabel->setText(QString("Cursor: (%1, %2)").arg(pos.x(), 0, 'f', 2).arg(-pos.y(), 0, 'f', 2));
}

void MainWindow::updateZoomLevel(qreal scale)
{
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

void MainWindow::on_new_graph_triggered()
{
    this->graphPlane->ClearGraph();
}

void MainWindow::on_GenerateStateMachine_clicked()
{
    this->generateStateMachine();
}

void MainWindow::on_CalcAdjacencyMatrix_clicked()
{
    this->matrixWindow = new AdjacencyMatrix(this);
    this->matrixWindow->show();
    this->graphPlane = this->findChild<GraphPlane*>("graphicsView");
    this->matrixWindow->updateMatrix(this->graphPlane->graph);
    QObject::connect(this->graphPlane, &GraphPlane::graphChanged, matrixWindow, &AdjacencyMatrix::updateMatrix);
}

void MainWindow::saveGraph() {
    QString fileName = QFileDialog::getSaveFileName(
        nullptr,
        "Сохранить граф",
        "",
        "JSON Files (*.json)"
        );

    if (fileName.isEmpty()) {
        QMessageBox::information(nullptr, "Save Graph", "Файл не выбран!");
        return;
    }

    if (!fileName.endsWith(".json", Qt::CaseInsensitive)) {
        fileName += ".json";
    }

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

    QJsonDocument jsonDoc(graphJson);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(nullptr, "Error", "Cannot open file for writing!");
        return;
    }

    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();

    QMessageBox::information(nullptr, "Save Graph", "Graph saved successfully!");
}

void MainWindow::loadGraphFromJson() {
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        "Load Graph",
        "",
        "JSON Files (*.json)"
        );

    if (fileName.isEmpty()) {
        QMessageBox::information(nullptr, "Load Graph", "No file selected!");
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Error", "Cannot open file for reading!");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        QMessageBox::critical(nullptr, "Error", "Invalid JSON format!");
        return;
    }

    QJsonObject graphJson = jsonDoc.object();

    Graph* g = new Graph();

    QJsonArray vertexesArray = graphJson["vertexes"].toArray();
    QMap<int, GraphVertex*> idToVertexMap;
    for (const QJsonValue &value : vertexesArray) {
        QJsonObject vertexJson = value.toObject();
        int id = vertexJson["id"].toInt();
        qreal x = vertexJson["x"].toDouble();
        qreal y = vertexJson["y"].toDouble();
        qreal radius = vertexJson["radius"].toDouble();
        QString name = vertexJson["name"].toString();

        GraphVertex* vertex = new GraphVertex(x, y, id, radius);
        vertex->SetName(name.toStdString());
        idToVertexMap[id] = vertex;
        g->addVertex(vertex);
    }

    QJsonArray edgesArray = graphJson["edges"].toArray();
    for (const QJsonValue &value : edgesArray) {
        QJsonObject edgeJson = value.toObject();
        int startVertexId = edgeJson["from"].toInt();
        int endVertexId = edgeJson["to"].toInt();
        double weight = edgeJson["weight"].toDouble();
        QString name = edgeJson["name"].toString();

        GraphVertex* startVertex = idToVertexMap.value(startVertexId, nullptr);
        GraphVertex* endVertex = idToVertexMap.value(endVertexId, nullptr);

        if (startVertex && endVertex) {
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

void MainWindow::generateStateMachine() {
    QString targetFilePath = QFileDialog::getSaveFileName(
        nullptr, "Save State Machine", "", "C++ Files (*.cpp)");

    if (targetFilePath.isEmpty()) {
        return;
    }

    QFile file(targetFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << targetFilePath;
        return;
    }

    QTextStream out(&file);

    this->generateCppCode(out);

    file.close();
}

void MainWindow::generateCppCode(QTextStream& out) {
    QStringList stateNames;

    std::vector<std::shared_ptr<GraphVertex>> vertexes = this->graphPlane->graph->vertexes;
    for (const auto& vertex : vertexes) {
        stateNames << sanitizeName(vertex->GetName());
    }


    QStringList eventNames;
    std::vector<std::shared_ptr<GraphEdge>> edges = this->graphPlane->graph->edges;
    for (const auto& edge : edges) {
        eventNames << sanitizeName(edge->GetName());
    }

    out << "#include <iostream>\n";
    out << "#include <variant>\n";
    out << "#include <stdexcept>\n\n";

    out << "namespace state {\n";
    for (const auto& st : stateNames) {
        out << "    struct " << st << " {\n";
        out << QString::fromUtf8("        // Добавьте поля состояния при необходимости\n");
        out << "        void PrintState() const {\n";
        out << "            std::cout << \"State: " << st << "\" << std::endl;\n";
        out << "        }\n";
        out << "    };\n\n";
    }
    out << "}\n\n";

    out << "namespace event {\n";
    for (const auto& ev : eventNames) {
        out << "    struct " << ev << " {\n";
        out << QString::fromUtf8("        // Добавьте поля события при необходимости\n");
        out << "    };\n\n";
    }
    out << "}\n\n";

    out << "using StateVariant = std::variant";
    out << "<";
    for (int i = 0; i < stateNames.size(); ++i) {
        out << "state::" << stateNames[i];
        if (i < stateNames.size() - 1) out << ", ";
    }
    out << ">;\n\n";

    out << "using EventVariant = std::variant";
    out << "<";
    for (int i = 0; i < eventNames.size(); ++i) {
        out << "event::" << eventNames[i];
        if (i < eventNames.size() - 1) out << ", ";
    }
    out << ">;\n\n";

    out << "template<typename State, typename Event>\n";
    out << "StateVariant onEvent(const State&, const Event&) {\n";
    out << "    throw std::logic_error{\"Unsupported state transition\"};\n";
    out << "}\n\n";

    for (const auto& edge : edges) {
        QString startState = sanitizeName(edge->from->GetName());
        QString endState = sanitizeName(edge->to->GetName());
        QString eventName = sanitizeName(edge->GetName());

        out << "StateVariant onEvent(const state::" << startState << "&, const event::" << eventName << "&) {\n";
        out << "    std::cout << \"Transition: " << startState << " -> " << endState << " on " << eventName << "\" << std::endl;\n";
        out << "    return state::" << endState << "{};\n";
        out << "}\n\n";
    }

    out << "auto StateReporter = [](const auto& s){ s.PrintState(); };\n";
    out << "auto EventProcessor = [](const auto& st, const auto& ev) { return onEvent(st, ev); };\n\n";

    // Генерируем класс StateMachine
    out << "class StateMachine {\n";
    out << "private:\n";
    out << "    StateVariant state_;\n";
    out << "public:\n";
    out << "    void startMachine() {\n";
    if (!stateNames.isEmpty()) {
        out << "        state_ = state::" << stateNames.first() << "{};\n";
    } else {
        out << "        // Нет состояний\n";
    }
    out << "    }\n\n";

    out << "    void processEvent(const EventVariant& event) {\n";
    out << "        state_ = std::visit(EventProcessor, state_, event);\n";
    out << "    }\n\n";

    out << "    void reportCurrentState() {\n";
    out << "        std::visit(StateReporter, state_);\n";
    out << "    }\n";
    out << "};\n\n";

    out << "int main() {\n";
    out << "    StateMachine sm;\n";
    out << "    sm.startMachine();\n";
    out << "    sm.reportCurrentState();\n\n";
    if (!edges.empty()) {
        out << "    // Example usage:\n";
        out << "    try {";
        for (const auto& edge : edges) {
            QString eventName = sanitizeName(edge->GetName());
            out << "    sm.processEvent(event::" << eventName << "{});\n";
            out << "    sm.reportCurrentState();\n";
        }
        out << "    }\n";
        out << "    catch(std::exception &ex) {\n";
        out << "        std::cout << \"Exception: \" << ex.what() << std::endl;\n";
        out << "    }\n";
    }
    out << "    return 0;\n";
    out << "}\n";
}

QString MainWindow::sanitizeName(std::string name) {
    QString s = QString::fromStdString(name);
    s.replace(" ", "_");
    s.remove(QRegularExpression("[^a-zA-Zа-яА-я0-9_]"));
    return s;
}

