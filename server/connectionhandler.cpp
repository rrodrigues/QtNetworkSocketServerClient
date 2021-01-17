#include "connectionhandler.h"

#include <QThread>
#include <QTcpSocket>
#include <QProcess>


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


ConnectionHandler *BashProxyConnectionHandler::Factory::create(QTcpSocket *s)
{
    return new BashProxyConnectionHandler(s);
}

BashProxyConnectionHandler::BashProxyConnectionHandler(QTcpSocket* s, QObject* parent)
    : ConnectionHandler(s, parent)
{
    process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [](int exitCode, QProcess::ExitStatus exitStatus){ qDebug() << "finished" << exitCode << exitStatus; });

    connect(process, &QProcess::errorOccurred, [this](auto e) { qDebug() << "error" << e; });
    connect(process, &QProcess::readyReadStandardOutput, [this]() {  socket->write(process->readAllStandardOutput()); });
    connect(process, &QProcess::readyReadStandardError, [this]() { socket->write(process->readAllStandardError()); });
    connect(socket, &QProcess::readyRead, [this](){
        auto all = socket->readAll();
        socket->write("> ");
        socket->write(all);
        socket->write("\n");

        process->write(all);
        process->write("\n");
    });
    process->start("bash", QStringList());
}
