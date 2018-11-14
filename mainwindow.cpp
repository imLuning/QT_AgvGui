#if _MSC_VER >= 1400
#pragma execution_character_set("utf-8")
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpClient = new QTcpSocket(this);
    labSocketState  = new QLabel("socket状态：");
    labSocketState->setMinimumWidth(300);
    ui->statusBar->addWidget(labSocketState);
    QString localIP = getLocalIP();
    this->setWindowTitle(this->windowTitle()+" Local IP "+localIP);
    ui->lineEditTarAddr->setText(localIP);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getLocalIP()
{
    QString hostName = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    QString localIP = "";
    QList<QHostAddress> addList = hostInfo.addresses();

    if(!addList.isEmpty())
    {
        for(int i = 0;i<addList.count();i++)
        {
            QHostAddress aHost = addList.at(i);
            if(QAbstractSocket::IPv4Protocol == aHost.protocol())
            {
                localIP = aHost.toString();
                break;
            }
        }
    }
    return localIP;
}
