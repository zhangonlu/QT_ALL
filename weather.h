#ifndef WEATHER_H
#define WEATHER_H

#include <QMainWindow>
#include "mainwindow.h"
#include <QMap>
#include <QNetworkAccessManager>
#include <QSslSocket>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QMouseEvent>
#include "mainpage.h"
namespace Ui {
    class weather;
}

class weather : public QMainWindow {
    Q_OBJECT

public:
    explicit weather(QWidget *parent = 0);
    ~weather();
    //发送请求
    void send_request(QString);
    //读取文件信息 城市ID和城市名
    void read_weather_info();
    //处理请求到的json数据
    void handl_info(QNetworkReply *reply);
    //右键退出
    void mousePressEvent(QMouseEvent *event) override;
private:
    Ui::weather *ui;
    //城市信息 城市名 ： id
    QMap<QString, QString> city_info;
    //http请求
    QNetworkAccessManager *http;
};

void img_handl(QString &img_name, QString type);

#endif // WEATHER_H
