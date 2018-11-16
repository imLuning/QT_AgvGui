#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLabel>
#include <QHostInfo>
#include <QHostAddress>
#include <QDebug>
#include "stringtohex.h"
#include "getcrc16.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpClient;
    QLabel *labSocketState;
    QString getLocalIP();

private slots:
    void onConnected();
    void onDisconnected();
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
    void onSocketReadyRead();
    void on_actConnect_triggered();
    void on_actDisconnect_triggered();
    void on_actSend_triggered();
    void on_actQuit_triggered();
    void on_btnSetCommand_clicked();
};

#endif // MAINWINDOW_H
