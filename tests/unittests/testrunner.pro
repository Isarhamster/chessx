######################################################################
# testrunner - project file
######################################################################

CONFIG += qtestlib debug
TEMPLATE = app
TARGET = testrunner
DEPENDPATH += .
INCLUDEPATH += . ../../src/database/
LIBS += ../../src/database/libdatabase.a
TARGETDEPS += ../../src/database/libdatabase.a

# Input
HEADERS += playerdatabasetest.h \
           pgndatabasetest.h \
           spellcheckertests.h \
           boardtest.h \
           databaseconversiontest.h \
           stringtagvaluestest.h \
           taglisttest.h \
           indexitemtest.h \
           indextest.h \
           filtertest.h

SOURCES += testrunner.cpp playerdatabasetest.cpp \
           pgndatabasetest.cpp \
           spellcheckertests.cpp \
           boardtest.cpp \
           databaseconversiontest.cpp \
           stringtagvaluestest.cpp \
           taglisttest.cpp \
           indexitemtest.cpp \
           indextest.cpp \
           filtertest.cpp
