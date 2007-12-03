# Database core library

HEADERS += board.h \
           move.h \
           common.h \
           game.h \
           partialdate.h \
           playerdatabase.h \
           playerdata.h \
           databaseconversion.h \
           tagvalues.h \
           taglist.h \
           stringtagvalues.h \
           engine.h \
           wbengine.h \
           uciengine.h \
           search.h \
           query.h \
           database.h \
           pgndatabase.h \
           memorydatabase.h \
           filter.h \
           tristatetree.h \
           nag.h \
           settings.h \
           spellchecker.h \
           historylist.h \
           output.h \
           outputoptions.h \
           databaseinfo.h \
           indexitem.h \
           index.h \
           filtermodel.h \
           tablebase.h \
           bitboard.h \
	   openingtree.h \
	   chessxdatabase.h \
           cxdmoves.h \
           cxdsaxhandler.h \
	   cxdindex.h \
	   cxdassign.h \
           cxdcfile.h \
           cxdcblock.h \
	   cxdflags.h \
 version.h \
 playerinfo.h
SOURCES += board.cpp \
           common.cpp \
           game.cpp \
           databaseconversion.cpp \
           partialdate.cpp \
           playerdatabase.cpp \
           playerdata.cpp \
           tagvalues.cpp \
           stringtagvalues.cpp \
           taglist.cpp \
           engine.cpp \
           wbengine.cpp \
           uciengine.cpp \
           search.cpp \
           query.cpp \
           database.cpp \
           pgndatabase.cpp \
           memorydatabase.cpp \
           filter.cpp \
           tristatetree.cpp \
           nag.cpp \
           settings.cpp \
           spellchecker.cpp \
           historylist.cpp \
           output.cpp \
           outputoptions.cpp \
           databaseinfo.cpp \
           indexitem.cpp \
           index.cpp \
           filtermodel.cpp \
           tablebase.cpp \
           bitboard.cpp \
	   openingtree.cpp \
	   chessxdatabase.cpp \
           cxdmoves.cpp \
           cxdsaxhandler.cpp \
	   cxdindex.cpp \
	   cxdassign.cpp \
           cxdcfile.cpp \
           cxdcblock.cpp \
           cxdflags.cpp \
 playerinfo.cpp

include(../common.pro)
TARGET = database
CONFIG += staticlib
TEMPLATE = lib
QT += xml

fastbits {
    DEFINES += FASTBITS
}
