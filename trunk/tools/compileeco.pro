
# Main application

CONFIG += release warn_on qt uic
QT += network xml

SOURCES += compileeco.cpp

TEMPLATE = app
INCLUDEPATH += ../src/database
INCLUDEPATH += ../src/guess/

TARGET=compileeco
LIBS += ../src/database/libdatabase.a
LIBS += ../src/guess/libguess.a
PRE_TARGETDEPS += ../src/database/libdatabase.a

