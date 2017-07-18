#-------------------------------------------------
#
# Project created by QtCreator 2017-07-04T04:40:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenCK
TEMPLATE = app

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
    ui/mainwindow.cpp \
    ui/datawindow.cpp \
    define/form.cpp \
    define/tes4form.cpp \
    parse/parser.cpp \
    parse/readfile.cpp

HEADERS += \
    ui/mainwindow.h \
    ui/datawindow.h \
    define/form.h \
    define/tes4form.h \
    parse/parser.h \
    parse/readfile.h

FORMS += \
    ui/mainwindow.ui \
    ui/datawindow.ui

RESOURCES += \
    resources/resources.qrc

INCLUDEPATH += \
    ui \
    parse \
    define \
