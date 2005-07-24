# Fil oprettet af Kdevelops qmake-håndtering. 
# ------------------------------------------- 
# Delkatalog relativt til projektets hovedkatalog: ./src/database
# Målet er et bibliotek:  database

HEADERS += board.h \
           move.h \
           common.h \
           game.h \
           pgnparser.h \
           playerdatabase.h \
           playerdata.h \
           databaseconversion.h 
SOURCES += board.cpp \
           move.cpp \
           databaseconversion.cpp \
           pgnparser.cpp \
           playerdatabase.cpp \
           playerdata.cpp 
TARGET = database
CONFIG += release warn_on qt staticlib
TEMPLATE = lib
QT += qt3support
INCLUDEPATH += ../compatibility
QT34 = ../compatibility/qt34
