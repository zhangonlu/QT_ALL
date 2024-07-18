#include "mylabel.h"
#include "ui_mylabel.h"

mylabel::mylabel(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::mylabel) {
    ui->setupUi(this);
    this->flag = false;
}

mylabel::~mylabel() {
    delete ui;
}



void mylabel::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        emit mylabel_clicked(this->flag);
    }
}


void mylabel::enterEvent(QEvent *event) {
    this->flag = true;
}

