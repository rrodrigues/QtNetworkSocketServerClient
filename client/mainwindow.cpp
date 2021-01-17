#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QDebug>
#include <QIntValidator>
#include <QShortcut>
#include <QStatusBar>
#include <QTcpSocket>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::disconnectFromServer);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::send);
    connect(ui->clearButton, &QPushButton::clicked, ui->inputTextEdit, &QPlainTextEdit::clear);
    auto sendShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return), ui->inputTextEdit, nullptr, nullptr, Qt::WidgetShortcut);
    connect(sendShortcut, &QShortcut::activated, this, &MainWindow::send);

    ui->portLineEdit->setValidator(new QIntValidator(0, std::numeric_limits<unsigned short>::max()));
    statusBar()->showMessage("running...");

    connectToServer();
}

MainWindow::~MainWindow()
{
    disconnectFromServer();
    delete ui;
}

void MainWindow::connectToServer()
{
    disconnectFromServer();

    QString host = ui->hostLineEdit->text();
    quint16 port = ui->portLineEdit->text().toUShort();

    statusBar()->showMessage(QString("connecting to %1:%2 ...").arg(host).arg(port));

    client = new QTcpSocket;
    connect(client, &QTcpSocket::errorOccurred, [this]() { statusBar()->showMessage(client->errorString()); });
    connect(client, &QTcpSocket::readyRead, [this]() { ui->replyTextEdit->setPlainText(client->readAll()); });
    connect(client, &QTcpSocket::connected, [this, host, port]() {
        statusBar()->showMessage(QString("connected to %1:%2 ...").arg(host).arg(port));
    });
    client->connectToHost(host, port);
}

void MainWindow::disconnectFromServer()
{
    if (client) {
        client->disconnectFromHost();
        connect(client, &QTcpSocket::disconnected, client, &QObject::deleteLater);
        client = nullptr;
        statusBar()->showMessage("disconnected...");
    }
}

void MainWindow::send()
{
    if (client == nullptr) return;

    auto data = ui->inputTextEdit->toPlainText().toUtf8();
    ui->inputTextEdit->clear();

    client->write(data);
}
