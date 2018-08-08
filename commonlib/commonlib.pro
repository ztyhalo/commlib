#-------------------------------------------------
#
# Project created by QtCreator 2018-05-21T13:31:10
#
#-------------------------------------------------

QT       -= gui
QT       += xml

TARGET = commonlib
TEMPLATE = lib
#CONFIG += staticlib

INCLUDEPATH = .\
            epoll\
            prodata\
            prodata/sem\
            reflect\
            ztimer\
            zprint\
            mutex\
            sigslot

SOURCES += \
    prodata/sem/syssem.cpp \
    zprint/zprint.cpp \
    ztimer/ztimers.cpp

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
    zprint/version.h \
    zprint/zprint.h \
    mutex/mutex.h \
    sigslot/sigslot.h \
    prodata/zmsg.h \
    sempro/semprocess.h \
    ztimer/zevent.h \
    ztimer/ztimers.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
