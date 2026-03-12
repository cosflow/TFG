#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QObject{parent} {
    coordServer = new QTcpServer();
    buffer = new QByteArray();
}

int Server::start() {
    if(!coordServer->listen(QHostAddress::Any, LISTEN_PORT)){
        qDebug() << "Error al escuchar" << coordServer->errorString();
        coordServer->deleteLater();
        return -1;
    }
    qDebug() << "Servidor escuchando en " << coordServer->serverAddress()
             << ":" << coordServer->serverPort();
    connect(coordServer, SIGNAL(newConnection()),SLOT(newConnection()));
    return 0;
}

Server::~Server() {
    coordServer->close();
    coordServer->deleteLater();
    if (client) client->deleteLater();
}
void Server::newConnection() {
    if (coordServer->hasPendingConnections()) {
        qDebug() << "Hay clientes.";
        client = coordServer->nextPendingConnection();
        coordServer->pauseAccepting();
        qDebug() << "Cliente conectado desde la IP " << client->peerAddress();
        connect(client, SIGNAL(disconnected()),SLOT(disconnected()));
        connect(client, SIGNAL(readyRead()),SLOT(readyRead()));
    }
}

void Server::emitVibration(int vibId) {
    if(!client) return;
    QByteArray block;
    block.setNum(vibId);
    block.append('\n');
    client->write(block);
}

void Server::disconnected() {
    coordServer->resumeAccepting();
    qDebug() << "Cliente desconectado.";
    client = nullptr;
    buffer->clear();
}

void Server::readyRead() {
    if (client->bytesAvailable() > 0) {
        buffer->append(client->readAll());
        int indiceFin = buffer->indexOf('\n');
        if (indiceFin != -1) {
            QByteArray coords = buffer->left(indiceFin).trimmed();
            emit dataReceived(coords);
            buffer->clear();
        }
    }
}
