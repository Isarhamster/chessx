# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterverzeichnis relativ zum Projektverzeichnis: ./tests/cppunit
# Das Target ist eine Anwendung:  test_runner

TARGETDEPS += ../../src/database/libdatabase.a 
LIBS += ../../src/database/libdatabase.a \
        -lcppunit 
INCLUDEPATH += ../../src/database \
               /usr/lib/qt3/include \
               . \
               spelling \
               ../../src/compatibility 
TARGET = test_runner 
CONFIG += release \
          warn_on \
          qt 
TEMPLATE = app 
HEADERS += playerdatabasetest.h \
           spelling/spellcheckertests.h \
           databaseconversiontest.h \
           playerdatabasechangestest.h \
           boardtest.h \
           querytest.h \
           pgndatabasetest.h 
SOURCES += playerdatabasetest.cpp \
           test_runner.cpp \
           spelling/spellcheckertests.cpp \
           databaseconversiontest.cpp \
           playerdatabasechangestest.cpp \
           boardtest.cpp \
           querytest.cpp \
           pgndatabasetest.cpp 
