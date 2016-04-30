#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    ui->scene->buildSimpleTriangular();
}

void MainWindow::on_clearButton_clicked() {
    ui->scene->clear();
}

void MainWindow::on_convertToDelaunayTriangular_clicked() {
    ui->scene->convertToDelaunayTriangular();
}

void MainWindow::on_convexHull_clicked(){
    ui->scene->showConvexHull();
}

void MainWindow::on_generatePoints_clicked(){
    ui->scene->generatePoints();
}
