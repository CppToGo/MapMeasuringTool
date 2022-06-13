#include "httpserver.h"

HttpServer &HttpServer::instance(){
    static HttpServer obj;
    return obj;
}


void HttpServer::newConnection(){
    qDebug()<< "httpSercer newConnection";
    QTcpSocket* m_socket = m_httpServer->nextPendingConnection();
    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &HttpServer::readyRead);
}

void HttpServer::readyRead(){
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket){
//        QByteArray request = socket->readAll();

//        qDebug() << "Request Data:" << request;

        static int count = 0;
        count++;
        QByteArray response = m_client;


        QString http = "HTTP/1.1 200 OK\r\n";
        http += "Server: nginx\r\n";
        http += "Content-Type: text/html;charset=utf-8\r\n";
        http += "Connection: keep-alive\r\n";
        http += QString("Content-Length: %1\r\n\r\n").arg(QString::number(response.size()));

        socket->write(http.toUtf8());
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten(http.size() + response.size());
        socket->close();
    }
}
HttpServer::HttpServer(const QHostAddress &address, const quint16 &port, QString WebSoketAddr, QObject *parent) : QObject(parent)
{
    m_httpServer = new QTcpServer(this);
    m_httpServer->setMaxPendingConnections(128);//设置最大允许连接数
    QObject::connect(m_httpServer,&QTcpServer::newConnection,this,&HttpServer::newConnection);

    QFile file(QCoreApplication::applicationDirPath() +"/websocketClient/SMTWebHUDClient/index.html");
    qDebug()<< QCoreApplication::applicationDirPath() +"/websocketClient/SMTWebHUDClient/index.html";
    if(file.open(QIODevice::ReadOnly)){
        m_client = file.readAll().replace(QString("xxxxdddd").toUtf8(), WebSoketAddr.toUtf8());
        m_httpServer->listen(address, port);
        file.close();
    }else{
         throw std::runtime_error("SMThttpServer: failed to get client.html");
    }

}

HttpServer::~HttpServer()
{

}
