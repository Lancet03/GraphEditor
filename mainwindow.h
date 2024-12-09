#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QLabel>

#include "GraphPlane.h"
#include "adjacencymatrix.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateMousePosition(const QPointF &pos); // Слот для обновления координат
    void updateZoomLevel(qreal scale);           // Слот для обновления уровня зума
    void onMouseEntered(); // Обработка входа курсора в область
    void onMouseLeft();    // Обработка выхода курсора из области

    void on_ViewMode_toggled(bool checked);

    void on_AddVertexes_toggled(bool checked);

    void on_AddEdges_toggled(bool checked);

    void on_EditMode_toggled(bool checked);

    void on_MoveMode_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    AdjacencyMatrix *matrixWindow;
    GraphPlane *graphPlane; // Виджет с координатной плоскостью
    QLabel *mousePosLabel;  // Метка для координат
    QLabel *zoomLevelLabel; // Метка для уровня зума
};
#endif // MAINWINDOW_H
