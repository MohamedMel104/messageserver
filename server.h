
#ifndef SERVER_H
#define SERVER_H

#include <QDebug>
#include <iostream>
#include <QObject>
#include <QVector>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>


class Server : public QObject
{
public:
    explicit Server(QObject* parent = nullptr);
    void startServer();
    void sendMessage(QString message);
private:
    QTcpServer* server;
    QVector<QTcpSocket*>* allClients;
private slots:
    void newClientConnect();
    void socketReadyRead();
    void socketDisconnected();
    void socketStateChanged(QAbstractSocket::SocketState state);
};

#endif // SERVER_H
