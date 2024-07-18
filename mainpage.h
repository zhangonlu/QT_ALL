#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QMouseEvent>
#include <QGridLayout>
#include "music.h"
#include "wise_fa.h"
#include "photo.h"
namespace Ui {
    class mainpage;
}

class mainpage : public QWidget {
    Q_OBJECT

public:
    explicit mainpage(QWidget *parent = 0);
    ~mainpage();
    //处理点击事件 点击图片进入相应的界面
    void handl_clicked(bool flag, QString name);
    //右键非label区域实现退出
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::mainpage *ui;
    QPixmap backgroundPixmap;
};

#endif // MAINPAGE_H
