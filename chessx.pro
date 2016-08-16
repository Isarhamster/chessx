# Main application
DEFINES +=
CONFIG += uic warn_on
QT += network svg

greaterThan(QT_MAJOR_VERSION, 4) {
   QT += printsupport
   QT += widgets
   QT += xml
   QT -= multimediawidgets
   # Comment out sound for Ubuntu with Qt5 if multimedia is not available (default in Ubuntu)
   CONFIG += sound
   # Comment out c++11 for all non-C++11 compilers, Qt5 is required in addition
   CONFIG += c++11
}

sound {
   DEFINES += USE_SOUND
   QT += multimedia
}

c++11 {
   DEFINES += USE_C11
}

static { # Everything below takes effect with CONFIG += static
    DEFINES += STATIC
    message("Static build.")
}

DEFINES += QUAZIP_STATIC
DEFINES += QT_NO_CAST_TO_ASCII
DEFINES *= QT_USE_QSTRINGBUILDER

macx {
  QMAKE_MAC_SDK = macosx10.11
  QMAKE_CXXFLAGS += -fvisibility=hidden
  QMAKE_LFLAGS_RELEASE -= -O2
  QMAKE_LFLAGS_RELEASE += -m64 -O3
  QMAKE_CXXFLAGS_RELEASE -= -O2
  QMAKE_CXXFLAGS_RELEASE *= -m64 -O3
}

FORMS += \
    src/dialogs/savedialog.ui \
    src/dialogs/copydialog.ui \
    src/dialogs/renametagdialog.ui \
    src/dialogs/aboutdlg.ui \
    src/dialogs/promotiondialog.ui \
    src/dialogs/actiondialog.ui \
    src/dialogs/dlgsavebook.ui \
    src/dialogs/boardsearchdialog.ui \
    src/dialogs/recipientaddressdialog.ui \
    src/dialogs/commentdialog.ui \
    src/gui/engineoptiondialog.ui \
    src/gui/boardsetup.ui \
    src/gui/analysiswidget.ui \
    src/gui/tagdetailwidget.ui \
    src/gui/openingtreewidget.ui \
    src/gui/loadquery.ui \
    src/gui/ficsconsole.ui \
    src/dialogs/tagdialog.ui \
    src/dialogs/preferences.ui \
    src/dialogs/quicksearch.ui \
    src/gui/gamewindow.ui \
    src/dialogs/matchparameterdlg.ui \
    src/gui/boardviewex.ui \
    src/gui/helpbrowsershell.ui


HEADERS += src/database/board.h \
	src/database/move.h \
	src/database/game.h \
	src/database/partialdate.h \
	src/database/playerdatabase.h \
	src/database/playerdata.h \
	src/database/databaseconversion.h \
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
	src/guess/guessengine.h \
	src/guess/error.h \
	src/guess/guess.h \
	src/guess/hash.h \
	src/guess/misc.h \
	src/guess/movelist.h \
	src/guess/position.h \
	src/guess/recog.h \
	src/guess/sqlist.h \
	src/guess/sqmove.h \
	src/guess/sqset.h \
	src/guess/tokens.h \
	src/gui/mainwindow.h \
	src/gui/boardview.h \
	src/gui/boardtheme.h \
	src/gui/boardsetup.h \
	src/gui/colorlist.h \
	src/gui/gamelist.h \
	src/gui/chessbrowser.h \
	src/gui/ecothread.h \
	src/gui/analysiswidget.h \
	src/gui/tableview.h \
	src/gui/messagedialog.h \
	src/gui/plaintextedit.h \
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
    src/gui/toolmainwindow.h \
    src/gui/playerlistwidget.h \
    src/gui/qled.h \
    src/gui/eventlistwidget.h \
    src/database/eventinfo.h \
    src/gui/openingtreewidget.h \
    src/gui/ecolistwidget.h \
    src/database/ecoinfo.h \
    src/gui/textbrowserex.h \
    src/gui/loadquery.h \
    src/guess/compileeco.h \
    src/database/pdbtest.h \
    src/gui/kbaction.h \
    src/gui/translatingslider.h \
    src/gui/exclusiveactiongroup.h \
    src/database/polyglotdatabase.h \
    src/database/openingtreethread.h \
    src/database/movedata.h \
    src/database/polyglotwriter.h \
    src/gui/shellhelper.h \
    src/database/ctgdatabase.h \
    src/database/ctgbookwriter.h \
    src/database/telnetclient.h \
    src/database/ficsclient.h \
    src/gui/style.h \
    src/gui/chartwidget.h \
    src/gui/ficsconsole.h \
    src/database/clipboarddatabase.h \
    src/database/ficsdatabase.h \
    src/gui/exttool.h \
    src/dialogs/aboutdlg.h \
    src/dialogs/actiondialog.h \
    src/dialogs/boardsearchdialog.h \
    src/dialogs/commentdialog.h \
    src/dialogs/copydialog.h \
    src/dialogs/dlgsavebook.h \
    src/dialogs/promotiondialog.h \
    src/dialogs/recipientaddressdialog.h \
    src/dialogs/renametagdialog.h \
    src/dialogs/savedialog.h \
    src/dialogs/tagdialog.h \
    src/dialogs/preferences.h \
    src/dialogs/quicksearch.h \
    src/database/tags.h \
    src/database/piece.h \
    src/database/square.h \
    src/database/result.h \
    src/database/threadedguess.h \
    src/gui/gamewindow.h \
    src/database/ecopositions.h \
    src/gui/textedit.h \
    src/gui/ooo/converter.h \
    src/gui/ooo/document.h \
    src/gui/ooo/formatproperty.h \
    src/gui/ooo/styleinformation.h \
    src/gui/ooo/styleparser.h \
    src/gui/ooo/kzip.h \
    src/database/engineparameter.h \
    src/dialogs/matchparameterdlg.h \
    src/gui/boardviewex.h \
    src/gui/simplelabel.h \
    src/database/bitfind.h \
    src/gui/helpbrowsershell.h \
    src/database/gameid.h \
    src/database/positionsearch.h \
    src/database/duplicatesearch.h \
    src/database/elosearch.h \
    src/database/datesearch.h \
    src/database/tagsearch.h \
    src/database/filtersearch.h \
    src/database/numbersearch.h \
    src/database/circularbuffer.h \
    src/gui/digitalclock.h \
    src/gui/historylabel.h

SOURCES += src/database/board.cpp \
	src/database/game.cpp \
	src/database/databaseconversion.cpp \
	src/database/partialdate.cpp \
	src/database/playerdatabase.cpp \
	src/database/playerdata.cpp \
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
	src/guess/guessengine.cpp \
	src/guess/guess.cpp \
	src/guess/misc.cpp \
	src/guess/movelist.cpp \
	src/guess/position.cpp \
	src/guess/recog.cpp \
	src/gui/mainwindow.cpp \
	src/gui/mainwindowactions.cpp \
	src/gui/mainwindowabout.cpp \
	src/gui/colorlist.cpp \
	src/gui/boardview.cpp \
	src/gui/boardtheme.cpp \
	src/gui/boardsetup.cpp \
	src/gui/gamelist.cpp \
	src/gui/chessbrowser.cpp \
	src/gui/analysiswidget.cpp \
	src/gui/main.cpp \
	src/gui/tableview.cpp \
	src/gui/messagedialog.cpp \
	src/gui/plaintextedit.cpp \
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
    src/gui/toolmainwindow.cpp \
    src/gui/playerlistwidget.cpp \
    src/gui/qled.cpp \
    src/gui/eventlistwidget.cpp \
    src/database/eventinfo.cpp \
    src/gui/openingtreewidget.cpp \
    src/gui/ecolistwidget.cpp \
    src/database/ecoinfo.cpp \
    src/gui/loadquery.cpp \
    src/guess/compileeco.cpp \
    src/database/pdbtest.cpp \
    src/gui/kbaction.cpp \
    src/gui/translatingslider.cpp \
    src/gui/exclusiveactiongroup.cpp \
    src/database/polyglotdatabase.cpp \
    src/database/openingtreethread.cpp \
    src/database/movedata.cpp \
    src/database/polyglotwriter.cpp \
    src/gui/shellhelper.cpp \
    src/database/ctgdatabase.cpp \
    src/database/ctgbookwriter.cpp \
    src/database/telnetclient.cpp \
    src/database/ficsclient.cpp \
    src/gui/style.cpp \
    src/gui/chartwidget.cpp \
    src/gui/ficsconsole.cpp \
    src/database/clipboarddatabase.cpp \
    src/database/ficsdatabase.cpp \
    src/gui/exttool.cpp \
    src/dialogs/aboutdlg.cpp \
    src/dialogs/actiondialog.cpp \
    src/dialogs/boardsearchdialog.cpp \
    src/dialogs/commentdialog.cpp \
    src/dialogs/copydialog.cpp \
    src/dialogs/dlgsavebook.cpp \
    src/dialogs/promotiondialog.cpp \
    src/dialogs/recipientaddressdialog.cpp \
    src/dialogs/renametagdialog.cpp \
    src/dialogs/savedialog.cpp \
    src/dialogs/tagdialog.cpp \
    src/dialogs/preferences.cpp \
    src/dialogs/quicksearch.cpp \
    src/database/tags.cpp \
    src/database/piece.cpp \
    src/database/result.cpp \
    src/database/threadedguess.cpp \
    src/gui/gamewindow.cpp \
    src/database/ecopositions.cpp \
    src/gui/textedit.cpp \
    src/gui/ooo/converter.cpp \
    src/gui/ooo/document.cpp \
    src/gui/ooo/formatproperty.cpp \
    src/gui/ooo/styleinformation.cpp \
    src/gui/ooo/styleparser.cpp \
    src/gui/ooo/kzip.cpp \
    src/dialogs/matchparameterdlg.cpp \
    src/gui/boardviewex.cpp \
    src/gui/simplelabel.cpp \
    src/gui/helpbrowsershell.cpp \
    src/database/positionsearch.cpp \
    src/database/duplicatesearch.cpp \
    src/database/elosearch.cpp \
    src/database/datesearch.cpp \
    src/database/tagsearch.cpp \
    src/database/filtersearch.cpp \
    src/database/numbersearch.cpp \
    src/gui/digitalclock.cpp \
    src/gui/historylabel.cpp

TEMPLATE = app
INCLUDEPATH += src/database
INCLUDEPATH += src/guess
INCLUDEPATH += src/gui
INCLUDEPATH += src/dialogs
INCLUDEPATH += src/quazip
INCLUDEPATH += $$[QT_INSTALL_PREFIX]/src/3rdparty/zlib

win32 {
    # DEFINES += ZLIB_WINAPI
    # LIBS += -lz
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib
}

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
    DEFINES += QT_NO_DEBUG_OUTPUT NDEBUG
}

static {
    DESTDIR = "static"
    OBJECTS_DIR = "obj_static"
    DEFINES += QT_NO_DEBUG_OUTPUT NDEBUG
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
 ENGINE_DATA.files = data/engines-mac/uci/stockfish-7-64
 ENGINE_DATA.path = Contents/MacOS/data/engines-mac/uci
 QMAKE_BUNDLE_DATA += ENGINE_DATA
 BOOK_DATA.files = data/engines-mac/uci/book.bin
 BOOK_DATA.path = Contents/MacOS/data/engines-mac/uci
 QMAKE_BUNDLE_DATA += BOOK_DATA
 TIMESEAL_DATA.files = data/timeseal/mac/timeseal
 TIMESEAL_DATA.path = Contents/MacOS/data/timeseal/mac
 QMAKE_BUNDLE_DATA += TIMESEAL_DATA
 QMAKE_INFO_PLIST = mac_osx/Info.plist
}

RESOURCES = \
    resources.qrc

TRANSLATIONS = i18n/chessx_de.ts \
        i18n/chessx_da.ts \
        i18n/chessx_fr.ts \
        i18n/chessx_it.ts

#        i18n/chessx_nl.ts \
#        i18n/chessx_pl.ts \
#        i18n/chessx_pt_BR.ts \
#        i18n/chessx_ro.ts \
#        i18n/chessx_zh.ts

isEmpty(QMAKE_LRELEASE) {
     win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\\lrelease.exe
     else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

isEmpty(TS_DIR):TS_DIR = i18n
TSQM.name = lrelease ${QMAKE_FILE_IN}
TSQM.input = TRANSLATIONS
TSQM.output = $$TS_DIR/${QMAKE_FILE_BASE}.qm
TSQM.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
TSQM.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += TSQM
PRE_TARGETDEPS += compiler_TSQM_make_all

OTHER_FILES += \
    data/templates/pgn-default.template \
    data/templates/notation-default.template \
    data/templates/latex-default.template \
    data/templates/html-default.template \
    ChangeLog \
    COPYING \
    ChangeLog.txt \
    data/help/about.css \
    data/help/about0.html \
    data/help/about1.html \
    data/help/about1a.html \
    data/help/about2.html \
    data/help/about3.html \
    data/help/about4.html \
    data/help/about5.html \
    data/help/about6.html \
    setup7-64.iss \
    setup7-32.iss \
    setupXP.iss \
    data/styles/orange.css \
    unix/chessx.desktop

macx {
  OTHER_FILES += \
    mac_osx/Info.plist \
    mac_osx/qt_menu.nib \
    data/engines-mac/uci/stockfish-7-64 \
    data/engines-mac/uci/book.bin \
    data/timeseal/mac/timeseal
}

win32 {
  OTHER_FILES += \
    src/chessx.rc \
    src/chessx.ico
}

DISTFILES += \
    INSTALL \
    TODO \
    Doxyfile \
    License.txt \
    README.developers \
    unix/make_tarball \
    mac_osx/svnlist


