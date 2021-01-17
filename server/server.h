#ifndef SERVER_H
#define SERVER_H

#include "threadpool.h"

#include <QTcpServer>

class ConnectionHandlerFactory;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(ConnectionHandlerFactory* factoy, QObject *parent = nullptr);
    ~Server() override;

    bool open(quint16 port);

Q_SIGNALS:

protected:
    void incomingConnection(qintptr handle) override;

private:
    ThreadPool threadPool;
    ConnectionHandlerFactory* connectionHandlerFactory;
};

#endif // SERVER_H
