# Fil oprettet af Kdevelops qmake-h�ndtering. 
# ------------------------------------------- 
# Delkatalog relativt til projektets hovedkatalog: ./src/database
# M�let er et bibliotek:  database

HEADERS += board.h \
           history.h \
           move.h \
           common.h \
           history.h \
           game.h \
           partialdate.h \
           playerdatabase.h \
           playerdata.h \
           databaseconversion.h \
           tags.h 
SOURCES += board.cpp \
           history.cpp \
           move.cpp \
           common.cpp \
           history.cpp \
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
