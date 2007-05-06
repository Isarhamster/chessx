
# Main application

CONFIG += release warn_on qt uic
QT += network xml

SOURCES += compileeco.cpp

TEMPLATE = app
INCLUDEPATH += ../src/database

TARGET=compileeco
LIBS += ../src/database/libdatabase.a
PRE_TARGETDEPS += ../src/database/libdatabase.a

