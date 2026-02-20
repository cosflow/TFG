#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QLabel>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QPixmap tuxPixmap(":/images/img.png");
    ui->tuxLabel->setPixmap(tuxPixmap.scaled(100,100,Qt::KeepAspectRatio));
    ui->tuxLabel->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
