# Plik utworzony przez mened?era projekt�w kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?�wnego katalogu projektu: ./src/database
# Cel to biblioteka database

HEADERS += board.h \
           move.h \
           common.h \
           history.h \
           game.h \
           partialdate.h \
           playerdatabase.h \
           playerdata.h \
           databaseconversion.h \
           tags.h \
           engine.h \
           wbengine.h \
           uciengine.h \
           search.h \
           query.h \
           database.h \
           pgndatabase.h \
           filter.h \
           tristatetree.h \
           nag.h \
           settings.h \
           spellchecker.h \
           historylist.h 
SOURCES += board.cpp \
           move.cpp \
           common.cpp \
           history.cpp \
           game.cpp \
           databaseconversion.cpp \
           partialdate.cpp \
           playerdatabase.cpp \
           playerdata.cpp \
           tags.cpp \
           engine.cpp \
           wbengine.cpp \
           uciengine.cpp \
           search.cpp \
           query.cpp \
           database.cpp \
           pgndatabase.cpp \
           filter.cpp \
           tristatetree.cpp \
           nag.cpp \
           settings.cpp \
           spellchecker.cpp \
           historylist.cpp 
TARGET = database
CONFIG += release warn_on qt staticlib
TEMPLATE = lib
QT += qt3support
INCLUDEPATH += ../compatibility
