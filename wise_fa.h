#ifndef WISE_FA_H
#define WISE_FA_H

#include <QWidget>
#include <QMovie>
#include <QTimer>
#include "mainpage.h"
#include <QPainter>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QDebug>

namespace Ui {
    class wise_fa;
}
//
class wise_fa : public QWidget {
    Q_OBJECT

public:
    explicit wise_fa(QWidget *parent = 0);
    ~wise_fa();
    void handle_fun(bool flag, QString send);
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    Ui::wise_fa *ui;
    //定时器
    QTimer *timer;
    //室内温度
    double wendu;
    QPixmap backgroundPixmap;
    QTcpServer *server;
    QTcpSocket *socket;//客户端用于通信的套接字
};

#endif // WISE_FA_H
