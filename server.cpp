
#include "server.h"

Server::Server(QObject* parent) : QObject(parent)
{

}
void Server::startServer(){
    server = new QTcpServer();
    allClients = new QVector<QTcpSocket*>;
    server->setMaxPendingConnections(10);
    QObject::connect(server, &QTcpServer::newConnection, this, &Server::newClientConnect);
    if(server->listen(QHostAddress::Any, 8001)){
        qDebug()<<"Server has started! Listening at 8001.";

    }
    else {
        qDebug()<<"Server failed to start! "<<server->errorString();
    }
}
void Server::newClientConnect(){ //Revision required here!
    QTcpSocket* client = server->nextPendingConnection();
    QString ipAddress = client->peerAddress().toString();
    int port= client->peerPort();

    /*QObject::connect(client, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));     //connection method needs revision
    QObject::connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    QObject::connect(client, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));*/
    QObject::connect(client, &QTcpSocket::disconnected, this, &Server::socketDisconnected);     //connection method needs revision
    QObject::connect(client, &QTcpSocket::stateChanged, this, &Server::socketStateChanged);
    QObject::connect(client, &QTcpSocket::readyRead, this, &Server::socketReadyRead);
    allClients->push_back(client);
    qDebug()<<"New socket connected! IP address: "+ipAddress+" Port: "+QString::number(port);
}
void Server::socketDisconnected(){
    QTcpSocket* client =qobject_cast<QTcpSocket*>(QObject::sender());
    QString ipAddress = client->peerAddress().toString();
    int port = client->peerPort();

    qDebug()<<"Socket diconnected! IP address: "+ipAddress+" Port: "+QString::number(port);
}
void Server::socketReadyRead(){

    QTcpSocket* client =qobject_cast<QTcpSocket*>(QObject::sender());
    QString ipAddress = client->peerAddress().toString();
    int port = client->peerPort();

    QString data;
    data=client->readAll();
    qDebug()<<"Message: "+data+" (IP address: "+ipAddress+":"+QString::number(port) + ")";
    sendMessage(data);
}
void Server::socketStateChanged(QAbstractSocket::SocketState state){
    QTcpSocket* client =qobject_cast<QTcpSocket*>(QObject::sender());
    QString ipAddress = client->peerAddress().toString();
    int port = client->peerPort();

    QString desc;
    switch(state){
    case  QAbstractSocket::UnconnectedState:
        desc = "The socket is not connected.";
        break;
    case QAbstractSocket::HostLookupState:
        desc = "The socket is performing a host name lookup.";
        break;

    case QAbstractSocket::ConnectingState:
        desc = "The socket is establishing a connection.";
        break;
    case QAbstractSocket::ConnectedState:
        desc = "A connection is established.";
        break;
    case QAbstractSocket::BoundState:
        desc = "The socket is bound to an address and a port.";
        break;
    case QAbstractSocket::ClosingState:
        desc = "The socket is about to close.";
        break;
    case QAbstractSocket::ListeningState:
        desc =  "The socket is listening.";
        break;
    }
    qDebug()<<"Socket State Changed! IP address: "+ipAddress+" Port: "+QString::number(port) + desc;

}
void Server::sendMessage(QString message){
    if(allClients->size()>0){
        for(int i=0; i<allClients->size();i++){
            if(allClients->at(i)->isOpen()&&allClients->at(i)->isWritable()){
                allClients->at(i)->write(message.toUtf8());
            }
        }
    }
}
