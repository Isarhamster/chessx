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
HEADERS += ../../src/database/*.h playerdatabasetest.h \
           pgndatabasetest.h \
           spellcheckertests.h
SOURCES += testrunner.cpp playerdatabasetest.cpp \
           pgndatabasetest.cpp \
           spellcheckertests.cpp
