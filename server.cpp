#include "server.h"
#include <QDebug>

Server::Server(QObject *parent)
    : QObject{parent}
{
    coordServer = new QTcpServer();
    buffer = new QByteArray();
    coordServer->listen(QHostAddress::Any,LISTEN_PORT);
    qDebug() << "Escuchando en " << coordServer->serverAddress() << ":" << coordServer->serverPort();
    connect(coordServer, SIGNAL(newConnection),SLOT(newConnection));
}

Server::~Server()
{
    coordServer->close();
    coordServer->deleteLater();
}

void Server::newConnection()
{
    if (coordServer->hasPendingConnections()) {
        QTcpSocket *client = coordServer->nextPendingConnection();
        coordServer->pauseAccepting();
        qDebug() << "Cliente conectado desde la IP " << client->peerAddress();
        connect(client, SIGNAL(disconnected),SLOT(disconnected));
        connect(client, SIGNAL(readyRead),SLOT(readyRead));
    }
}

void Server::disconnected()
{
    coordServer->resumeAccepting();
    qDebug() << "Cliente desconectado.";
    client->deleteLater();
    client = nullptr;
    buffer->clear();
}

void Server::readyRead() {
    while (client->bytesAvailable() > 0) {
        buffer->append(client->readAll());
        int indiceFin = buffer->indexOf('\n');
        while (indiceFin != -1) {
            QByteArray coords = buffer->left(indiceFin).trimmed();
            emit dataReceived(coords);
        }
    }
}
