TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    gtest_main.cpp

INCLUDEPATH += \
    /usr/include/gtest

LIBS    += \
    -L/usr/local/lib -lgtest \
    -L/usr/local/lib -lgtest_main \
    -lboost_system \
    -lboost_thread \
    -lpthread
