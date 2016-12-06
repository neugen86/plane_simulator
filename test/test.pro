TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    ../src/physics/algebra.cpp \
    ../src/physics/concepts.cpp \
    ../src/scene/scene.cpp \
    ../src/utils/iterative.cpp


HEADERS += \
    algebra_test.h \
    concepts_test.h \
    scene_test.h \
    ../src/physics/algebra.h \
    ../src/physics/concepts.h \
    ../src/physics/constants.h \
    ../src/scene/scene.h \
    ../src/utils/iterative.h \
    ../src/utils/types.h \

INCLUDEPATH += \
    /usr/include/gtest \
    ../src

LIBS    += \
    -L/usr/local/lib -lgtest \
    -L/usr/local/lib -lgtest_main \
    -lboost_system \
    -lboost_thread \
    -lboost_chrono \
    -lpthread
