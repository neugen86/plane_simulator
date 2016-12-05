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
    interchange/broadcaster.cpp \
    interchange/subscription.cpp \
    physics/algebra.cpp \
    physics/concepts.cpp \
    scene/scene.cpp \
    scene/director.cpp \
    utils/iterative.cpp

HEADERS  += \
    mainwindow.h \
    interchange/broadcaster.h \
    interchange/subscription.h \
    physics/algebra.h \
    physics/concepts.h \
    physics/constants.h \
    scene/scene.h \
    scene/director.h \
    utils/iterative.h \
    utils/types.h

FORMS    += \
    mainwindow.ui

LIBS    += \
    -lboost_system \
    -lboost_thread
