QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = TSP_Project
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    graph.cpp \
    tsp_solver.cpp \
    visualization.cpp

HEADERS += \
    mainwindow.h \
    graph.h \
    tsp_solver.h \
    visualization.h \
    stack.h \
    queue.h

FORMS += \
    mainwindow.ui
