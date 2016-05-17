#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_clearButton_clicked();

    void on_convertToDelaunayTriangular_clicked();

    void on_convexHull_clicked();

    void on_generatePoints_clicked();

    void on_buildSurfaceButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
