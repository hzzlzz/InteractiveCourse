#-------------------------------------------------
#
# Project created by QtCreator 2013-04-04T15:14:51
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InteractiveCourse
TEMPLATE = app

CONFIG += qwt

SOURCES += main.cpp\
    mapper.cpp \
    parser.cpp \
    handler.cpp \
    icquestion.cpp \
    icanswer.cpp \
    icmessageparser.cpp \
    icmessagemapper.cpp \
    icmessagehandler.cpp \
    icserver.cpp \
    icmessage.cpp \
    icsystemstore.cpp \
    icmessageprocessor.cpp \
    icresultchart.cpp \
    icresultwindow.cpp \
    icstartdialog.cpp \
    icmainwindow.cpp \
    icnewquestiondialog.cpp

HEADERS  += \
    mapper.h \
    source.h \
    parser.h \
    handler.h \
    icquestion.h \
    icanswer.h \
    icmessageparser.h \
    icmessagemapper.h \
    icmessagehandler.h \
    icserver.h \
    icmessage.h \
    icsystemstore.h \
    icconfig.h \
    icmessageprocessor.h \
    icresultchart.h \
    icresultwindow.h \
    icstartdialog.h \
    icmainwindow.h \
    icnewquestiondialog.h

FORMS    += \
    icstartdialog.ui \
    icmainwindow.ui \
    icnewquestiondialog.ui
