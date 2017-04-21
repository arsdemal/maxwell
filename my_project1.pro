#-------------------------------------------------
#
# Project created by QtCreator 2015-04-22T23:53:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = my_project1
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    graph_scene.cpp \
    post.cpp \
    molecule.cpp \
    maxwell_scene.cpp \
    trap_of_maxvell_scene.cpp

HEADERS  += widget.h \
    graph_scene.h \
    post.h \
    molecule.h \
    maxvell_scene.h \
    trap_of_maxvell_scene.h

FORMS    += widget.ui


