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

void Server::newConnection()
{
    if (coordServer->hasPendingConnections()){
        QTcpSocket *client = coordServer->nextPendingConnection();
        coordServer->pauseAccepting();
        qDebug() << "Cliente conectado desde la IP " << client->peerAddress();
        //connect(client, &QTcpSocket::disconnected, this, &Server::onDisconnect);
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
    s = 0;
}

void Server::readyRead()
{
    int size = s;
    while (client->bytesAvailable() > 0)
    {
        buffer->append(client->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //While can process data, process it
        {
            if (size == 0 && buffer->size() >= 4) //if size of data has received completely, then store it on our global variable
            {
                size = ArrayToInt(buffer->mid(0, 4));
                s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size) // If data has received completely, then emit our SIGNAL with the data
            {
                QByteArray data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                s = size;
                emit dataReceived(data);
            }
        }
    }
}

qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}
