#include "wise_client.h"
#include "ui_wise_client.h"

wise_client::wise_client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wise_client) {
    ui->setupUi(this);
    this->socket = new QTcpSocket(this);
    ui->btn_send->setEnabled(false);
    ui->btn_disconnect->setEnabled(false);
    QLabel *statu = new QLabel("未连接", this);
    ui->statusbar->addWidget(statu);
    this->setWindowTitle("客户端");
    ui->host_ip->setText("127.0.0.1");//设置默认地址
    ui->lineEdit_ports->setText("20000");//设置默认端口
    //连接按钮
    connect(ui->btn_connect, &QPushButton::clicked, this, [ = ]() {
        QString adress = ui->host_ip->text();
        unsigned int port_num = ui->lineEdit_ports->text().toUInt();
        this->socket->connectToHost(adress, port_num); //连接服务器，但是不一定能连上
    });
    //如果连接成功会触发 connected
    connect(this->socket, &QTcpSocket::connected, this, [ = ]() {
        ui->mes_recode->append("已与服务器建立连接");
        ui->lineEdit_ports->setEnabled(false);
        ui->host_ip->setEnabled(false);
        ui->btn_connect->setEnabled(false);
        ui->btn_disconnect->setEnabled(true);
        ui->btn_send->setEnabled(true);
        statu->setText("已连接");
    });
    connect(ui->btn_disconnect, &QPushButton::clicked, this->socket, &QTcpSocket::disconnectFromHost);
    //断开连接 触发 disconnected
    connect(this->socket, &QTcpSocket::disconnected, this, [ = ]() {
        ui->mes_recode->append("已与服务器断开连接");
        this->socket->close();
        statu->setText("未连接");
        ui->lineEdit_ports->setEnabled(true);
        ui->host_ip->setEnabled(true);
        ui->btn_connect->setEnabled(true);
        ui->btn_disconnect->setEnabled(false);
        ui->btn_send->setEnabled(false);
    });
    connect(ui->btn_clear, &QPushButton::clicked, ui->mes_send, &QTextEdit::clear);
    //发送与接收
    connect(ui->btn_send, &QPushButton::clicked, this, [ = ]() {
        QString data = ui->mes_send->toPlainText();
        ui->mes_recode->append("客户端:" + data);
        this->socket->write(data.toUtf8());
        emit ui->btn_clear->clicked();
    });
    connect(this->socket, &QTcpSocket::readyRead, this, [ = ]() {
        QByteArray data = this->socket->readAll();
        ui->mes_recode->append("服务器:" + data);
    });
    ui->mes_send->installEventFilter(this->centralWidget());
}

wise_client::~wise_client() {
    delete ui;
}

bool wise_client::eventFilter(QObject *watched, QEvent *event) {
    if(watched == ui->mes_send && event->type() == QEvent::KeyPress) {
        QKeyEvent *e = dynamic_cast<QKeyEvent *>(event);
        keyPressEvent(e);
        return true;
    }
    return wise_client::eventFilter(watched, event);
}

void wise_client::keyPressEvent(QKeyEvent *e) {
    qDebug() << (e->key() == Qt::Key_Enter);
    if(e->key() == Qt::Key_Enter && ui->btn_send->isEnabled()) {
        emit ui->btn_send->clicked();
    }
}
