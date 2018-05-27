#-------------------------------------------------
#
# Project created by QtCreator 2018-05-23T16:02:01
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = libtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH +=$$PWD/../commonlib\
            $$PWD/../commonlib/epoll\
            $$PWD/../commonlib/prodata\
            $$PWD/../commonlib/prodata/sem\
            $$PWD/../commonlib/reflect\
            $$PWD/../commonlib/timer\
            $$PWD/../commonlib/zprint\
            $$PWD/../commonlib/mutex

LIBS += $$PWD/../../build-commonlib-unknown-Debug/libcommonlib.a

SOURCES += main.cpp
