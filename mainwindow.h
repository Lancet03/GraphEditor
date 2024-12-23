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
    void updateMousePosition(const QPointF &pos);
    void updateZoomLevel(qreal scale);
    void onMouseEntered();
    void onMouseLeft();

    void on_ViewMode_toggled(bool checked);

    void on_AddVertexes_toggled(bool checked);

    void on_AddEdges_toggled(bool checked);

    void on_EditMode_toggled(bool checked);

    void on_MoveMode_toggled(bool checked);

    void on_save_triggered();

    void on_open_triggered();

    void on_new_graph_triggered();

    void on_CalcAdjacencyMatrix_clicked();

    void on_GenerateStateMachine_clicked();

private:
    Ui::MainWindow *ui;
    AdjacencyMatrix *matrixWindow;
    GraphPlane *graphPlane;
    QLabel *mousePosLabel;
    QLabel *zoomLevelLabel;

    QString sanitizeName(std::string name);
    void generateStateMachine();
    void generateCppCode(QTextStream& out);
    void saveGraph();
    void loadGraphFromJson();
};
#endif // MAINWINDOW_H
