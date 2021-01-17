#include "server.h"
#include "connectionhandler.h"

#include <QCoreApplication>
#include <QDebug>
#include <QTcpSocket>
#include <QThread>

bool isMainThread() {
    return QThread::currentThread() == QCoreApplication::instance()->thread();
}

Server::Server(ConnectionHandlerFactory* factoy, QObject *parent)
    : QTcpServer(parent)
    , threadPool(20)
    , connectionHandlerFactory(factoy)
{
    setMaxPendingConnections(1);
}

Server::~Server()
{
    qDebug() << "Server dtor...";
}

bool Server::open(quint16 port)
{
    if (!listen(QHostAddress::Any, port)) {
        return false;
    }

    qDebug() << "listen at port:" << serverPort();
    return true;
}

void Server::incomingConnection(qintptr handle)
{
    auto socket = new QTcpSocket;
    if (!socket->setSocketDescriptor(handle)) {
        qDebug() << "Unable to create connection at port:" << serverPort() << ". error: " << socket->errorString() << QThread::currentThread() << isMainThread();
        acceptError(socket->error());
        socket->disconnectFromHost();
        socket->deleteLater();
        return;
    }

    auto t = threadPool.get();
    if (t == nullptr) {
        qWarning() << "Unable to get thread to run connection:" << QThread::currentThread() << isMainThread();
        acceptError(QAbstractSocket::ConnectionRefusedError);
        socket->disconnectFromHost();
        socket->deleteLater();
        return;
    }

    qDebug() << "received connection at port:" << serverPort() << "starting thread" << t;

    socket->moveToThread(t);
    connect(socket, &QTcpSocket::disconnected, std::bind(&ThreadPool::store, &threadPool, t));
    connect(socket, &QTcpSocket::disconnected, [](){ qDebug() << "connection closed..." << QThread::currentThread() << isMainThread(); });
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    auto connectionHandler = connectionHandlerFactory->create(socket);
    connectionHandler->moveToThread(t);
}
