#ifndef PHOTO_H
#define PHOTO_H

#include <QWidget>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QMessageBox>
#include <QDateTime>
#include "mainpage.h"
#include <QPainter>
namespace Ui {
    class photo;
}

class photo : public QWidget {
    Q_OBJECT

public:
    explicit photo(QWidget *parent = 0);
    ~photo();
    //右键退出
    void mousePressEvent(QMouseEvent *event) override;
private slots:
    //点击按钮 打开摄像头
    void on_startBt_clicked();
    //捕获照片
    void on_photographBt_clicked();
    //保存图片
    void take_photo(int id, const QImage &image);

    //绘制背景
    void paintEvent(QPaintEvent *event) override;
private:
    Ui::photo *ui;
    //摄像头对象指针
    QCamera *Camera;
    //摄像头的取景器
    QCameraViewfinder *CameraViewFinder;
    //存储摄像机设备信息
    QList<QCameraInfo> CameraInfoList;
    //记录摄像头内容
    QCameraImageCapture *CameraImageCapture;
    //照片的名字
    QString ImageName;
    //背景图片
    QPixmap backgroundPixmap;
};

#endif // PHOTO_H
