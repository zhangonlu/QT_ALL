#include "mainpage.h"
#include "ui_mainpage.h"
#include "weather.h"
#include "music.h"
#include <QLabel>
#include "mylabel.h"
mainpage::mainpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainpage) {
    ui->setupUi(this);
    this->setWindowTitle("菜单界面");
    backgroundPixmap = QPixmap(":/new/prefix1/page_res/1.jpg");
    mylabel *wea = new mylabel(this);
    wea->setScaledContents(true);
    wea->setPixmap(QPixmap(":/new/prefix1/page_res/OIP (5).jpg"));
    mylabel *music_mylabel = new mylabel(this);
    music_mylabel->setPixmap(QPixmap(":/new/prefix1/page_res/wyysjyy.webp"));
    music_mylabel->setScaledContents(true);
    mylabel *wise_fa = new mylabel(this);
    wise_fa->setScaledContents(true);
    wise_fa->setPixmap(QPixmap(":/new/prefix1/page_res/OIP (7).jpg"));
    mylabel *photo = new mylabel(this);
    photo->setScaledContents(true);
    photo->setPixmap(QPixmap(":/new/prefix1/page_res/OIP (8).jpg"));
    QGridLayout *gri = new QGridLayout;
    gri->addItem(new QSpacerItem(50, 0), 0, 0);
    gri->addWidget(wea, 0, 1);
    gri->addWidget(music_mylabel, 0, 2);
    gri->addItem(new QSpacerItem(50, 0), 1, 0);
    gri->addWidget(photo, 1, 1);
    gri->addWidget(wise_fa, 1, 2);
    ui->widget->setLayout(gri);
    connect(wea, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handl_clicked(flag, "wea");
    });
    connect(music_mylabel, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handl_clicked(flag, "music_mylabel");
    });
    connect(photo, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handl_clicked(flag, "photo");
    });
    connect(wise_fa, &mylabel::mylabel_clicked, this, [ = ](bool flag) {
        handl_clicked(flag, "wise");
    });
}

mainpage::~mainpage() {
    delete ui;
}

void mainpage::handl_clicked(bool flag, QString name) {
    if(flag == true) {
        if(name == "wea") {
            weather *wea_page = new weather;
            wea_page->setAttribute(Qt::WA_DeleteOnClose);
            wea_page->show();
            this->hide();
        }
        if(name == "music_mylabel") {
            music *music_page = new music;
            music_page->setAttribute(Qt::WA_DeleteOnClose);
            music_page->show();
            this->hide();
        }
        if(name == "wise") {
            wise_fa *wis = new wise_fa;
            wis->setAttribute(Qt::WA_DeleteOnClose);
            wis->show();
            this->hide();
        }
        if(name == "photo") {
            photo *ph = new photo;
            ph->setAttribute(Qt::WA_DeleteOnClose);
            ph->show();
            this->hide();
        }
    }
}

void mainpage::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::RightButton) {
        MainWindow *main = new MainWindow;
        main->setAttribute(Qt::WA_DeleteOnClose);
        main->show();
        this->close();
    }
}

void mainpage::paintEvent(QPaintEvent *event) {
    // 调用基类的paintEvent以确保正确绘制窗口的其他部分
    QWidget::paintEvent(event);
    // 创建一个绘图对象
    QPainter painter(this);
    // 绘制背景图片
    painter.drawPixmap(rect(), backgroundPixmap);
}


