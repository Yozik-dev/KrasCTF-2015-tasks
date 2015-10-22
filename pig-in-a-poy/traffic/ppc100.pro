#-------------------------------------------------
#
# Project created by QtCreator 2015-08-28T14:40:04
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = ppc100
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp

HEADERS += \
    server.h

OTHER_FILES += \
    proto.txt
