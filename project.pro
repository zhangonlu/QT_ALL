#-------------------------------------------------
#
# Project created by QtCreator 2024-04-03T09:04:08
#
#-------------------------------------------------

QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += resources_big
TARGET = project
TEMPLATE = app
QT += multimedia
QT += multimediawidgets
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    weather.cpp \
    music.cpp \
    mainpage.cpp \
    mylabel.cpp \
    photo.cpp \
    wise_fa.cpp \
    wise_server.cpp \
    wise_client.cpp

HEADERS += \
        mainwindow.h \
    weather.h \
    music.h \
    mainpage.h \
    mylabel.h \
    photo.h \
    wise_fa.h \
    wise_server.h \
    wise_client.h

FORMS += \
    mainwindow.ui \
    weather.ui \
    music.ui \
    mainpage.ui \
    mylabel.ui \
    photo.ui \
    wise_fa.ui \
    wise_server.ui \
    wise_client.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    user_info.txt \
    song_info.txt \
    song.txt
