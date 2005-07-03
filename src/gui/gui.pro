SOURCES += mainwindow.cpp
HEADERS += mainwindow.h

TARGET = gui
CONFIG += release warn_on qt  staticlib 
TEMPLATE = lib 

# for qt3/4 compatibility
QT += qt3support

INCLUDEPATH += ../compatibility
QT34 = ../compatibility/qt34

HEADERS += $$QT34/qmainwindow.h \
           $$QT34/qpopupmenu.h
