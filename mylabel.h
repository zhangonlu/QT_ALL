#ifndef MYLABEL_H
#define MYLABEL_H

#include <QWidget>
#include <QMouseEvent>
#include <QEvent>
#include <QLabel>
#include <QPaintDevice>
#include <QPainter>
namespace Ui {
    class mylabel;
}

class mylabel : public QLabel {
    Q_OBJECT

public:
    explicit mylabel(QWidget *parent = 0);
    ~mylabel();


    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override ;
signals:
    void mylabel_clicked(bool flag);
private:
    Ui::mylabel *ui;
    bool flag;
};

#endif // MYLABEL_H
