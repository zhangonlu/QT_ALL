#include "wise_fa.h"
#include "ui_wise_fa.h"
#include "mylabel.h"
#include <QHBoxLayout>

wise_fa::wise_fa(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wise_fa) {
    ui->setupUi(this);
    this->wendu = 24;
    this->timer = new QTimer();
    this->timer->setInterval(1000);
    this->setWindowTitle("智能家居界面");
    this->socket = new QTcpSocket(this);
    backgroundPixmap = QPixmap(":/new/prefix1/page_res/weather.webp");
    connect(this->timer, &QTimer::timeout, this, [ = ]() {
        if(ui->pushButton_kongtiao->text() == "打开") {
            this->timer->stop();
        }
        if(this->wendu < 20) {
            this->wendu = this->wendu + 0.5;
            ui->kongtiao_wendu->setValue(this->wendu);
        } else if(this->wendu > 24) {
            this->wendu -= 0.5;
            ui->kongtiao_wendu->setValue(this->wendu);
        } else {
            this->timer->stop();
            ui->kongtiao_wendu->setValue(this->wendu);
        }
    });
    ui->light1->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
    ui->light1->setScaledContents(true);
    ui->pushButton_light1->setText("打开");
    connect(ui->pushButton_light1, &QPushButton::clicked, this, [ = ]() {
        if(ui->pushButton_light1->text() == "打开") {
            ui->light1->setPixmap(QPixmap(":/new/prefix1/page_res/灯亮.jpg"));
            ui->pushButton_light1->setText("关闭");
            this->socket->write("LED1_open\r\n");
        } else {
            ui->light1->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
            ui->pushButton_light1->setText("打开");
            this->socket->write("LED1_close\r\n");
        }
    });
    ui->pushButton_send->setEnabled(false);
    //设置默认端口
    ui->ip->setText("192.168.4.1");
    ui->lineEdit_port->setText("10000");
    //连接按钮
    connect(ui->pushButton_connect_2, &QPushButton::clicked, this, [ = ]() {
        this->socket->connectToHost(ui->ip->text(), (unsigned int)ui->lineEdit_port->text().toInt());
    });
    //连接成功触发
    connect(this->socket, &QTcpSocket::connected, this, [ = ]() {
        ui->pushButton_connect_2->setEnabled(false);
        ui->pushButton_disconnect_2->setEnabled(true);
        ui->pushButton_send->setEnabled(true);
        ui->ip->setEnabled(false);
        ui->lineEdit_port->setEnabled(false);
        QMessageBox::information(this, "连接服务器", "连接成功");
    });
    connect(ui->pushButton_disconnect_2, &QPushButton::clicked, this->socket, &QTcpSocket::disconnectFromHost);
    connect(this->socket, &QTcpSocket::disconnected, this, [ = ]() {
        ui->textEdit_record_2->append("已断开连接");
        ui->pushButton_connect_2->setEnabled(true);
        this->socket->close();
    });
    connect(ui->light1, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handle_fun(flag, "light1");
    });
    ui->fs->setPixmap(QPixmap(":/new/prefix1/page_res/风扇停止.jpg"));
    ui->fs->setScaledContents(true);
    // 加载动态图像文件
    QMovie *movie = new QMovie(":/new/prefix1/page_res/风扇动态.gif");
    QMovie *movie_watch = new QMovie(":/new/prefix1/page_res/电视动态.gif");
    QMovie *movie_kongtiao = new QMovie(":/new/prefix1/page_res/空调动态.gif");
    connect(ui->pushButton_fs, &QPushButton::clicked, this, [ = ]() {
        ui->fs->setPixmap(QPixmap(":/new/prefix1/page_res/风扇停止.jpg"));
        this->socket->write("fs_0\r\n");
//        if(ui->pushButton_fs->text() == "打开") {
//            // 将动态图像绑定到QLabel上
//            ui->fs->setMovie(movie);
//            // 播放动画
//            movie->start();
//            ui->pushButton_fs->setText("关闭");
//            this->socket->write("fs_1\r\n");
//        } else {
//        }
    });
    connect(ui->fs, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handle_fun(flag, "fs");
    });
    ui->light2->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
    ui->light2->setScaledContents(true);
    ui->pushButton_light2->setText("打开");
    connect(ui->pushButton_light2, &QPushButton::clicked, this, [ = ]() {
        if(ui->pushButton_light2->text() == "打开") {
            ui->light2->setPixmap(QPixmap(":/new/prefix1/page_res/灯亮.jpg"));
            ui->pushButton_light2->setText("关闭");
            this->socket->write("LED2_open\r\n");
        } else {
            ui->light2->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
            ui->pushButton_light2->setText("打开");
            this->socket->write("LED2_close\r\n");
        }
    });
    connect(ui->light2, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handle_fun(flag, "light2");
    });
    ui->fs1->setPixmap(QPixmap(":/new/prefix1/page_res/风扇停止.jpg"));
    ui->fs1->setScaledContents(true);
    ui->pushButton_fs1->setText("打开");
    // 加载动态图像文件
    connect(ui->pushButton_fs1, &QPushButton::clicked, this, [ = ]() {
        if(ui->pushButton_fs1->text() == "打开") {
            // 将动态图像绑定到QLabel上
            ui->fs1->setMovie(movie);
            // 播放动画
            movie->start();
            ui->pushButton_fs1->setText("关闭");
        } else {
            ui->fs1->setPixmap(QPixmap(":/new/prefix1/page_res/风扇停止.jpg"));
            ui->pushButton_fs1->setText("打开");
        }
    });
    connect(ui->fs1, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handle_fun(flag, "fs1");
    });
    ui->light3->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
    ui->light3->setScaledContents(true);
    ui->pushButton_light3->setText("打开");
    connect(ui->pushButton_light3, &QPushButton::clicked, this, [ = ]() {
        if(ui->pushButton_light3->text() == "打开") {
            ui->light3->setPixmap(QPixmap(":/new/prefix1/page_res/灯亮.jpg"));
            ui->pushButton_light3->setText("关闭");
            this->socket->write("LED3_open\r\n");
        } else {
            ui->light3->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
            ui->pushButton_light3->setText("打开");
            this->socket->write("LED3_close\r\n");
        }
    });
    connect(ui->light3, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handle_fun(flag, "light3");
    });
    ui->light4->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
    ui->light4->setScaledContents(true);
    ui->pushButton_light4->setText("打开");
    connect(ui->pushButton_light4, &QPushButton::clicked, this, [ = ]() {
        if(ui->pushButton_light4->text() == "打开") {
            ui->light4->setPixmap(QPixmap(":/new/prefix1/page_res/灯亮.jpg"));
            ui->pushButton_light4->setText("关闭");
        } else {
            ui->light4->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
            ui->pushButton_light4->setText("打开");
        }
    });
    connect(ui->light4, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handle_fun(flag, "light4");
    });
    ui->light5->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
    ui->light5->setScaledContents(true);
    ui->pushButton_light5->setText("打开");
    connect(ui->pushButton_light5, &QPushButton::clicked, this, [ = ]() {
        if(ui->pushButton_light5->text() == "打开") {
            ui->light5->setPixmap(QPixmap(":/new/prefix1/page_res/灯亮.jpg"));
            ui->pushButton_light5->setText("关闭");
        } else {
            ui->light5->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
            ui->pushButton_light5->setText("打开");
        }
    });
    connect(ui->light5, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handle_fun(flag, "light5");
    });
    ui->watch->setPixmap(QPixmap(":/new/prefix1/page_res/电视.jpg"));
    ui->watch->setScaledContents(true);
    ui->pushButton_watch->setText("打开");
    connect(ui->pushButton_watch, &QPushButton::clicked, this, [ = ]() {
        if(ui->pushButton_watch->text() == "打开") {
            ui->watch->setMovie(movie_watch);
            // 播放动画
            movie_watch->start();
            ui->pushButton_watch->setText("关闭");
        } else {
            ui->watch->setPixmap(QPixmap(":/new/prefix1/page_res/电视.jpg"));
            ui->pushButton_watch->setText("打开");
        }
    });
    connect(ui->watch, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handle_fun(flag, "watch");
    });
    void (QDoubleSpinBox::*p)(double value) = QDoubleSpinBox::valueChanged;
    ui->kongtiao->setPixmap(QPixmap(":/new/prefix1/page_res/空调停止.jpg"));
    ui->kongtiao->setScaledContents(true);
    ui->pushButton_kongtiao->setText("打开");
    connect(ui->pushButton_kongtiao, &QPushButton::clicked, this, [ = ]() {
        if(ui->pushButton_kongtiao->text() == "打开") {
            ui->kongtiao->setMovie(movie_kongtiao);
            // 播放动画
            movie_kongtiao->start();
            ui->pushButton_kongtiao->setText("关闭");
        } else {
            ui->kongtiao->setPixmap(QPixmap(":/new/prefix1/page_res/空调停止.jpg"));
            ui->pushButton_kongtiao->setText("打开");
        }
    });
    connect(ui->kongtiao, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handle_fun(flag, "kongtiao");
    });
    ui->kongtiao_wendu->setValue(this->wendu);
    ui->kongtiao_wendu->setSingleStep(0.5);
    ui->kongtiao_wendu->setRange(0, 100);
    connect(ui->kongtiao_wendu, p, this, [ = ](int value) {
        this->wendu = value;
        if(value > 24 || value < 20) {
            //开启空调
            if(ui->pushButton_kongtiao->text() == "打开") {
                emit ui->pushButton_kongtiao->clicked();
            }
            this->timer->start();
        } else {
            if(ui->pushButton_kongtiao->text() == "关闭") {
                emit ui->pushButton_kongtiao->clicked();
            }
        }
    });
    //客户端接收到服务器的消息
    connect(this->socket, &QTcpSocket::readyRead, this, [ = ]() {
        QByteArray msg = this->socket->readAll();
        ui->textEdit_record_2->append(msg.data());
        //解析数据
        QString info = msg.data();
        QStringList list = info.split(",");
        qDebug() << list.length();
        if(list.length() < 8) {
            return;
        }
        QString LED1 = list[1];
        QString LED2 = list[2];
        QString LED3 = list[3];
        QString Hum = list[4];
        QString Tem = list[5];
        QString Light = list[6];
        QString fs_status = list[7];
        if(Light !=  "0" && Hum != "0") {
            ui->label_light->setText(Light);
            ui->label_wendu->setText(Tem);
            ui->label_shidu->setText(Hum);
        }
        //qDebug() << 246 + " " << s;
        int num = ui->lineEdit_value->text().toInt();
        if(Light.toInt() > num) {
            this->socket->write("BEEP_open\r\n");
        } else {
            this->socket->write("BEEP_close\r\n");
        }
        QString str = msg.data();
        if(LED2.contains("LED2_open")) {
            ui->light2->setPixmap(QPixmap(":/new/prefix1/page_res/灯亮.jpg"));
            ui->pushButton_light2->setText("关闭");
        } else if(LED2.contains("LED2_close")) {
            ui->light2->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
            ui->pushButton_light2->setText("打开");
        }
        if( LED1.contains("LED1_open")) {
            ui->light1->setPixmap(QPixmap(":/new/prefix1/page_res/灯亮.jpg"));
            ui->pushButton_light1->setText("关闭");
        } else if( LED1.contains("LED1_close")) {
            ui->light1->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
            ui->pushButton_light1->setText("打开");
        }
        if(LED3.contains("LED3_open")) {
            ui->light3->setPixmap(QPixmap(":/new/prefix1/page_res/灯亮.jpg"));
            ui->pushButton_light3->setText("关闭");
        } else if(LED3.contains("LED3_close")) {
            ui->light3->setPixmap(QPixmap(":/new/prefix1/page_res/灯泡.jpg"));
            ui->pushButton_light3->setText("打开");
        }
        if(fs_status.contains("0")) {
            ui->fs->setPixmap(QPixmap(":/new/prefix1/page_res/风扇停止.jpg"));
            ui->pushButton_fs_1->setEnabled(true);
            ui->pushButton_fs_2->setEnabled(true);
            ui->pushButton_fs_3->setEnabled(true);
        } else if(fs_status.contains("1")) {
            // 将动态图像绑定到QLabel上
            ui->fs->setMovie(movie);
            // 播放动画
            movie->start();
            ui->pushButton_fs_1->setEnabled(false);
            ui->pushButton_fs_2->setEnabled(true);
            ui->pushButton_fs_3->setEnabled(true);
        } else if(fs_status.contains("2")) {
            // 将动态图像绑定到QLabel上
            ui->fs->setMovie(movie);
            // 播放动画
            movie->start();
            ui->pushButton_fs_1->setEnabled(true);
            ui->pushButton_fs_2->setEnabled(false);
            ui->pushButton_fs_3->setEnabled(true);
        } else if(fs_status.contains("3")) {
            // 将动态图像绑定到QLabel上
            ui->fs->setMovie(movie);
            // 播放动画
            movie->start();
            ui->pushButton_fs_1->setEnabled(true);
            ui->pushButton_fs_2->setEnabled(true);
            ui->pushButton_fs_3->setEnabled(false);
        }
    });
    connect(ui->pushButton_fs_1, &QPushButton::clicked, this, [ = ]() {
        this->socket->write("fs_open_1");
        // 将动态图像绑定到QLabel上
        ui->fs->setMovie(movie);
        // 播放动画
        movie->start();
        ui->pushButton_fs_1->setEnabled(false);
        ui->pushButton_fs_2->setEnabled(true);
        ui->pushButton_fs_3->setEnabled(true);
    });
    connect(ui->pushButton_fs_2, &QPushButton::clicked, this, [ = ]() {
        this->socket->write("fs_open_2");
        // 将动态图像绑定到QLabel上
        ui->fs->setMovie(movie);
        // 播放动画
        movie->start();
        ui->pushButton_fs_1->setEnabled(true);
        ui->pushButton_fs_2->setEnabled(false);
        ui->pushButton_fs_3->setEnabled(true);
    });
    connect(ui->pushButton_fs_3, &QPushButton::clicked, this, [ = ]() {
        this->socket->write("fs_open_3");
        // 将动态图像绑定到QLabel上
        ui->fs->setMovie(movie);
        // 播放动画
        movie->start();
        ui->pushButton_fs_1->setEnabled(true);
        ui->pushButton_fs_2->setEnabled(true);
        ui->pushButton_fs_3->setEnabled(false);
    });
}
wise_fa::~wise_fa() {
    delete ui;
}
void wise_fa::handle_fun(bool, QString send) {
    if(send == "light1") {
        emit ui->pushButton_light1->clicked();
    }
    if(send == "fs") {
        emit ui->pushButton_fs->clicked();
    }
    if(send == "fs1") {
        emit ui->pushButton_fs1->clicked();
    }
    if(send == "light2") {
        emit ui->pushButton_light2->clicked();
    }
    if(send == "light3") {
        emit ui->pushButton_light3->clicked();
    }
    if(send == "light4") {
        emit ui->pushButton_light4->clicked();
    }
    if(send == "light5") {
        emit ui->pushButton_light5->clicked();
    }
    if(send == "watch") {
        emit ui->pushButton_watch->clicked();
    }
    if(send == "kongtiao") {
        emit ui->pushButton_kongtiao->clicked();
    }
}
void wise_fa::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::RightButton) {
        this->hide();
        mainpage *page = new mainpage;
        page->setAttribute(Qt::WA_DeleteOnClose);
        page->show();
    }
}
void wise_fa::paintEvent(QPaintEvent *event) {
    // 调用基类的paintEvent以确保正确绘制窗口的其他部分
    QWidget::paintEvent(event);
    // 创建一个绘图对象
    QPainter painter(this);
    // 绘制背景图片
    painter.drawPixmap(rect(), backgroundPixmap);
}
