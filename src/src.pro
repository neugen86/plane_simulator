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
    ui/mainwindow.cpp \
    ui/interchange/subscriber.cpp \
    simulator/interchange/broadcaster.cpp \
    simulator/interchange/subscription.cpp \
    simulator/physics/algebra.cpp \
    simulator/physics/physics.cpp \
    simulator/scene/interface/periodic.cpp \
    simulator/scene/interface/playable.cpp \
    simulator/scene/logic.cpp \
    simulator/scene/scene.cpp


HEADERS  += \
    ui/mainwindow.h \
    ui/interchange/subscriber.h \
    simulator/concurrent/event.h \
    simulator/concurrent/lock.h \
    simulator/interchange/broadcaster.h \
    simulator/interchange/subscription.h \
    simulator/physics/algebra.h \
    simulator/physics/constants.h \
    simulator/physics/physics.h \
    simulator/physics/types.h \
    simulator/scene/interface/container.h \
    simulator/scene/interface/controllable.h \
    simulator/scene/interface/periodic.h \
    simulator/scene/interface/playable.h \
    simulator/scene/interface/with_gravity.h \
    simulator/scene/logic.h \
    simulator/scene/scene.h

FORMS    += \
    ui/mainwindow.ui

INCLUDEPATH += \
    simulator \
    ui

LIBS    += \
    -lboost_system \
    -lboost_thread \
    -lboost_chrono
