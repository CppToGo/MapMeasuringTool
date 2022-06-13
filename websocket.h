#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QWebSocketServer>
#include <QWebSocket>
#include <QObject>
#include <iostream>
#include <memory>

class websocket;

class websocket: public QObject
{
    Q_OBJECT

    QWebSocketServer* Server;
    QList<QWebSocket*> clients;

public:
    websocket(uint16_t port);
    void SendToClientMessage(QString data);

signals:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();
};

#endif // WEBSOCKET_H
