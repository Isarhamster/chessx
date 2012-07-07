TEMPLATE = app
QT += network xml
CONFIG = qt warn_on
TARGET = test

macx {
        QMAKE_CXXFLAGS += -fvisibility=hidden
}

SOURCES += main.cpp \
    ../../src/database/wbengine.cpp \
    ../../src/database/uciengine.cpp \
    ../../src/database/tristatetree.cpp \
    ../../src/database/tagvalues.cpp \
    ../../src/database/tags.cpp \
    ../../src/database/taglist.cpp \
    ../../src/database/tablebase.cpp \
    ../../src/database/stringtagvalues.cpp \
    ../../src/database/spellchecker.cpp \
    ../../src/database/settings.cpp \
    ../../src/database/search.cpp \
    ../../src/database/query.cpp \
    ../../src/database/playerlistmodel.cpp \
    ../../src/database/playerinfo.cpp \
    ../../src/database/playerdatabase.cpp \
    ../../src/database/playerdata.cpp \
    ../../src/database/pgndatabase.cpp \
    ../../src/database/partialdate.cpp \
    ../../src/database/outputoptions.cpp \
    ../../src/database/output.cpp \
    ../../src/database/openingtree.cpp \
    ../../src/database/nag.cpp \
    ../../src/database/memorydatabase.cpp \
    ../../src/database/indexitem.cpp \
    ../../src/database/index.cpp \
    ../../src/database/historylist.cpp \
    ../../src/database/game.cpp \
    ../../src/database/filtermodel.cpp \
    ../../src/database/filter.cpp \
    ../../src/database/enginelist.cpp \
    ../../src/database/enginedata.cpp \
    ../../src/database/engine.cpp \
    ../../src/database/editaction.cpp \
    ../../src/database/databaseinfo.cpp \
    ../../src/database/databaseconversion.cpp \
    ../../src/database/database.cpp \
    ../../src/database/common.cpp \
    ../../src/database/board.cpp \
    ../../src/database/bitboard.cpp \
    ../../src/database/analysis.cpp


HEADERS += ../../src/database/board.h \
        ../../src/database/move.h \
        ../../src/database/common.h \
        ../../src/database/game.h \
        ../../src/database/partialdate.h \
        ../../src/database/playerlistmodel.h \
        ../../src/database/playerdatabase.h \
        ../../src/database/playerdata.h \
        ../../src/database/databaseconversion.h \
        ../../src/database/tagvalues.h \
        ../../src/database/taglist.h \
        ../../src/database/stringtagvalues.h \
        ../../src/database/engine.h \
        ../../src/database/wbengine.h \
        ../../src/database/uciengine.h \
        ../../src/database/search.h \
        ../../src/database/query.h \
        ../../src/database/database.h \
        ../../src/database/pgndatabase.h \
        ../../src/database/memorydatabase.h \
        ../../src/database/filter.h \
        ../../src/database/tristatetree.h \
        ../../src/database/nag.h \
        ../../src/database/settings.h \
        ../../src/database/spellchecker.h \
        ../../src/database/historylist.h \
        ../../src/database/output.h \
        ../../src/database/outputoptions.h \
        ../../src/database/databaseinfo.h \
        ../../src/database/indexitem.h \
        ../../src/database/index.h \
        ../../src/database/filtermodel.h \
        ../../src/database/tablebase.h \
        ../../src/database/bitboard.h \
        ../../src/database/openingtree.h \
        ../../src/database/version.h \
        ../../src/database/playerinfo.h \
        ../../src/database/enginelist.h \
        ../../src/database/enginedata.h \
        ../../src/database/analysis.h \
        ../../src/database/editaction.h

UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated

OBJECTS_DIR = objects

TEMPLATE = app
INCLUDEPATH += ../../src/database \
               ../../src/compatibility 
							 
