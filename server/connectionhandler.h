#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QObject>

class QTcpSocket;

class ConnectionHandler;
class ConnectionHandlerFactory : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionHandlerFactory(QObject *parent = nullptr);

    ConnectionHandler* create(QTcpSocket* s);

};

class ConnectionHandler : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionHandler(QTcpSocket* s, QObject *parent = nullptr);


protected:
    QTcpSocket* socket = nullptr;

};


class EchoConnectionHandler : public ConnectionHandler
{
    Q_OBJECT
public:
    explicit EchoConnectionHandler(QTcpSocket* s, QObject *parent = nullptr);

protected:
    void echo();

};

#endif // CONNECTIONHANDLER_H
