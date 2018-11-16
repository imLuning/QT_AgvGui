#if _MSC_VER >= 1400
#pragma execution_character_set("utf-8")
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stringtohex.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpClient = new QTcpSocket(this);   //创建socket
    labSocketState  = new QLabel("socket状态：");  //添加状态栏信息
    labSocketState->setMinimumWidth(300);
    ui->statusBar->addWidget(labSocketState);
    QString localIP = getLocalIP(); //获取本地IP
    this->setWindowTitle(this->windowTitle()+" Local IP "+localIP); //标题栏显示IP
    ui->lineEditTarAddr->setText(localIP);  //初始化地址栏
    ui->spinBoxTarPort->setValue(8888);   //初始化端口

    ui->comboBoxCommand->addItem("手动",0);
    ui->comboBoxCommand->addItem("自动",1);
    ui->comboBoxCommand->addItem("定点",2);
    ui->comboBoxDirection->addItem("前",2);
    ui->comboBoxDirection->addItem("后",7);
    ui->comboBoxDirection->addItem("左",4);
    ui->comboBoxDirection->addItem("右",5);

    connect(tcpClient,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(tcpClient,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(tcpClient,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
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

void MainWindow::onConnected()
{
    ui->plainTextEditLocalHistory->appendPlainText("[Message]已连接到服务器");
    //ui->plainTextEditLocalHistory->appendPlainText("->peer name: "+tcpClient->peerName());
    ui->plainTextEditLocalHistory->appendPlainText("[Message]"+tcpClient->peerAddress().toString());
    ui->plainTextEditLocalHistory->appendPlainText("[Message]"+QString::number(tcpClient->peerPort()));
    //qDebug()<<tcpClient->peerPort();

    ui->actConnect->setEnabled(false);
    ui->actDisconnect->setEnabled(true);
}

void MainWindow::onDisconnected()
{
    ui->plainTextEditLocalHistory->appendPlainText("[Message]已与服务器断开连接");

    ui->actConnect->setEnabled(true);
    ui->actDisconnect->setEnabled(false);
}

void MainWindow::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    switch(socketState)
    {
        case QAbstractSocket::UnconnectedState:
            {labSocketState->setText("socket state：UnconnectedState");break;}
        case QAbstractSocket::HostLookupState:
            {labSocketState->setText("socket state：HostLookupState");break;}
        case QAbstractSocket::ConnectingState:
            {labSocketState->setText("socket state：ConnectingState");break;}
        case QAbstractSocket::ConnectedState:
            {labSocketState->setText("socket state：ConnectedState");break;}
        case QAbstractSocket::BoundState:
            {labSocketState->setText("socket state：BoundState");break;}
        case QAbstractSocket::ClosingState:
            {labSocketState->setText("socket state：ClosingState");break;}
        case QAbstractSocket::ListeningState:
            {labSocketState->setText("socket state：ListeningState");break;}
    }
}

void MainWindow::onSocketReadyRead()
{
//    while(tcpClient->canReadLine())
//    {
//        ui->plainTextEditLocalHistory->appendPlainText("[in]"+tcpClient->readLine().toHex().toUpper());
//    }
    ui->plainTextEditLocalHistory->appendPlainText("[in]"+tcpClient->readAll().toHex().toUpper());
}

void MainWindow::on_actConnect_triggered()
{
    QString addr = ui->lineEditTarAddr->text();
    quint16 port = ui->spinBoxTarPort->value();
    tcpClient->connectToHost(addr,port);
}

void MainWindow::on_actDisconnect_triggered()
{
    if(tcpClient->state() == QAbstractSocket::ConnectedState)
    {
        tcpClient->disconnectFromHost();
    }
}

void MainWindow::on_actSend_triggered()
{
    if(tcpClient->state() == QAbstractSocket::ConnectedState)
    {
        QString msg = ui->lineEditCommand->text();
        QByteArray msgToSend;

        ConvertStringToHex(msg,msgToSend);
        ui->plainTextEditLocalHistory->appendPlainText("[out]"+msgToSend.toHex().toUpper());
        //ui->lineEditMsg->clear();
        ui->lineEditCommand->setFocus();
        tcpClient->write(msgToSend);
    }
    else
        ui->plainTextEditLocalHistory->appendPlainText("[Warning] Connection has been lost");
}

void MainWindow::on_actQuit_triggered()
{
    if(tcpClient->state() == QAbstractSocket::ConnectedState)
    {
        tcpClient->disconnectFromHost();
    }

    tcpClient->deleteLater();

    QApplication::exit();
}

void MainWindow::on_btnSetCommand_clicked()
{
    uint16_t crc16;
    QByteArray command;
    command.resize(8);
    command[0] = 0x55;
    command[1] = 0x55;
    command[2] = 0x03;
    command[3] = 0x02;
    switch (ui->comboBoxCommand->currentData().toInt())
    {
        case 0: //手动模式
            command[4] = 0x10;
            command[5] = ui->comboBoxDirection->currentData().toInt();
            crc16 = GetCRC16(command,6);
            command[6] = crc16;
            command[7] = crc16>>8;
            ui->lineEditCommand->setText(command.toHex().toUpper());
            break;
        case 1: //自动模式
            command[4] = 0x11;
            command[5] = ui->spinBoxCountOrPoint->value();
            crc16 = GetCRC16(command,6);
            command[6] = crc16;
            command[7] = crc16>>8;
            ui->lineEditCommand->setText(command.toHex().toUpper());
            break;
        case 2: //定点模式
            command[4] = 0x12;
            command[5] = ui->spinBoxCountOrPoint->value();
            crc16 = GetCRC16(command,6);
            command[6] = crc16;
            command[7] = crc16>>8;
            ui->lineEditCommand->setText(command.toHex().toUpper());
            break;
        default:
            break;
    }
}
