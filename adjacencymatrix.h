#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H

#include <QDialog>
#include <QTableWidget>
#include <QTableView>
#include "Graph.h"

namespace Ui {
class AdjacencyMatrix;
}

class AdjacencyMatrix : public QDialog
{
    Q_OBJECT

public:
    explicit AdjacencyMatrix(QWidget *parent = nullptr);
    ~AdjacencyMatrix();

    void updateMatrix(Graph* graph);

private:
    Ui::AdjacencyMatrix *ui;
    QTableWidget* table; // Таблица для отображения матрицы
};

#endif // ADJACENCYMATRIX_H
