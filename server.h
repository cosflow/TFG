#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QTcpSocket>
#define LISTEN_PORT 45454

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    qint32 ArrayToInt(QByteArray source);

private:
    QTcpServer * coordServer;
    QByteArray *buffer; //We need a buffer to store data until block has completely received
    qint32 s = 0;
    QTcpSocket * client;

signals:
    void dataReceived(QByteArray);

private slots:
    void newConnection();
    void disconnected();
    void readyRead();
};

#endif // SERVER_H
