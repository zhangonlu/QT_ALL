#ifndef MUSIC_H
#define MUSIC_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidget>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkRequest>
#include <QMediaContent>
#include <QMediaMetaData>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTextEdit>
#include <QMap>
#include <QTableWidget>
#include "mainpage.h"
#include <QString>
#include <QTextStream>
#include <QPainter>
#include <QEvent>
namespace Ui {
    class music;
}

class music : public QWidget {
    Q_OBJECT

public:
    explicit music(QWidget *parent = 0);
    ~music();
    //发送请求
    void send_request();
    //处理请求信息
    void handle_info(QNetworkReply *reply);
    //右键退出
    void mousePressEvent(QMouseEvent *event) override;
    //读取歌曲信息
    void read_file();
    //绘制背景
    void paintEvent(QPaintEvent *event) override;


private:
    Ui::music *ui;
    //播放器
    QMediaPlayer *player;
    //播放列表
    QMediaPlaylist *play_list;
    //歌曲信息请求
    QNetworkAccessManager *http;
    //歌曲实际地址请求
    QNetworkAccessManager *http1;
    //歌词请求
    QNetworkAccessManager *http2;
    //存储歌曲信息
    QMap<QString, QString> song_info;
    QString total;
    //歌曲url
    QString *song_url;
    QTableWidget *table;
    int row;
    QTimer *timer;
    int time;

    QPixmap backgroundPixmap;
};

#endif // MUSIC_H
