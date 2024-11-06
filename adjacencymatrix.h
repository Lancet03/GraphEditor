#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H

#include <QDialog>

namespace Ui {
class AdjacencyMatrix;
}

class AdjacencyMatrix : public QDialog
{
    Q_OBJECT

public:
    explicit AdjacencyMatrix(QWidget *parent = nullptr);
    ~AdjacencyMatrix();

private:
    Ui::AdjacencyMatrix *ui;
};

#endif // ADJACENCYMATRIX_H
