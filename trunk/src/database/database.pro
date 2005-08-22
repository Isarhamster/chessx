# Fil oprettet af Kdevelops qmake-håndtering. 
# ------------------------------------------- 
# Delkatalog relativt til projektets hovedkatalog: ./src/database
# Målet er et bibliotek:  database

HEADERS += board.h \
           move.h \
           common.h \
           capture.h \
           game.h \
           partialdate.h \
           playerdatabase.h \
           playerdata.h \
           databaseconversion.h \
           tags.h 
SOURCES += board.cpp \
           capture.cpp \
           move.cpp \
           databaseconversion.cpp \
           partialdate.cpp \
           playerdatabase.cpp \
           playerdata.cpp \
           tags.cpp 
TARGET = database
CONFIG += release warn_on qt staticlib
TEMPLATE = lib
QT += qt3support
INCLUDEPATH += ../compatibility
