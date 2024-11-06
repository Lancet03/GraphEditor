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
