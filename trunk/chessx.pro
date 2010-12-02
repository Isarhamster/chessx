# Main application
fastbits:DEFINES += FASTBITS
CONFIG += uic warn_on
QT += xml network

FORMS += src/gui/playerdialog.ui \
	src/gui/preferences.ui \
	src/gui/boardsetup.ui \
	src/gui/quicksearch.ui \
	src/gui/savedialog.ui \
	src/gui/analysiswidget.ui \
	src/gui/copydialog.ui \
	src/gui/commentdialog.ui

HEADERS += src/database/board.h \
	src/database/move.h \
	src/database/common.h \
	src/database/game.h \
	src/database/partialdate.h \
	src/database/playerdatabase.h \
	src/database/playerdata.h \
	src/database/databaseconversion.h \
	src/database/tagvalues.h \
	src/database/taglist.h \
	src/database/stringtagvalues.h \
	src/database/engine.h \
	src/database/wbengine.h \
	src/database/uciengine.h \
	src/database/search.h \
	src/database/query.h \
	src/database/database.h \
	src/database/pgndatabase.h \
	src/database/memorydatabase.h \
	src/database/filter.h \
	src/database/tristatetree.h \
	src/database/nag.h \
	src/database/settings.h \
	src/database/spellchecker.h \
	src/database/historylist.h \
	src/database/output.h \
	src/database/outputoptions.h \
	src/database/databaseinfo.h \
	src/database/indexitem.h \
	src/database/index.h \
	src/database/filtermodel.h \
	src/database/tablebase.h \
	src/database/bitboard.h \
	src/database/openingtree.h \
	src/database/version.h \
	src/database/playerinfo.h \
	src/database/enginelist.h \
	src/database/enginedata.h \
	src/database/analysis.h \
	src/database/editaction.h \
	src/guess/attacks.h \
	src/guess/common.h \
	src/guess/dstring.h \
	src/guess/guessengine.h \
	src/guess/error.h \
	src/guess/guess.h \
	src/guess/hash.h \
	src/guess/misc.h \
	src/guess/movelist.h \
	src/guess/myassert.h \
	src/guess/position.h \
	src/guess/recog.h \
	src/guess/sqlist.h \
	src/guess/sqmove.h \
	src/guess/sqset.h \
	src/guess/timer.h \
	src/guess/tokens.h \
	src/gui/mainwindow.h \
	src/gui/playerdialog.h \
	src/gui/preferences.h \
	src/gui/boardview.h \
	src/gui/boardtheme.h \
	src/gui/boardsetup.h \
	src/gui/colorlist.h \
	src/gui/savedialog.h \
	src/gui/gamelist.h \
	src/gui/chessbrowser.h \
	src/gui/ecothread.h \
	src/gui/quicksearch.h \
	src/gui/analysiswidget.h \
	src/gui/tableview.h \
	src/gui/copydialog.h \
	src/gui/messagedialog.h \
	src/gui/commentdialog.h \
	src/gui/plaintextedit.h

SOURCES += src/database/board.cpp \
	src/database/common.cpp \
	src/database/game.cpp \
	src/database/databaseconversion.cpp \
	src/database/partialdate.cpp \
	src/database/playerdatabase.cpp \
	src/database/playerdata.cpp \
	src/database/tagvalues.cpp \
	src/database/stringtagvalues.cpp \
	src/database/taglist.cpp \
	src/database/engine.cpp \
	src/database/wbengine.cpp \
	src/database/uciengine.cpp \
	src/database/search.cpp \
	src/database/query.cpp \
	src/database/database.cpp \
	src/database/pgndatabase.cpp \
	src/database/memorydatabase.cpp \
	src/database/filter.cpp \
	src/database/tristatetree.cpp \
	src/database/nag.cpp \
	src/database/settings.cpp \
	src/database/spellchecker.cpp \
	src/database/historylist.cpp \
	src/database/output.cpp \
	src/database/outputoptions.cpp \
	src/database/databaseinfo.cpp \
	src/database/indexitem.cpp \
	src/database/index.cpp \
	src/database/filtermodel.cpp \
	src/database/tablebase.cpp \
	src/database/bitboard.cpp \
	src/database/openingtree.cpp \
	src/database/playerinfo.cpp \
	src/database/enginelist.cpp \
	src/database/enginedata.cpp \
	src/database/analysis.cpp \
	src/database/editaction.cpp \
	src/guess/dstring.cpp \
	src/guess/guessengine.cpp \
	src/guess/guess.cpp \
	src/guess/misc.cpp \
	src/guess/movelist.cpp \
	src/guess/myassert.cpp \
	src/guess/position.cpp \
	src/guess/recog.cpp \
	src/gui/mainwindow.cpp \
	src/gui/mainwindowactions.cpp \
	src/gui/mainwindowabout.cpp \
	src/gui/playerdialog.cpp \
	src/gui/colorlist.cpp \
	src/gui/preferences.cpp \
	src/gui/boardview.cpp \
	src/gui/boardtheme.cpp \
	src/gui/boardsetup.cpp \
	src/gui/savedialog.cpp \
	src/gui/gamelist.cpp \
	src/gui/quicksearch.cpp \
	src/gui/chessbrowser.cpp \
	src/gui/analysiswidget.cpp \
	src/gui/main.cpp \
	src/gui/tableview.cpp \
	src/gui/copydialog.cpp \
	src/gui/messagedialog.cpp \
	src/gui/commentdialog.cpp \
	src/gui/plaintextedit.cpp

TEMPLATE = app
INCLUDEPATH += src/database
INCLUDEPATH += src/guess
INCLUDEPATH += src/gui

macx {
 INSTALLATION_DATA.files = data
 INSTALLATION_DATA.path = Contents/MacOS
 QMAKE_BUNDLE_DATA += INSTALLATION_DATA
 ICON = data/images/chessx.icns
}
else {
	TARGET = bin/chessx
}

RESOURCES = resources.qrc

TRANSLATIONS = i18n/chessx_pl.ts \
	 i18n/chessx_de.ts \
	 i18n/chessx_fr.ts \
	 i18n/chessx_zh.ts

# automatically build translations
isEmpty(QMAKE_LRELEASE) {
     win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
     else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}
isEmpty(TS_DIR):TS_DIR = i18n
TSQM.name = lrelease ${QMAKE_FILE_IN}
TSQM.input = TRANSLATIONS
TSQM.output = $$TS_DIR/${QMAKE_FILE_BASE}.qm
TSQM.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN}
TSQM.CONFIG = no_link
QMAKE_EXTRA_COMPILERS += TSQM
PRE_TARGETDEPS += compiler_TSQM_make_all
 