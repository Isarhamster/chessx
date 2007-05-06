######################################################################
# testrunner - project file
######################################################################

CONFIG += qtestlib debug
TEMPLATE = app
TARGET = testrunner
DEPENDPATH += .
INCLUDEPATH += . ../../src/database/
LIBS += ../../src/database/libdatabase.a

# Input
HEADERS += playerdatabasetest.h \
           pgndatabasetest.h \
           spellcheckertests.h \
           boardtest.h \
           databaseconversiontest.h
           
SOURCES += testrunner.cpp playerdatabasetest.cpp \
           pgndatabasetest.cpp \
           spellcheckertests.cpp \
           boardtest.cpp \
           databaseconversiontest.cpp
