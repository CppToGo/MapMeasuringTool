#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    static HttpServer &instance();
    explicit HttpServer(const QHostAddress &address = QHostAddress::Any, const quint16 &port = 8080 , QString WebSoketAddr = "127.0.0.1:12345", QObject *parent = nullptr);
    ~HttpServer();
signals:

public slots:

private slots:
    void newConnection();
    void readyRead();
private:

    Q_DISABLE_COPY(HttpServer)
private:
    QTcpServer *m_httpServer;
    QByteArray m_client;
};

#endif // HTTPSERVER_H
