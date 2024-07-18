#ifndef WISE_CLIENT_H
#define WISE_CLIENT_H

#include <QMainWindow>

#include <QTcpSocket>
#include <QKeyEvent>
namespace Ui {
    class wise_client;
}

class wise_client : public QMainWindow {
    Q_OBJECT

public:
    explicit wise_client(QWidget *parent = 0);
    ~wise_client();

private:
    Ui::wise_client *ui;
    bool eventFilter(QObject *watched, QEvent *event)override;
    void keyPressEvent(QKeyEvent *e);
    QTcpSocket *socket;//客户端用于通信的套接字
};

#endif // WISE_CLIENT_H
