TEMPLATE = app
QT +=
CONFIG = qt warn_on
TARGET = test

macx {
        QMAKE_CXXFLAGS += -fvisibility=hidden
}

SOURCES += main.cpp \
    ../../src/database/playerdatabase.cpp \
    ../../src/database/playerdata.cpp \
    ../../src/database/partialdate.cpp

HEADERS += ../../src/database/partialdate.h \
        ../../src/database/playerdatabase.h \
        ../../src/database/playerdata.h \
        ../../src/database/databaseconversion.h

UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated

OBJECTS_DIR = objects

INCLUDEPATH += ../../src/database
							 
