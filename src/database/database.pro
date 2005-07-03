HEADERS += board.h \
           move.h \
	   common.h
SOURCES += board.cpp \
           move.cpp 

TARGET = database 
CONFIG += release warn_on qt staticlib 
TEMPLATE = lib 

# for qt3/4 compatibility
QT += qt3support

INCLUDEPATH += ../compatibility
QT34 = ../compatibility/qt34
