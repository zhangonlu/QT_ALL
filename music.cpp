#include "music.h"
#include "ui_music.h"
#include "mainwindow.h"
music::music(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::music) {
    ui->setupUi(this);
    this->setWindowTitle("音乐界面");
    // 设置背景图片
    backgroundPixmap = QPixmap(":/new/prefix1/page_res/a.jpg");
    this->table = new QTableWidget(this);
    table->hide();
    this->player = new QMediaPlayer(this);
    this->play_list = new QMediaPlaylist(this);
    this->timer = new QTimer(this);
    timer->setInterval(1000);
    time = 0;
    this->http2 = new QNetworkAccessManager(this);
    connect(this->timer, &QTimer::timeout, this, [ = ]() {
        time++;
        ui->jindu->setValue(time);
        ui->label_current->setText(QString::number(time));
        if(time >= ui->label_end->text().toInt()) {
            timer->stop();
        }
    });
    connect(ui->jindu, &QSlider::valueChanged, this, [ = ]() {
        time = ui->jindu->value();
    });
    read_file();
    //给播放器设置播放列表
    this->player->setPlaylist(play_list);
    //设置默认播放模式
    this->play_list->setPlaybackMode(QMediaPlaylist::Loop);
    //给播放器设置音量范围和初始音量
    this->player->setVolume(50);
    ui->volum->setRange(0, 100);
    ui->volum->setSingleStep(1);
    ui->volum->setValue(50);
    // 显示搜索音乐的窗口
    table->setColumnCount(4);  // 设置列数
    table ->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不可编辑
    table ->setShowGrid(false); // 关闭网格
    table ->setFocusPolicy(Qt::NoFocus); //去掉虚线格
    table ->verticalHeader() ->setHidden(true); // 去掉表头行号
    table->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: transparent; }");
    table->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: transparent; }");
    QStringList vtable_list;
    vtable_list << "歌名"  << "歌手"  << "id" << "时长";
    table->setFixedSize(600, 400);
    table->setHorizontalHeaderLabels(vtable_list);
    // 设置表格选择行为为选中整行
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    //隐藏最后一栏
    table->setColumnHidden(table->columnCount() - 1, true);
    connect(ui->pushButton_close, &QPushButton::clicked, this, [ = ]() {
        table->hide();
        row = 0;
    });
    //添加本地音乐
//    this->play_list->addMedia(QMediaContent(QUrl("qrc:/music/何洁 - 你是我的风景.mp3")));
//    this->play_list->addMedia(QMediaContent(QUrl("qrc:/music/AZU - For You.mp3")));
//    this->play_list->addMedia(QMediaContent(QUrl("qrc:/music/Chanlex - 特别的人.mp3")));
//    this->play_list->addMedia(QMediaContent(QUrl("qrc:/music/Rainton桐 - 最后的旅行（纯歌版）.mp3")));
//    this->play_list->addMedia(QMediaContent(QUrl("qrc:/music/林加弦 - 夏天的风（男声 吉他版）.mp3")));
//    ui->listWidget_musiclist->addItems(QStringList()
//                                       << "何洁 - 你是我的风景.mp3"
//                                       << "AZU - For You.mp3"
//                                       << "Chanlex - 特别的人.mp3"
//                                       << "Rainton桐 - 最后的旅行（纯歌版）.mp3"
//                                       << "林加弦 - 夏天的风（男声 吉他版）.mp3");
    connect(ui->jindu, &QSlider::sliderReleased, this, [ = ]() {
        int time = ui->jindu->value();
        player->setPosition(time * 1000 );
    });
    //歌词请求处理
    connect(this->http2, &QNetworkAccessManager::finished, this, [ = ](QNetworkReply * reply) {
        int status =  reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(status != 200) {
            QMessageBox::critical(this, "请求", "当前无歌词");
            return;
        } else {
            QByteArray data = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if(doc.isObject()) {
                QJsonObject obj = doc.object();
                QJsonObject lrc = obj["lrc"].toObject();
                QString song_word = lrc["lyric"].toString();
                QStringList parts = song_word.split("\n");
                for(int i = 0 ; i < parts.size(); i++) {
                    qDebug() << 81 << parts[i];
                    QRegularExpression regex("\\[(\\d{2}::\\d{2}::\\d{2})\\]");
                    // 进行匹配
                    QRegularExpressionMatchIterator k = regex.globalMatch(parts[i]);
                    // 遍历匹配结果
                    while (k.hasNext()) {
                        qDebug() << 87;
                        QRegularExpressionMatch match = k.next();
                        QString timestamp = match.captured(1); // 使用captured(1)来获取匹配到的第一个捕获组
                        qDebug() << "Found timestamp:" << timestamp;
                    }
                }
            }
        }
    });
    //播放按钮
    connect(ui->pushButton_stop, &QPushButton::clicked, this, [ = ]() {
        int index = ui->listWidget_musiclist->currentRow();
        this->play_list->setCurrentIndex(index);
        if(ui->pushButton_stop->text() == "播放" ) {
            time = 0;
            this->player->play();
            QString name = ui->listWidget_musiclist->currentItem()->text();
            QFile file("../project/song.txt");
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::critical(this, "获取时长", "获取失败");
            } else {
                int flag = 0;
                while (!file.atEnd()) {
                    QByteArray data = file.readLine();
                    QString   str   = data.data();
                    QStringList parts = str.split("---");
                    qDebug() << 58;
                    if(parts[0] == name) {
                        qDebug() << 59;
                        QString number = parts[3].trimmed();
                        int duraction = number.toInt();
                        ui->jindu->setRange(0, duraction);
                        ui->label_end->setText(QString::number(duraction));
                        ui->label_current->setText("0");
                        QString id = parts[2];
                        timer->start();
                        flag = 1;
                        QString ip = "http://iwxyi.com:3000/lyric?id=" + id;
                        this->http2->get(QNetworkRequest(QUrl(ip)));
                        break;
                    }
                }
                if(flag == 0) {
                    QMessageBox::critical(this, "获取时长", "获取时长失败");
                }
                file.close();
            }
            ui->pushButton_stop->setText("暂停");
        } else {
            this->player->pause();
            this->timer->stop();
            ui->pushButton_stop->setText("播放");
        }
    });
    connect(ui->pushButton_premusic, &QPushButton::clicked, this->play_list, &QMediaPlaylist::previous);
    connect(ui->pushButton_nextmusic, &QPushButton::clicked, this->play_list, &QMediaPlaylist::next);
    //音量控制
    connect(ui->volum, &QSlider::valueChanged, this->player, &QMediaPlayer::setVolume);
    connect(ui->comboBox_model, &QComboBox::currentTextChanged, this, [ = ](QString text) {
        if(text == "随机播放") {
            this->play_list->setPlaybackMode(QMediaPlaylist::Random);
        } else if(text == "列表循环") {
            this->play_list->setPlaybackMode(QMediaPlaylist::Loop);
        } else if(text == "单曲循环") {
            this->play_list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        }
    });
    if(ui->listWidget_musiclist->count() > 0) {
        ui->listWidget_musiclist->setCurrentRow(0);
    }
    connect(this->play_list, &QMediaPlaylist::currentIndexChanged, this, [ = ](int index) {
        ui->listWidget_musiclist->setCurrentRow(index);
    });
    //双击播放
    connect(ui->listWidget_musiclist, &QListWidget::itemDoubleClicked, this, [ = ](QListWidgetItem * item) {
        //获取双击条目的索引和当前播放音乐索引
        int index =  ui->listWidget_musiclist->row(item);
        int play_index = this->play_list->currentIndex();
        if(index == play_index) {
            emit ui->pushButton_stop->click();
        } else {
            this->play_list->setCurrentIndex(index);
            this->player->play();
            ui->pushButton_stop->setText("暂停");
        }
    });
    //添加本地音乐和删除音乐
    connect(ui->pushButton_addmusic, &QPushButton::clicked, this, [ = ]() {
        QStringList data = QFileDialog::getOpenFileNames(this, "选择音乐"
                           , "C:\\Users\\zhangmingdong\\Desktop\\hqyj_note\\QT\\project\\music",
                           "*.mp3 *.mp4 *.wav *.acc");
        for(int i = 0; i < data.size() ; i++) {
            if(this->play_list->addMedia(QMediaContent(QUrl::fromLocalFile(data[i])))) {
                QFileInfo info(data[i]);
                ui->listWidget_musiclist->addItem(info.fileName());
                int num = 0;
                if(num == 0) {
                    num++;
                    ui->pushButton_stop->setEnabled(true);
                }
            }
        }
    });
    connect(ui->pushButton_removemusic, &QPushButton::clicked, this, [ = ]() {
        int index = ui->listWidget_musiclist->currentRow();
        if(this->play_list->removeMedia(index)) {
            ui->listWidget_musiclist->takeItem(index);
        } else {
            QMessageBox::critical(this, "删除", "没有文件可以删除");
            return;
        }
        if(this->play_list->isEmpty()) {
            if(ui->pushButton_stop->text() == "暂停") {
                emit ui->pushButton_stop->clicked();
                ui->pushButton_stop->setEnabled(false);
            }
        }
    });
    this->player->setVideoOutput(ui->mp4_player);
    this->http = new QNetworkAccessManager(this);
    this->http1 = new QNetworkAccessManager(this);
    //点击查询 发送请求
    connect(ui->pushButton_search, &QPushButton::clicked, this, &music::send_request);
    //请求完成 做相应处理
    connect(this->http, &QNetworkAccessManager::finished, this, &music::handle_info);
    total = "";
    this->song_url = new QString("");
    row = 0;
    //请求到歌曲url 处理歌曲url
    connect(this->http1, &QNetworkAccessManager::finished, this, [ = ](QNetworkReply * rep)mutable {
        int status2 =  rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(status2 != 200) {
            QMessageBox::critical(this, "请求", "歌曲请求出错");
            return;
        } else {
            QByteArray data2 = rep->readAll();
            QJsonDocument doc2 = QJsonDocument::fromJson(data2);
            if(doc2.isObject()) {
                QJsonObject obj2 = doc2.object();
                QJsonArray array2 = obj2["data"].toArray();
                QJsonObject data_song = array2[0].toObject();
                *song_url = data_song["url"].toString();
                song_info.insert(table->item(row, 0)->text(), *song_url);
                row++;
            }
        }
    });
}
music::~music() {
    delete ui;
}
void music::send_request() {
    QString ip = "http://iwxyi.com:3000/search?keywords=" +  ui->lineEdit_search->text();
    this->http->get(QNetworkRequest( QUrl(ip)) );
}
void music::handle_info(QNetworkReply *reply) {
    int status =  reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(status != 200) {
        QMessageBox::critical(this, "请求", "请求出错");
        return;
    } else {
        table->clearContents();
        table->setRowCount(0);
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
//        dia->setWindowTitle("搜索歌曲");
        if(doc.isObject()) {
            static int num = 0;
            if(num == 0) {
                connect(table, &QTableWidget::itemDoubleClicked, this, [ = ](QTableWidgetItem * item) {
                    auto it = song_info.find(item->text());
                    if(it != song_info.end()) {
                        QString value = it.value();
                        this->play_list->addMedia(QMediaContent(QUrl(value)));
                        QString key = it.key();
                        ui->listWidget_musiclist->addItems(QStringList() << key);
                        QFile file("../project/song.txt");
                        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
                            QMessageBox::critical(this, "保存文件", "保存歌曲信息失败");
                        } else {
                            int row = item->row();
                            // 获取该行的所有列数
                            int columnCount = table->columnCount();
                            // 创建一个容器来存储该行的所有数据
                            QList<QVariant> rowData;
                            // 遍历该行的每一列，获取每个单元格的数据
                            QString data = "";
                            for (int col = 0; col < columnCount; ++col) {
                                QTableWidgetItem *cellItem = table->item(row, col);
                                if(col != columnCount - 1) {
                                    data += cellItem->text() + "---";
                                } else {
                                    data += cellItem->text() + "\n";
                                }
                            }
                            file.write(data.toUtf8());
                            file.close();
                        }
                    } else {
                        QMessageBox::information(this, "搜索歌曲", "请点击歌名添加歌曲");
                        return;
                    }
                });
                num++;
            }
            QJsonObject obj = doc.object();
            obj = obj["result"].toObject();
            QJsonArray arry = obj["songs"].toArray();
            qDebug() << arry.size();
            for(int i = 0; i < arry.size() ; i++) {
                QJsonObject arr =  arry[i].toObject();
                QString name = arr["name"].toString();
                int size = arr["duration"].toInt();
                QJsonArray artists = arr["artists"].toArray();
                QString authors = "";
                for(int j = 0 ; j < artists.size() ; j++) {
                    QJsonObject author = artists[j].toObject();
                    authors += +" " + author["name"].toString();
                }
                QJsonObject album = arr["album"].toObject();
                //获取播放路径
                int id = arr["id"].toInt();
                QString play_path = "http://iwxyi.com:3000/song/url?id=" + QString::number( id);
                this->http1->get(QNetworkRequest(QUrl(play_path)));
                // 将 QTableWidgetItem 添加到表格中的相应位置
                table->insertRow(i);
                table->setItem(i, 0, new QTableWidgetItem(name));
                table->setItem(i, 1, new QTableWidgetItem(authors));
                table->setItem(i, 2, new QTableWidgetItem(QString::number(id)));
                QString len = QString::number( size / 1000);
                table->setItem(i, 3, new QTableWidgetItem(len));
            }
        }
        qDebug() << 310;
        table->show();
    }
}

void music::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::RightButton) {
        QFile file("../project/song_info.txt");
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QMessageBox::critical(this, "保存文件", "保存歌曲信息失败");
        } else {
            for (int i = 0; i < play_list->mediaCount(); ++i) {
                QMediaContent media = play_list->media(i);
                QUrl url = media.canonicalUrl();
                QString data =  ui->listWidget_musiclist->item(i)->text() + "——————" + url.toString() + "\n";
                file.write(data.toUtf8());
            }
            file.close();
        }
        mainpage *page = new mainpage;
        page->setAttribute(Qt::WA_DeleteOnClose);
        page->show();
        this->close();
    }
}
void music::read_file() {
    QFile file("../project/song_info.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "保存文件", "保存歌曲信息失败");
    } else {
        while (!file.atEnd()) {
            QByteArray data = file.readLine();
            QString   str   = data.data();
            QStringList parts = str.split("——————");
            this->play_list->addMedia(QMediaContent(QUrl( parts[1].trimmed())));
//            qDebug() <<  parts[1].trimmed();
            ui->listWidget_musiclist->addItems(QStringList() <<  parts[0]);
        }
        file.close();
    }
}

void music::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    // 创建一个绘图对象
    QPainter painter(this);
    // 绘制背景图片
    painter.drawPixmap(rect(), backgroundPixmap);
}
