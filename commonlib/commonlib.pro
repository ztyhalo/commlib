#-------------------------------------------------
#
# Project created by QtCreator 2018-05-21T13:31:10
#
#-------------------------------------------------

QT       -= gui
QT       += xml

TARGET = commonlib
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH = .\
            epoll\
            prodata\
            prodata/sem\
            reflect\
            timer\
            zprint\
            mutex

SOURCES += \
    prodata/sem/syssem.cpp \
    timer/timers.cpp \
    zprint/zprint.cpp

HEADERS += \
    epoll/e_poll.h \
    prodata/sem/syssem.h \
    prodata/clist.h \
    prodata/pro_data.h \
    prodata/ptdataapp.h \
    prodata/semshare.h \
    prodata/sharemem.h \
    prodata/zmap.h \
    ptxml/ptxml.h \
    reflect/reflect.h \
    reflect/xmlprocess.h \
    timer/timers.h \
    zprint/version.h \
    zprint/zprint.h \
    mutex/mutex.h \
    sigslot/sigslot.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
