TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    ../src/simulator/interchange/broadcaster.cpp \
    ../src/simulator/interchange/subscription.cpp \
    ../src/simulator/physics/algebra.cpp \
    ../src/simulator/physics/physics.cpp \
    ../src/simulator/scene/director.cpp \
    ../src/simulator/scene/scene.cpp \
    ../src/simulator/utils/iterative.cpp

HEADERS += \
    algebra_test.h \
    concepts_test.h \
    scene_test.h

INCLUDEPATH += \
    /usr/include/gtest \
    ../src/simulator

LIBS    += \
    -L/usr/local/lib -lgtest \
    -L/usr/local/lib -lgtest_main \
    -lboost_system \
    -lboost_thread \
    -lboost_chrono \
    -lpthread
