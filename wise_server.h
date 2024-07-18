#ifndef WISE_SERVER_H
#define WISE_SERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
namespace Ui {
    class wise_server;
}

class wise_server : public QMainWindow {
    Q_OBJECT

public:
    explicit wise_server(QWidget *parent = 0);
    ~wise_server();
    QTcpServer *server;//服务器对象
    QTcpSocket *socket;//套接字通对象
private:
    Ui::wise_server *ui;
};

#endif // WISE_SERVER_H
