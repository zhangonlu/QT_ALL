#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainpage.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("登录界面");
    connect(ui->pushButton_register, &QPushButton::clicked, this, [ = ]() {
        QDialog *dia = new QDialog();
        dia->setAttribute(Qt::WA_DeleteOnClose);
        dia->setFixedSize(500, 500);
        dia->setWindowTitle("注册");
        QLabel *img = new QLabel(dia);
        img->setPixmap(QPixmap(":/new/prefix1/page_res/016b3e5b14056ea801202e60cd9a80.png@1280w_1l_2o_100sh.png"));
        img->setScaledContents(true);
        img->setFixedSize(150, 150);
        QLabel *label_phone = new QLabel("电话号码", dia);
        QLineEdit *phone = new QLineEdit(dia);
        QLabel *label_pwd = new QLabel("密码", dia);
        QLineEdit *pwd = new QLineEdit(dia);
        QLabel *label_sure_pwd = new QLabel("确认密码", dia);
        QLineEdit *sure_pwd = new QLineEdit(dia);
        sure_pwd->setEchoMode(QLineEdit::Password);
        pwd->setEchoMode(QLineEdit::Password);
        QPushButton *pushbutton_submit = new QPushButton("注册", dia);
        QPushButton *pushbutton_exit = new QPushButton("返回登录", dia);
        QGridLayout *gri = new QGridLayout;
        gri->addItem(new QSpacerItem(50, 0), 0, 0);
        gri->addWidget(label_phone, 0, 1);
        gri->addWidget(phone, 0, 2);
        gri->addItem(new QSpacerItem(50, 0), 0, 3);
        gri->addWidget(label_pwd, 1, 1);
        gri->addWidget(pwd, 1, 2);
        gri->addItem(new QSpacerItem(50, 0), 1, 0);
        gri->addItem(new QSpacerItem(50, 0), 1, 3);
        gri->addItem(new QSpacerItem(50, 0), 2, 0);
        gri->addWidget(label_sure_pwd, 2, 1);
        gri->addWidget(sure_pwd, 2, 2);
        gri->addItem(new QSpacerItem(50, 0), 2, 3);
        QHBoxLayout *hb1 = new QHBoxLayout;
        hb1->addWidget(img);
        QHBoxLayout *hb = new QHBoxLayout;
        hb->addWidget(pushbutton_submit);
        hb->addWidget(pushbutton_exit);
        QVBoxLayout *vb = new QVBoxLayout;
        vb->addLayout(hb1);
        vb->addLayout(gri);
        vb->addLayout(hb);
        dia->setLayout(vb);
        QString account = randomgenerator();
        qDebug() << account;
        //返回登录
        connect(pushbutton_exit, &QPushButton::clicked, dia, [ = ]() {
            dia->hide();
            this->show();
        });
        //提交注册信息
        connect(pushbutton_submit, &QPushButton::clicked, this, [ = ]() mutable{
            QFile file("../project/user_info.txt");
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::question(this, "打开文件", "打开文件失败");
                return;
            }
            int flag = 0;
            while (!file.atEnd()) {
                QByteArray line = file.readLine();
                qDebug() << line;
                QString data = line.data();
                QStringList str = data.split(" ");
                if(str[2].trimmed() == phone->text()) {
                    QMessageBox::information(this, "注册", "该手机号码已经被注册");
                    return;
                }
                if(account == line.data()) {
                    account = randomgenerator();
                    file.seek(0);
                }
            }
            file.close();
            //写入文件
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
                QMessageBox::question(this, "打开文件", "打开文件失败");
                return;
            }
            if(pwd->text().size() < 6 || pwd->text().size() > 18) {
                QMessageBox::question(this, "密码格式不正确", "密码应为6~18位");
                return;
            }
            if(pwd->text() != sure_pwd->text()) {
                QMessageBox::question(this, "密码不一致", "两次输入密码不一致");
                return;
            }
            if(phone->text().size() != 11) {
                QMessageBox::question(this, "手机号格式不正确", "手机号格式错误");
                return;
            }


            QString user = account + " " + pwd->text() + " " + phone->text() + "\n";
            QByteArray data = user.toUtf8();
            file.write(data);
            QString user_info = "账号：" + account + "\n密码：" + pwd->text();
            QMessageBox::information(this, "注册成功", "您的账号信息如下：\n" + user_info);
            account = "";
            file.close();
        });
        this->hide();
        dia->show();
    });
    //登录按钮
    connect(ui->pushButton_login, &QPushButton::clicked, this, [ = ]() {
        QString login_username = ui->lineEdit_username->text();
        QString login_passwd = ui->lineEdit_passwd->text();
        QFile file("../project/user_info.txt");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::question(this, "打开文件", "打开文件失败");
            return;
        }
        while (!file.atEnd()) {
            QByteArray data = file.readLine();
            int index = data.indexOf(" ");
            QByteArray username = data.mid(0, index);
            int second_index = data.indexOf(" ", index + 1);
            QByteArray passwd = data.mid(index + 1, second_index - index - 1);
            if(login_username == username.data() && login_passwd == passwd.data()) {
                QMessageBox::information(this, "登录", "登录成功");
                file.close();
                this->hide();
                mainpage *page = new mainpage;
                page->setAttribute(Qt::WA_DeleteOnClose);
                page->show();
//                weather *wea = new weather(this);
//                wea->setAttribute(Qt::WA_DeleteOnClose);
//                wea->show();
            } else {
                QMessageBox::information(this, "登录", "登录失败");
                file.close();
                return;
            }
        }
    });
    connect(ui->pushButton_resetpwd, &QPushButton::clicked, this, [ = ]() {
        QDialog *dia = new QDialog();
        dia->setAttribute(Qt::WA_DeleteOnClose);
        dia->setFixedSize(500, 500);
        dia->setWindowTitle("忘记密码");
        QLabel *img = new QLabel(dia);
        img->setPixmap(QPixmap(":/new/prefix1/page_res/016b3e5b14056ea801202e60cd9a80.png@1280w_1l_2o_100sh.png"));
        img->setScaledContents(true);
        img->setFixedSize(150, 150);
        QLabel *label_phone = new QLabel("电话号码", dia);
        QLineEdit *phone = new QLineEdit(dia);
        QLabel *label_user = new QLabel("账号", dia);
        QLineEdit *user = new QLineEdit(dia);
        QLabel *label_new_pwd = new QLabel("新密码", dia);
        QLineEdit *new_pwd = new QLineEdit(dia);
        new_pwd->setEchoMode(QLineEdit::Password);
        QPushButton *pushbutton_submit = new QPushButton("提交", dia);
        QPushButton *pushbutton_exit = new QPushButton("返回登录", dia);
        QGridLayout *gri = new QGridLayout;
        gri->addItem(new QSpacerItem(50, 0), 0, 0);
        gri->addWidget(label_phone, 0, 1);
        gri->addWidget(phone, 0, 2);
        gri->addItem(new QSpacerItem(50, 0), 0, 3);
        gri->addWidget(label_user, 1, 1);
        gri->addWidget(user, 1, 2);
        gri->addItem(new QSpacerItem(50, 0), 1, 0);
        gri->addItem(new QSpacerItem(50, 0), 1, 3);
        gri->addItem(new QSpacerItem(50, 0), 2, 0);
        gri->addWidget(label_new_pwd, 2, 1);
        gri->addWidget(new_pwd, 2, 2);
        gri->addItem(new QSpacerItem(50, 0), 2, 3);
        QHBoxLayout *hb1 = new QHBoxLayout;
        hb1->addWidget(img);
        QHBoxLayout *hb = new QHBoxLayout;
        hb->addWidget(pushbutton_submit);
        hb->addWidget(pushbutton_exit);
        QVBoxLayout *vb = new QVBoxLayout;
        vb->addLayout(hb1);
        vb->addLayout(gri);
        vb->addLayout(hb);
        dia->setLayout(vb);
        connect(pushbutton_exit, &QPushButton::clicked, this, [ = ]() {
            dia->hide();
            this->show();
        });
        connect(pushbutton_submit, &QPushButton::clicked, this, [ = ]() {
            QString filename = "../project/user_info.txt";
            QFile file(filename);
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::question(this, "打开文件", "打开文件失败");
                return;
            }
            int row = 0;
            bool flag = false;
            while (!file.atEnd()) {
                row++;
                QByteArray line = file.readLine();
                int index = line.indexOf(" ");
                int index_second = line.indexOf(" ", index + 1);
                int index_last = line.lastIndexOf("\n");
                QByteArray username = line.mid(0, index);
                QByteArray phone_number = line.mid(index_second + 1, index_last - index_second - 1);
                if(username.data() == user->text() && phone_number.data() == phone->text()) {
                    flag = true;
                    break;
                }
            }
            if(flag == true) {
                QString user_info =  user->text() + " " + new_pwd->text() + " " + phone->text() + "\n";
                replaceLine(file, row, user_info);
                QMessageBox::information(this, "修改密码", "修改密码成功");
            }
        });
        this->hide();
        dia->show();
    });
}

MainWindow::~MainWindow() {
    delete ui;
}




QString randomgenerator() {
    //获取当前时间戳
    qint64 time = QDateTime::currentMSecsSinceEpoch();
    //将时间戳转为字符串 只要后六位
    QString timestring =  QString::number(time).right(6);
    QString rand_num;
    for(int i = 0; i < 4; i++) {
        rand_num += QString::number(qrand() % 10);
    }
    QString account = rand_num + timestring ;
    return account;
}


//替换文件指定行
void replaceLine(QFile &file, int lineNumber, const QString &newLine) {
    // 读取文件内容
    file.seek(0);
    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        lines.append(line);
    }
    file.close();
    // 替换指定行
    if (lineNumber < 1 || lineNumber > lines.size()) {
        qDebug() << "Invalid line number";
        return;
    }
    lines.replace(lineNumber - 1, newLine);
    // 打开文件以写入内容
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing";
        return;
    }
    // 将修改后的内容写入文件
    QTextStream out(&file);
    foreach (const QString &line, lines) {
        out << line ;
    }
    file.close();
    qDebug() << "Line replaced successfully";
}
