#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QThread>

#include "config.h"
#include "connectionhandler.h"
#include "server.h"

#ifndef Q_OS_WIN

#include <signal.h>
#include <unistd.h>

#endif // Q_OS_WIN

void quitApplicationSignalHandler(int signal)
{
    qDebug() << "Signal [" << signal << "] received. Exiting app now...";
    QCoreApplication::quit();
}

void registerSignalHandler()
{
    // ignore signals
    auto signalToIgnore = {
        SIGPIPE, // ignore broken pipe signal when there is no internet
    };
    for (auto sig: signalToIgnore) {
        signal(sig, SIG_IGN);
    }

    // catch exit signals
    auto signalsToHandleExit = {
        SIGINT,
        SIGQUIT,
        SIGTERM,
        SIGKILL,
    };

    sigset_t blockingMask;
    sigemptyset(&blockingMask);
    for (auto sig : signalsToHandleExit) {
        sigaddset(&blockingMask, sig);
    }

    struct sigaction sa;
    sa.sa_handler = quitApplicationSignalHandler;
    sa.sa_mask    = blockingMask;
    sa.sa_flags   = 0;

    for (auto sig : signalsToHandleExit) {
        sigaction(sig, &sa, nullptr);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationVersion(APP_VERSION_STR);

    QCommandLineOption portOpt({"p", "port"}, "port", "port", "11001");
    QCommandLineOption udpOpt("udp", "use UDP instead of TCP");
    QCommandLineOption typeOpt("type", "Type. values: echo, bash", "type", "echo");
    QCommandLineParser parser;
    parser.addOptions({ portOpt, udpOpt, typeOpt });
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(a);
    auto port = parser.value(portOpt).toUShort();
    auto type = parser.value(typeOpt);

    registerSignalHandler();

    qDebug().noquote()
            << QCoreApplication::applicationName() << QCoreApplication::applicationVersion()
            << "running on" << QThread::currentThreadId() << "port" << port << "type" << type;

    std::unique_ptr<ConnectionHandlerFactory> factory;
    if (type == "bash") {
        factory.reset(new BashProxyConnectionHandler::Factory);
    } else {
        factory.reset(new ConnectionHandlerFactory);
    }

    auto server = Server(factory.get(), &a);
    if (!server.open(port)) {
        qWarning() << "Unable to open server at port" << port << ". exiting...";
        return 1;
    }

    return a.exec();
}
