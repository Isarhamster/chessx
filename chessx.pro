# Main application
DEFINES +=
CONFIG += uic warn_on
QT += xml network

greaterThan(QT_MAJOR_VERSION, 4) {
   QT += printsupport
}

static { # Everything below takes effect with CONFIG += static
    DEFINES += STATIC
    message("Static build.")
}

DEFINES += QUAZIP_STATIC

macx {
	QMAKE_CXXFLAGS += -fvisibility=hidden
}

FORMS += src/gui/playerdialog.ui \
	src/gui/preferences.ui \
	src/gui/boardsetup.ui \
	src/gui/quicksearch.ui \
	src/gui/savedialog.ui \
	src/gui/analysiswidget.ui \
	src/gui/copydialog.ui \
	src/gui/commentdialog.ui \
    src/gui/engineoptiondialog.ui

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
	src/gui/plaintextedit.h \
    src/gui/playerlist.h \
    src/database/playerlistmodel.h \
    src/gui/databaselist.h \
    src/gui/databaselistmodel.h \
    src/gui/dockwidgetex.h \
    src/gui/logstream.h \
    src/gui/boardsetuptoolbutton.h \
    src/gui/GameMimeData.h \
    src/database/engineoptiondata.h \
    src/gui/engineoptionmodel.h \
    src/gui/engineoptiondialog.h \
    src/gui/engineoptionlist.h \
    src/gui/helpbrowser.h \
    src/database/downloadmanager.h \
    src/quazip/zip.h \
    src/quazip/unzip.h \
    src/quazip/quazipnewinfo.h \
    src/quazip/quazipfileinfo.h \
    src/quazip/quazipfile.h \
    src/quazip/quazipdir.h \
    src/quazip/quazip.h \
    src/quazip/quazip_global.h \
    src/quazip/quaziodevice.h \
    src/quazip/quagzipfile.h \
    src/quazip/quacrc32.h \
    src/quazip/quachecksum32.h \
    src/quazip/quaadler32.h \
    src/quazip/JlCompress.h \
    src/quazip/ioapi.h \
    src/quazip/crypt.h \
    src/gui/toolmainwindow.h

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
	src/gui/plaintextedit.cpp \
    src/gui/playerlist.cpp \
    src/database/playerlistmodel.cpp \
    src/gui/databaselist.cpp \
    src/gui/databaselistmodel.cpp \
    src/gui/dockwidgetex.cpp \
    src/gui/logstream.cpp \
    src/gui/boardsetuptoolbutton.cpp \
    src/database/engineoptiondata.cpp \
    src/gui/engineoptionmodel.cpp \
    src/gui/engineoptiondialog.cpp \
    src/gui/engineoptionlist.cpp \
    src/gui/helpbrowser.cpp \
    src/database/downloadmanager.cpp \
    src/quazip/quazipnewinfo.cpp \
    src/quazip/quazipfile.cpp \
    src/quazip/quazipdir.cpp \
    src/quazip/quazip.cpp \
    src/quazip/quaziodevice.cpp \
    src/quazip/quagzipfile.cpp \
    src/quazip/quacrc32.cpp \
    src/quazip/quaadler32.cpp \
    src/quazip/qioapi.cpp \
    src/quazip/JlCompress.cpp \
    src/quazip/zip.c \
    src/quazip/unzip.c \
    src/gui/toolmainwindow.cpp

TEMPLATE = app
INCLUDEPATH += src/database
INCLUDEPATH += src/guess
INCLUDEPATH += src/gui
INCLUDEPATH += src/quazip
INCLUDEPATH += $$[QT_INSTALL_PREFIX]/src/3rdparty/zlib
INCLUDEPATH += /QtSDK/QtSources/4.8.1/src/3rdparty/zlib

UI_DIR = src/generated
MOC_DIR = src/generated
RCC_DIR = src/generated

CONFIG(debug, debug|release) {
    DESTDIR = "debug"
    OBJECTS_DIR = "obj_dbg"
}

CONFIG(release, debug|release) {
    DESTDIR = "release"
    OBJECTS_DIR = "obj_rel"
    DEFINES += QT_NO_DEBUG_OUTPUT
}

static {
    DESTDIR = "static"
    OBJECTS_DIR = "obj_static"
    DEFINES += QT_NO_DEBUG_OUTPUT
}

!win32 {
    CONFIG(debug, debug|release) {
        LIBS += -lz
    }
    CONFIG(release, debug|release) {
        LIBS += -lz
    }
}

TARGET = chessx

ICON = data/images/chessx.icns
RC_FILE = src/chessx.rc

macx {
 INSTALLATION_DATA.files = mac_osx/qt_menu.nib
 INSTALLATION_DATA.path = Contents/Resources/
 QMAKE_BUNDLE_DATA += INSTALLATION_DATA
 ENGINE_DATA.files = data/engines-mac/uci/stockfish-23-64
 ENGINE_DATA.path = Contents/MacOS/data/engines-mac/uci
 QMAKE_BUNDLE_DATA += ENGINE_DATA
 QMAKE_INFO_PLIST = mac_osx/Info.plist
}

unix:!macx {
 target.path = /usr/games/chessx
 INSTALLS += target
}

RESOURCES = resources.qrc

#TRANSLATIONS = i18n/chessx_de.ts \
#	i18n/chessx_fr.ts \
#	i18n/chessx_it.ts \
#	i18n/chessx_nl.ts \
#	i18n/chessx_pl.ts \
#	i18n/chessx_pt_BR.ts \
#	i18n/chessx_zh.ts

# automatically build translations
#isEmpty(QMAKE_LRELEASE) {
#     win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\\lrelease.exe
#     else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
#}

#isEmpty(TS_DIR):TS_DIR = i18n
#TSQM.name = lrelease ${QMAKE_FILE_IN}
#TSQM.input = TRANSLATIONS
#TSQM.output = $$TS_DIR/${QMAKE_FILE_BASE}.qm
#TSQM.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
#TSQM.CONFIG += no_link target_predeps
#QMAKE_EXTRA_COMPILERS += TSQM
#PRE_TARGETDEPS += compiler_TSQM_make_all

OTHER_FILES += \
    data/templates/pgn-default.template \
    data/templates/notation-default.template \
    data/templates/latex-default.template \
    data/templates/html-default.template

macx {
  OTHER_FILES += \
    mac_osx/Info.plist \
    mac_osx/qt_menu.nib \
    data/engines-mac/uci/stockfish-23-64
}

win32 {
  OTHER_FILES += \
    src/chessx.rc \
    src/chessx.ico
}


