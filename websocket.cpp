#include "websocket.h"

websocket::websocket(QHostAddress hostAddress, uint16_t port)
    :Server(new QWebSocketServer(QStringLiteral("Super Messure Tool Server"), QWebSocketServer::NonSecureMode))
{
    if(this->Server->listen(hostAddress, port)){
        qDebug() << "start websocket listen :" << port;

//        qDebug()<<Server->isListening();
//        qDebug()<<Server->serverPort();
//        qDebug()<<Server->serverAddress();
        QObject::connect(Server, &QWebSocketServer::newConnection, this, &websocket::onNewConnection);
    }else {
        throw std::runtime_error("SMTServer: failed to listen");
    }
}

void websocket::onNewConnection(){
    qDebug() << "SMTServer::onNewConnection";
    QWebSocket* socket(this->Server->nextPendingConnection());

    QObject::connect(socket, SIGNAL(textMessageReceived(QString)), this, SLOT(processTextMessage(QString)));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    socket->sendTextMessage("链接服务器成功……");
    this->clients.append(socket);
}

void websocket::processTextMessage(QString message){
    qDebug() << "SMTServer:: processTextMessage: " << message;
}

void websocket::socketDisconnected(){
    qDebug() << "SMTServer:: socketDisConnected" ;
}

void websocket::SendToClientMessage(QString data){
    if(! this->clients.isEmpty()){
        foreach (QWebSocket* i , this->clients){
            if(i->isValid()){
                i->sendTextMessage(data);
                i->flush();
            }
        }
    }
}

