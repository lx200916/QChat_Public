#-------------------------------------------------
#
# Project created by QtCreator 2019-07-12T14:41:25
#
#-------------------------------------------------

QT       += core gui
QT       += network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qchat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        addfriends.cpp \
    bbc.cpp \
    choose.cpp \
        dialog.cpp \
        externa.cpp \
    gmem.cpp \
    hello.cpp \
    item.cpp \
        main.cpp \
        mainwindow.cpp \
chatmessage/qnchatmessage.cpp \
        loginwindow.cpp \
    transfer.cpp \
        widget.cpp

HEADERS += \
        addfriends.h \
    bbc.h \
    choose.h \
        dialog.h \
        externa.h \
    gmem.h \
    hello.h \
    item.h \
        mainwindow.h \
chatmessage/qnchatmessage.h \
        loginwindow.h \
    transfer.h \
        widget.h

FORMS += \
        addfriends.ui \
        bbc.ui \
        choose.ui \
        dialog.ui \
        gmem.ui \
        hello.ui \
        item.ui \
        mainwindow.ui \
        loginwindow.ui \
        transfer.ui \
        widget.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=\
    img.qrc
INCLUDEPATH += C:/OpenSSL-Win64/include

DISTFILES +=
