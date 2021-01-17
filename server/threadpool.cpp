#include "threadpool.h"

#include <QDebug>

ThreadPool::ThreadPool(int maxNumThreads, QObject *parent)
    : QObject(parent)
    , maxNumberOfThreads(maxNumThreads)
{
    connect(this, &ThreadPool::store, this, &ThreadPool::storeThread);
}

ThreadPool::~ThreadPool()
{
    qDebug() << "quiting... storedThreads:" << storedThreads.size() << "running:" << runningThreads.size() << "total:" << maxNumberOfThreads << "current:" << thread();

    auto quit = [](QThread* t) { t->quit(); t->wait(); };
    for (auto t : qAsConst(runningThreads)) { quit(t); }
    for (auto t : qAsConst(storedThreads)) { quit(t); }
}

QThread *ThreadPool::get()
{
    if (QThread::currentThread() != thread()) {
        qWarning() << "WRONG THREAD: QThread::currentThread() != thread()" << thread();
        return nullptr;
    }

    if (runningThreads.size() == maxNumberOfThreads) {
        qWarning() << "Max number of threads in use.." << maxNumberOfThreads << "running threads..." << thread();
        return nullptr;
    }

    QThread * out;
    if (storedThreads.empty()) {
        out = new QThread(this);
        out->start();
    } else {
        out = storedThreads.back();
        storedThreads.pop_back();
    }

    runningThreads.append(out);

    qDebug() << "get thread. storedThreads:" << storedThreads.size() << "running:" << runningThreads.size() << "total:" << maxNumberOfThreads
             << "new:" << out << "current:" << thread();

    return out;
}

void ThreadPool::storeThread(QThread *t)
{
    if (QThread::currentThread() != thread()) {
        qWarning() << "WRONG THREAD: QThread::currentThread() != thread()" << thread();
    }

    runningThreads.removeOne(t);
    storedThreads.push_back(t);

    qDebug() << "store thread. storedThreads:" << storedThreads.size() << "running:" << runningThreads.size() << "total:" << maxNumberOfThreads
             << "stored:" << t << "current:" << thread();
}
