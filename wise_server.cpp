#include "wise_server.h"
#include "ui_wise_server.h"

wise_server::wise_server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wise_server) {
    ui->setupUi(this);
    this->server = new QTcpServer(this);
    this->socket = nullptr;
    ui->btn_send->setEnabled(false);
    QLabel *statu = new QLabel("未连接", this); //连接状态
    ui->statusbar->addWidget(statu);
    //设置默认端口号
    ui->lineEdit_ports->setText("20000");//设置默认端口号为20000
    connect(ui->btn_start, &QPushButton::clicked, this, [ = ]() {
        if(ui->btn_start->text() == "启动服务器") {
            unsigned int ports_num = ui->lineEdit_ports->text().toUInt();
            if(this->server->listen(QHostAddress::Any, ports_num)) {
                //服务器启动后，端口号不得更改
                ui->lineEdit_ports->setReadOnly(true);
                QMessageBox::information(this, "启动服务器", "服务器启动成功");
                ui->btn_start->setText("关闭服务器");
                ui->btn_start->setEnabled(false);
            }
        } else {
            this->server->close();
            //this->socket->close();//可以断掉连接
            this->socket->disconnectFromHost();
            ui->btn_start->setText("启动服务器");
            QMessageBox::information(this, "关闭服务器", "服务器关闭成功");
            ui->btn_send->setEnabled(false);
        }
    });
    //编写连接的逻辑
    connect(this->server, &QTcpServer::newConnection, this, [ = ]() {
        //获取与客户端通信的套接字
        this->socket = this->server->nextPendingConnection();
        ui->mes_recode->append("有新的客户连接");
        statu->setText("已连接");
        ui->btn_send->setEnabled(true);
        ui->btn_start->setEnabled(true);
        //连接后，可以收发消息,只有当socket 初始化后我才能正常使用
        connect(this->socket, &QTcpSocket::readyRead, this, [ = ]() {
            QByteArray data = this->socket->readAll();
            ui->mes_recode->append("客户端:" + data);
        });
        //当客户端与服务器断开连接的时候，socket会发送 disconnected 信号
        connect(this->socket, &QTcpSocket::disconnected, this, [ = ]() {
            statu->setText("未连接");
            ui->btn_send->setEnabled(false);
            //手动关闭一下套接字
            this->socket->close();
            ui->mes_recode->append("客户端已断开连接");
        });
    });
    //发送按钮
    connect(ui->btn_send, &QPushButton::clicked, this, [ = ]() {
        QString data = ui->mes_send->toPlainText();
        ui->mes_send->clear();
        this->socket->write(data.toUtf8());
        ui->mes_recode->append("服务器:" + data);
    });
    connect(ui->btn_clear, &QPushButton::clicked, ui->mes_send, &QTextEdit::clear);
    ui->btn_send->setDefault(true);
}

wise_server::~wise_server() {
    delete ui;
}
