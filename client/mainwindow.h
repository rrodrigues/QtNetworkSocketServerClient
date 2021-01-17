#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>

namespace Ui {
class MainWindow;
}
class QTcpSocket;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void connectToServer();
    void disconnectFromServer();

private:
    void clear();
    void send();
    void appendReply();

    Ui::MainWindow *ui;
    QTcpSocket* client = nullptr;
};

#endif // MAINWINDOW_H
