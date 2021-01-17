#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <QObject>
#include <QThread>

class ThreadPool : public QObject
{
    Q_OBJECT
public:
    explicit ThreadPool(int maxNumThreads = 10, QObject *parent = nullptr);
    ~ThreadPool() override;

    QThread *get();

Q_SIGNALS:
    // expose a signal to force store to be done on the object's thread
    void store(QThread* t);

private:
    void storeThread(QThread* t);

    int const maxNumberOfThreads;
    QList<QThread*> runningThreads;
    QList<QThread*> storedThreads;
};

#endif // THREADPOOL_H
