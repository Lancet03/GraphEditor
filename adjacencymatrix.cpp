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
    // Создаём таблицу
    this->table = this->findChild<QTableWidget*>("MatrixTable");

    // Рассчитываем матрицу смежности
    // auto matrix = calculateAdjacencyMatrix();
    auto matrix = graph->BuildCorrespMatrix();

    // Устанавливаем размеры таблицы
    int vertexCount = graph->vertexes.size();
    table->setRowCount(vertexCount);
    table->setColumnCount(vertexCount);

    // Настраиваем заголовки строк и столбцов
    QStringList headers;
    for (const auto& vertex : graph->vertexes) {
        headers << QString::fromStdString(vertex->GetName());
    }
    table->setHorizontalHeaderLabels(headers);
    table->setVerticalHeaderLabels(headers);

    // Заполняем таблицу данными
    for (int i = 0; i < vertexCount; ++i) {
        for (int j = 0; j < vertexCount; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(matrix[i][j]));
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, j, item);
        }
    }

    // Настраиваем таблицу
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
