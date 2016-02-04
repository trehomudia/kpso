#-------------------------------------------------
#
# Project created by QtCreator 2015-08-17T22:37:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = statistica
TEMPLATE = app


SOURCES += main.cpp\
        hockey_manager.cpp\
        storage.cpp\
        table_view.cpp\
        team.cpp


HEADERS  += hockey_manager.h\
            table_view.h\
            storage.h\
            team.h

FORMS    += hockey_manager.ui\
            championat_widget.ui
