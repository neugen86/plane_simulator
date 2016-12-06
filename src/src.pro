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
    gui/mainwindow.cpp \
    simulator/interchange/broadcaster.cpp \
    simulator/interchange/subscription.cpp \
    simulator/physics/algebra.cpp \
    simulator/physics/physics.cpp \
    simulator/scene/scene.cpp \
    simulator/scene/director.cpp \
    simulator/utils/iterative.cpp

HEADERS  += \
    gui/mainwindow.h \
    simulator/interchange/broadcaster.h \
    simulator/interchange/subscription.h \
    simulator/physics/algebra.h \
    simulator/physics/constants.h \
    simulator/physics/physics.h \
    simulator/scene/scene.h \
    simulator/scene/director.h \
    simulator/utils/iterative.h \
    simulator/utils/types.h

FORMS    += \
    gui/mainwindow.ui

INCLUDEPATH += \
    simulator \
    gui

LIBS    += \
    -lboost_system \
    -lboost_thread \
    -lboost_chrono
