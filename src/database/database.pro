# Plik utworzony przez menad?era projekt� kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?�nego katalogu projektu: ./src/database
# Cel to biblioteka database

HEADERS += board.h \
           move.h \
           common.h \
           capture.h \
           game.h \
           partialdate.h \
#           pgnparser.h \
           playerdatabase.h \
           playerdata.h \
           databaseconversion.h
SOURCES += board.cpp \
           capture.cpp \
           move.cpp \
           databaseconversion.cpp \
           partialdate.cpp \
#           pgnparser.cpp \
           playerdatabase.cpp \
           playerdata.cpp 
TARGET = database
CONFIG += release warn_on qt staticlib
TEMPLATE = lib
QT += qt3support
INCLUDEPATH += ../compatibility
