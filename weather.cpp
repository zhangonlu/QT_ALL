#include "weather.h"
#include "ui_weather.h"

weather::weather(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::weather) {
    ui->setupUi(this);
    read_weather_info();
    this->setWindowTitle("天气预报界面");
    this->http = new QNetworkAccessManager(this);
    qDebug() << ui->comboBox_city->currentText();
    qDebug() << this->city_info[ui->comboBox_city->currentText()];
    QString ip = "http://t.weather.sojson.com/api/weather/city/" + this->city_info[ui->comboBox_city->currentText()];
    //发送请求
    this->http->get(QNetworkRequest(QUrl(ip)));
    connect(ui->pushButton_city, &QPushButton::clicked, this, [ = ]() {
        QDialog *dia = new QDialog(this);
        dia->setAttribute(Qt::WA_DeleteOnClose);
        QLabel *label = new QLabel("请输入城市名", dia);
        QLineEdit *line = new QLineEdit(dia);
        QPushButton *butt = new QPushButton("确定", dia);
        QHBoxLayout *hb = new QHBoxLayout;
        hb->addWidget(label);
        hb->addWidget(line);
        hb->addWidget(butt);
        dia->setLayout(hb);
        connect(butt, &QPushButton::clicked, dia, [ = ]() {
            QString city = line->text();
            if(ui->comboBox_city->findText(city) == -1) {
                QMessageBox::information(dia, "城市信息", "暂无当前城市信息");
                return;
            } else {
                ui->comboBox_city->setCurrentText(city);
                dia->close();
            }
        });
        dia->show();
    });
    connect(ui->comboBox_city, &QComboBox::currentTextChanged, this, &weather::send_request);
    connect(this->http, &QNetworkAccessManager::finished, this, &weather::handl_info);
}

weather::~weather() {
    delete ui;
}

void weather::send_request(QString) {
    QString ip = "http://t.weather.sojson.com/api/weather/city/" +
                 this->city_info[ui->comboBox_city->currentText()];
    //发送请求
    this->http->get(QNetworkRequest(QUrl(ip)));
}

void weather::read_weather_info() {
    QFile file(":/city_code.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(this, "打开文件", "打开失败");
        return;
    }
    while (!file.atEnd()) {
        QByteArray file_data = file.readLine();
        QString line = file_data;
        int pos = line.indexOf('\t');
        QString code = line.mid(0, pos);
        QString city = line.mid(pos + 1);
        city = city.replace("\n", "");
        ui->comboBox_city->addItem(city);
        this->city_info.insert(city, code);
    }
}

void weather::handl_info(QNetworkReply *reply) {
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << status;
    if(status != 200) {
        QMessageBox::question(this, "请求错误", "请求出错了");
        return;
    }
    QByteArray data = reply->readAll();
    //解析json数据
    QJsonDocument json_data = QJsonDocument::fromJson(data);
    qDebug() << json_data;
    if(json_data.isObject()) {
        //解析当天信息
        QJsonObject obj = json_data.object();
        QJsonObject city_info = obj["cityInfo"].toObject();
        QString city_name = city_info["city"].toString();
        QJsonObject data = obj["data"].toObject();
        QJsonArray forecast_all = data["forecast"].toArray();
        QJsonObject forecast1 = forecast_all[0].toObject();
        QString shidu = data["shidu"].toString();
        QString pm25 = QString::number( data["pm25"].toInt()) ;
        qDebug() << pm25;
        QString quality = data["quality"].toString();
        QString wendu = data["wendu"].toString();
        QString tip = forecast1["notice"].toString();
        QString type = forecast1["type"].toString();
        QString high = forecast1["high"].toString();
        QString low = forecast1["low"].toString();
        QString week = forecast1["week"].toString();
        QString wendu_hl = type + "\t" + high  + "/" + low ;
        ui->textEdit_info->setText(week + "\t" + wendu + "℃" + "\n" + wendu_hl + "\n" + tip)  ;
        ui->textEdit_quality->setText("空气质量" + quality + "\n");
        ui->textEdit_pm25->setText("pm2.5指数：" + pm25);
        ui->textEdit_fx->setText(forecast1["fx"].toString() + forecast1["fl"].toString());
        ui->textEdit_shidu->setText(shidu);
        QString img_name0 = "";
        img_handl(img_name0, type);
        QMovie *movie = new QMovie(img_name0);
        ui->label_image->setMovie(movie);
        movie->start();
        ui->label_image->setScaledContents(true);
        //第二天天气信息设置
        QJsonObject forecast = forecast_all[1].toObject();
        QString date1 = forecast["date"].toString();
        QString type1 = forecast["type"].toString();
        QString high1 = forecast["high"].toString();
        QString low1 = forecast["low"].toString();
        QString week1 = forecast["week"].toString();
        QString img_name1 = "";
        img_handl(img_name1, type1);
        QMovie *movie1 = new QMovie(img_name1);
        ui->label_date_1->setMovie(movie1);
        movie1->start();
        ui->label_date_1->setScaledContents(true);
        ui->lineEdit_date_1->setText(date1 + week1 + type1);
        ui->lineEdit_info_1->setText(type1 + "\t" + high1  + "/" + low1);
        //第二天天气设置
        QJsonObject forecast2 = forecast_all[2].toObject();
        QString date2 = forecast2["date"].toString();
        QString type2 = forecast2["type"].toString();
        QString high2 = forecast2["high"].toString();
        QString low2 = forecast2["low"].toString();
        QString week2 = forecast2["week"].toString();
        QString img_name2 = "";
        img_handl(img_name2, type2);
        QMovie *movie2 = new QMovie(img_name2);
        ui->label_date_2->setMovie(movie2);
        movie2->start();
        ui->label_date_2->setScaledContents(true);
        ui->lineEdit_date_2->setText(date2 + week2 + type2);
        ui->lineEdit_info_2->setText(type2 + "\t" + high2  + "/" + low2);
        //第三天
        QJsonObject forecast3 = forecast_all[3].toObject();
        QString date3 = forecast3["date"].toString();
        QString type3 = forecast3["type"].toString();
        QString high3 = forecast3["high"].toString();
        QString low3 = forecast3["low"].toString();
        QString week3 = forecast3["week"].toString();
        QString img_name3 = "";
        img_handl(img_name3, type3);
        QMovie *movie3 = new QMovie(img_name3);
        ui->label_date_3->setMovie(movie3);
        movie3->start();
        ui->label_date_3->setScaledContents(true);
        ui->lineEdit_date_3->setText(date3 + week3 + type3);
        ui->lineEdit_info_3->setText(type3 + "\t" + high3  + "/" + low3);
        //第四天
        QJsonObject forecast4 = forecast_all[4].toObject();
        QString date4 = forecast4["date"].toString();
        QString type4 = forecast4["type"].toString();
        QString high4 = forecast4["high"].toString();
        QString low4 = forecast4["low"].toString();
        QString week4 = forecast4["week"].toString();
        QString img_name4 = "";
        img_handl(img_name4, type4);
        QMovie *movie4 = new QMovie(img_name4);
        ui->label_date_4->setMovie(movie4);
        movie4->start();
        ui->label_date_4->setScaledContents(true);
        ui->lineEdit_date_4->setText(date4 + week4 + type4);
        ui->lineEdit_info_4->setText(type4 + "\t" + high4  + "/" + low4);
        QJsonObject forecast5 = forecast_all[5].toObject();
        QString date5 = forecast5["date"].toString();
        QString type5 = forecast5["type"].toString();
        QString high5 = forecast5["high"].toString();
        QString low5 = forecast5["low"].toString();
        QString week5 = forecast5["week"].toString();
        QString img_name5 = "";
        img_handl(img_name5, type5);
        QMovie *movie5 = new QMovie(img_name5);
        ui->label_date_5->setMovie(movie5);
        movie5->start();
        ui->label_date_5->setScaledContents(true);
        ui->lineEdit_date_5->setText(date5 + week5 + type5);
        ui->lineEdit_info_5->setText(type5 + "\t" + high5  + "/" + low5);
    }
}

void weather::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::RightButton) {
        this->hide();
        mainpage *page = new mainpage;
        page->setAttribute(Qt::WA_DeleteOnClose);
        page->show();
    }
}

void img_handl(QString &img_name, QString type) {
    if(type == "多云") {
        img_name = ":/new/prefix1/page_res/icons8-rain-cloud.gif";
    } else if(type == "晴") {
        img_name = ":/new/prefix1/page_res/icons8-sun.gif";
    } else if(type == "阴") {
        img_name = ":/new/prefix1/page_res/icons8-cloud.gif";
    } else if(type == "小雨") {
        img_name = ":/new/prefix1/page_res/icons8-rain.gif";
    } else if(type == "中雨") {
        img_name = ":/new/prefix1/page_res/icons8-rainfall.gif";
    } else if(type == "大雨") {
        img_name = ":/new/prefix1/page_res/icons8-heavy-rain.gif";
    }
}

