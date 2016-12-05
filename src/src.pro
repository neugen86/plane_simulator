#-------------------------------------------------
#
# Project created by QtCreator 2016-12-05T13:40:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = src
TEMPLATE = app
CONFIG += c++11


SOURCES += \
    main.cpp\
    mainwindow.cpp \
    physics/algebra.cpp \
    physics/concepts.cpp \
    physics/scene.cpp \
    utils/iterative.cpp

HEADERS  += \
    mainwindow.h \
    physics/algebra.h \
    physics/concepts.h \
    physics/constants.h \
    physics/scene.h \
    physics/types.h \
    utils/iterative.h

FORMS    += \
    mainwindow.ui

LIBS    += \
    -lboost_system \
    -lboost_thread
