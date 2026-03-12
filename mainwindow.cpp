#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include <bitset>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QPixmap tuxPxm(":/images/img.png");
    ui->tuxLbl->setPixmap(tuxPxm.scaled(100,100,Qt::KeepAspectRatio));
    ui->tuxLbl->setFrameStyle(QFrame::Box);
    generateCollisionLabels();
    ui->tuxLbl->show();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::generateCollisionLabels() {
    for(int i = 0 ; i < 4 ; i++){
        QLabel *l = new QLabel();
        l->setObjectName("collisionLbl"+QString::number(i));
        l->setText("Collision");
        l->setFrameStyle(QFrame::Box);
        l->setAlignment(Qt::AlignCenter);
        std::bitset<2> temp = i;
        ui->grid->addWidget(l,temp[0]*2,temp[1]*2);
        colLbls.append(l);
    }
}

void MainWindow::moveCursor(QByteArray data) {
    QList<QByteArray> vector = data.split('/');
    ui->tuxLbl->move(vector[0].toInt(), vector[1].toInt());
    QRect tuxRect = ui->tuxLbl->geometry();
    for(int i=0;i<colLbls.size();i++) {
        if(tuxRect.intersects(colLbls[i]->geometry())) {
            emit collisionsDetected(i);
        }
    }
}
