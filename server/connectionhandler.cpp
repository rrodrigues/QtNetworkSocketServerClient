#include "connectionhandler.h"

#include <QThread>
#include <QTcpSocket>


ConnectionHandlerFactory::ConnectionHandlerFactory(QObject *parent)
    : QObject(parent)
{
}

ConnectionHandler *ConnectionHandlerFactory::create(QTcpSocket* s)
{
    return new EchoConnectionHandler(s);
}


ConnectionHandler::ConnectionHandler(QTcpSocket *s, QObject *parent)
    : QObject(parent)
    , socket(s)
{
}


EchoConnectionHandler::EchoConnectionHandler(QTcpSocket *s, QObject *parent)
    : ConnectionHandler(s, parent)
{
    connect(socket, &QTcpSocket::readyRead, this, &EchoConnectionHandler::echo);
}

void EchoConnectionHandler::echo()
{
    if (socket == nullptr) return;

    auto all = socket->readAll();
    qDebug() << "reading from" << socket->bytesAvailable() << all << thread();
    socket->write("> " + all);
}
