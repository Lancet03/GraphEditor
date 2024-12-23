#include "adjacencymatrix.h"
#include "ui_adjacencymatrix.h"

AdjacencyMatrix::AdjacencyMatrix(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AdjacencyMatrix)
{
    ui->setupUi(this);
}

AdjacencyMatrix::~AdjacencyMatrix()
{
    delete ui;
}

void AdjacencyMatrix::updateMatrix(Graph* graph) {
    this->table = this->findChild<QTableWidget*>("MatrixTable");

    auto matrix = graph->BuildCorrespMatrix();

    int vertexCount = graph->vertexes.size();
    table->setRowCount(vertexCount);
    table->setColumnCount(vertexCount);

    QStringList headers;
    for (const auto& vertex : graph->vertexes) {
        headers << QString::fromStdString(vertex->GetName());
    }
    table->setHorizontalHeaderLabels(headers);
    table->setVerticalHeaderLabels(headers);

    for (int i = 0; i < vertexCount; ++i) {
        for (int j = 0; j < vertexCount; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(matrix[i][j]));
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, j, item);
        }
    }

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
