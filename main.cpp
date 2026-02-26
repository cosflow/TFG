#include "mainwindow.h"
#include "server.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Server *tcpServer = new Server();
    QObject::connect(tcpServer, &Server::dataReceived, &w, &MainWindow::moveCursor);
    w.show();
    return a.exec();
}
