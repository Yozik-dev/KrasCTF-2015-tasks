#-------------------------------------------------
#
# Project created by QtCreator 2015-09-20T15:49:25
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = pcc300
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    data_extractor.cpp \
    utils.cpp \
    sqlite/sqlite3.c

HEADERS += \
    data_extractor.h
