#include "photo.h"
#include "ui_photo.h"

photo::photo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::photo) {
    ui->setupUi(this);
    backgroundPixmap = QPixmap(":/new/prefix1/page_res/400 (3).jpg");
    this->setWindowTitle("摄像界面");
    CameraInfoList = QCameraInfo::availableCameras();
    //遍历，将摄像头信息逐个添加到下拉列表当中
    QList<QCameraInfo>::const_iterator it;
    for(it = CameraInfoList.begin(); it != CameraInfoList.end(); it++) {
        ui->deviceComboBox->addItem((*it).description());
    }
    connect(ui->pushButton_2, &QPushButton::clicked, this, &photo::on_photographBt_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &photo::on_startBt_clicked);
}

photo::~photo() {
    delete ui;
}

void photo::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::RightButton) {
        this->hide();
        mainpage *page = new mainpage;
        page->setAttribute(Qt::WA_DeleteOnClose);
        page->show();
    }
}

void photo::on_startBt_clicked() {
    if(ui->pushButton->text() == "关闭摄像头") {
        Camera->stop();
        ui->pushButton->setText("开启摄像头");
    } else {
        ui->pushButton->setText("关闭摄像头");
        static int num = 0;
        if(num == 0) {
            //获取到要打开的设备的名称
            QCameraInfo CameraInfo = CameraInfoList.at(ui->deviceComboBox->currentIndex());
            //创建摄像头对象
            Camera = new QCamera(CameraInfo);
            //创建取景器并将其附在要显示的区域
            CameraViewFinder = new QCameraViewfinder(ui->CameraLabel);
            Camera->setViewfinder(CameraViewFinder);
            //调整摄像头数据显示的大小
            CameraViewFinder->resize(ui->CameraLabel->width(), ui->CameraLabel->height());
            //显示摄像头取景器
            CameraViewFinder->show();
            //开启摄像头
            num++;
        }
        Camera->start();
        //创建获取一帧数据对象
        CameraImageCapture = new  QCameraImageCapture(Camera);
        //关联图像获取信号
        connect(CameraImageCapture, &QCameraImageCapture::imageCaptured, this, &photo::take_photo);
    }
}


void photo::on_photographBt_clicked() {
    //发送获取一帧数据的信号
    CameraImageCapture->capture();
}

void photo::take_photo(int, const QImage &image) {
    //使用系统时间来命名图片的名称，时间是唯一的，图片名也是唯一的
    QDateTime dateTime(QDateTime::currentDateTime());
    QString time = dateTime.toString("yyyy-MM-dd-hh-mm-ss");
    //创建图片保存路径名
    QString filename = QString("../project/photo/%1.jpg").arg(time);
    //保存一帧数据
    image.save(filename);
    //提示获取到了一帧数据
    QMessageBox::information(this, "提示：", "获取到一帧图片");
}

void photo::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    // 创建一个绘图对象
    QPainter painter(this);
    // 绘制背景图片
    painter.drawPixmap(rect(), backgroundPixmap);
}
