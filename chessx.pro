# Main application
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += uic warn_on
QT += network svg

TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += printsupport
  QT += core gui widgets
  QT += xml
  QT -= multimediawidgets
  # Comment out sound for Ubuntu with Qt5 if multimedia is not available (default in Ubuntu)
  CONFIG += sound
  # Comment out speech for Linux - there is a bug in Speech which will make ChessX crash (QTBUG-90626)
  # Also comment out speech if libspeechdis is not available
  win32|macx {
    CONFIG += speech
  }

  # Scid requires c++17
  CONFIG += c++17
  # Add lc0 to package
  # CONFIG += lc0
  DEFINES += USE_C11
  CONFIG += scid
}

greaterThan(QT_MAJOR_VERSION, 5) {
    # Needed for QStringRef etc.
    QT += core5compat
}

speech {
  DEFINES += USE_SPEECH
  QT += texttospeech
}

sound {
  DEFINES += USE_SOUND
  QT += multimedia
}

DEFINES += QUAZIP_STATIC
DEFINES += QT_NO_CAST_TO_ASCII
DEFINES *= QT_USE_QSTRINGBUILDER

macx {
  QMAKE_CXXFLAGS += -fvisibility=hidden
  QMAKE_LFLAGS_RELEASE -= -O2
  QMAKE_LFLAGS_RELEASE += -m64 -Ofast
  QMAKE_CXXFLAGS_RELEASE -= -O2
  QMAKE_CXXFLAGS_RELEASE *= -m64 -Ofast

  QMAKE_LFLAGS_DEBUG += -m64 -O0 --coverage
  QMAKE_CXXFLAGS_DEBUG *= -m64 -O0 --coverage
}


unix|!macx {
    isEmpty(PREFIX) {
        bsd {
            PREFIX = /usr/local
        }
        PREFIX = /usr
    }
    BINDIR = $$PREFIX/bin
    DATADIR = $$PREFIX/share

    INSTALLS += appdata desktop icons binfiles

    appdata.files = unix/io.sourceforge.ChessX.metainfo.xml
    appdata.path = $$DATADIR/metainfo
    desktop.files = unix/chessx.desktop
    desktop.path = $$DATADIR/applications

    icons.path = $$DATADIR/icons/hicolor
    icons.commands = install -Dm644 data/images/chessx.png    $${icons.path}/128x128/apps/chessx.png; \
                     install -Dm644 data/images/chessx-32.png $${icons.path}/32x32/apps/chessx.png; \
                     install -Dm644 data/images/chessx-64.png $${icons.path}/64x64/apps/chessx.png;

    binfiles.files = release/chessx
    binfiles.path = $$BINDIR
}

scid {
  # Scid sources
  HEADERS += \
      dep/scid/code/src/attacks.h \
      dep/scid/code/src/board_def.h \
      dep/scid/code/src/bytebuf.h \
      dep/scid/code/src/codec_native.h \
      dep/scid/code/src/codec_scid4.h \
      dep/scid/code/src/codec.h \
      dep/scid/code/src/common.h \
      dep/scid/code/src/containers.h \
      dep/scid/code/src/date.h \
      dep/scid/code/src/dstring.h \
      dep/scid/code/src/error.h \
      dep/scid/code/src/fastgame.h \
      dep/scid/code/src/filebuf.h \
      dep/scid/code/src/fullmove.h \
      dep/scid/code/src/game.h \
      dep/scid/code/src/hash.h \
      dep/scid/code/src/hfilter.h \
      dep/scid/code/src/index.h \
      dep/scid/code/src/indexentry.h \
      dep/scid/code/src/matsig.h \
      dep/scid/code/src/misc.h \
      dep/scid/code/src/movegen.h \
      dep/scid/code/src/movelist.h \
      dep/scid/code/src/movetree.h \
      dep/scid/code/src/naglatex.h \
      dep/scid/code/src/nagtext.h \
      dep/scid/code/src/namebase.h \
      dep/scid/code/src/position.h \
      dep/scid/code/src/sortcache.h \
      dep/scid/code/src/sqmove.h \
      dep/scid/code/src/stored.h \
      dep/scid/code/src/textbuf.h

  SOURCES += \
    dep/scid/code/src/codec_scid4.cpp \
    dep/scid/code/src/game.cpp \
    dep/scid/code/src/matsig.cpp \
    dep/scid/code/src/misc.cpp \
    dep/scid/code/src/position.cpp \
    dep/scid/code/src/sortcache.cpp \
    dep/scid/code/src/stored.cpp \
    dep/scid/code/src/textbuf.cpp

  INCLUDEPATH += dep/scid/code/src

  # database implementation
  HEADERS += src/database/scid/sciddatabase.h
  SOURCES += src/database/scid/sciddatabase.cpp
  INCLUDEPATH += src/database/scid
  DEFINES += USE_SCID
}

FORMS += \
  src/dialogs/aboutdlg.ui \
  src/dialogs/actiondialog.ui \
  src/dialogs/boardsearchdialog.ui \
  src/dialogs/commentdialog.ui \
  src/dialogs/copydialog.ui \
  src/dialogs/databasetagdialog.ui \
  src/dialogs/dlgsavebook.ui \
  src/dialogs/matchparameterdlg.ui \
  src/dialogs/onlinebase.ui \
  src/dialogs/preferences.ui \
  src/dialogs/promotiondialog.ui \
  src/dialogs/quicksearch.ui \
  src/dialogs/recipientaddressdialog.ui \
  src/dialogs/renametagdialog.ui \
  src/dialogs/savedialog.ui \
  src/dialogs/tagdialog.ui \
  src/gui/analysiswidget.ui \
  src/gui/annotationwidget.ui \
  src/gui/boardsetup.ui \
  src/gui/boardviewex.ui \
  src/gui/engineoptiondialog.ui \
  src/gui/ficsconsole.ui \
  src/gui/gamewindow.ui \
  src/gui/helpbrowsershell.ui \
  src/gui/loadquery.ui \
  src/gui/openingtreewidget.ui \
  src/gui/tagdetailwidget.ui


HEADERS += src/compat/qt6compat.h \
  src/database/board.h \
  src/database/abk.h \
  src/database/analysis.h \
  src/database/annotation.h \
  src/database/arenabook.h \
  src/database/bitboard.h \
  src/database/bitfind.h \
  src/database/circularbuffer.h \
  src/database/clipboarddatabase.h \
  src/database/ctg.h \
  src/database/ctgbookwriter.h \
  src/database/ctgdatabase.h \
  src/database/database.h \
  src/database/databaseconversion.h \
  src/database/databaseinfo.h \
  src/database/datesearch.h \
  src/database/downloadmanager.h \
  src/database/duplicatesearch.h \
  src/database/ecoinfo.h \
  src/database/ecopositions.h \
  src/database/editaction.h \
  src/database/elosearch.h \
  src/database/enginedata.h \
  src/database/enginelist.h \
  src/database/engineoptiondata.h \
  src/database/engineparameter.h \
  src/database/enginex.h \
  src/database/eventinfo.h \
  src/database/ficsclient.h \
  src/database/ficsdatabase.h \
  src/database/filter.h \
  src/database/filtermodel.h \
  src/database/filteroperator.h \
  src/database/filtersearch.h \
  src/database/gamecursor.h \
  src/database/gameid.h \
  src/database/gameundocommand.h \
  src/database/gamex.h \
  src/database/historylist.h \
  src/database/index.h \
  src/database/indexitem.h \
  src/database/lichessopening.h \
  src/database/lichessopeningdatabase.h \
  src/database/memorydatabase.h \
  src/database/move.h \
  src/database/movedata.h \
  src/database/nag.h \
  src/database/networkhelper.h \
  src/database/numbersearch.h \
  src/database/openingtree.h \
  src/database/openingtreethread.h \
  src/database/output.h \
  src/database/outputoptions.h \
  src/database/partialdate.h \
  src/database/pdbtest.h \
  src/database/pgndatabase.h \
  src/database/piece.h \
  src/database/playerdata.h \
  src/database/playerdatabase.h \
  src/database/playerinfo.h \
  src/database/polyglotdatabase.h \
  src/database/polyglotwriter.h \
  src/database/positionsearch.h \
  src/database/refcount.h \
  src/database/result.h \
  src/database/search.h \
  src/database/settings.h \
  src/database/spellchecker.h \
  src/database/square.h \
  src/database/streamdatabase.h \
  src/database/tablebase.h \
  src/database/tags.h \
  src/database/tagsearch.h \
  src/database/telnetclient.h \
  src/database/threadedguess.h \
  src/database/uciengine.h \
  src/database/version.h \
  src/database/wbengine.h \
  src/dialogs/aboutdlg.h \
  src/dialogs/actiondialog.h \
  src/dialogs/boardsearchdialog.h \
  src/dialogs/commentdialog.h \
  src/dialogs/copydialog.h \
  src/dialogs/databasetagdialog.h \
  src/dialogs/dlgsavebook.h \
  src/dialogs/matchparameterdlg.h \
  src/dialogs/onlinebase.h \
  src/dialogs/preferences.h \
  src/dialogs/promotiondialog.h \
  src/dialogs/quicksearch.h \
  src/dialogs/recipientaddressdialog.h \
  src/dialogs/renametagdialog.h \
  src/dialogs/savedialog.h \
  src/dialogs/tagdialog.h \
  src/guess/guess.h \
  src/guess/guess_attacks.h \
  src/guess/guess_common.h \
  src/guess/guess_compileeco.h \
  src/guess/guess_error.h \
  src/guess/guess_guessengine.h \
  src/guess/guess_hash.h \
  src/guess/guess_misc.h \
  src/guess/guess_movelist.h \
  src/guess/guess_position.h \
  src/guess/guess_recog.h \
  src/guess/guess_sqlist.h \
  src/guess/guess_sqmove.h \
  src/guess/guess_sqset.h \
  src/gui/GameMimeData.h \
  src/gui/analysiswidget.h \
  src/gui/annotationtimeedit.h \
  src/gui/annotationwidget.h \
  src/gui/boardsetup.h \
  src/gui/boardsetuptoolbutton.h \
  src/gui/boardtheme.h \
  src/gui/boardview.h \
  src/gui/boardviewex.h \
  src/gui/chartwidget.h \
  src/gui/chessbrowser.h \
  src/gui/chessxsettings.h \
  src/gui/colorlist.h \
  src/gui/databaselist.h \
  src/gui/databaselistmodel.h \
  src/gui/digitalclock.h \
  src/gui/dockwidgetex.h \
  src/gui/ecolistwidget.h \
  src/gui/ecothread.h \
  src/gui/engineoptiondialog.h \
  src/gui/engineoptionlist.h \
  src/gui/engineoptionmodel.h \
  src/gui/eventlistwidget.h \
  src/gui/exclusiveactiongroup.h \
  src/gui/exttool.h \
  src/gui/ficsconsole.h \
  src/gui/gamelist.h \
  src/gui/gamelistsortmodel.h \
  src/gui/gamenotationwidget.h \
  src/gui/gametoolbar.h \
  src/gui/gamewindow.h \
  src/gui/helpbrowser.h \
  src/gui/helpbrowsershell.h \
  src/gui/historylabel.h \
  src/gui/htmlitemdelegate.h \
  src/gui/kbaction.h \
  src/gui/listwidgetex.h \
  src/gui/loadquery.h \
  src/gui/logstream.h \
  src/gui/mainwindow.h \
  src/gui/messagedialog.h \
  src/gui/ooo/converter.h \
  src/gui/ooo/document.h \
  src/gui/ooo/formatproperty.h \
  src/gui/ooo/kzip.h \
  src/gui/ooo/styleinformation.h \
  src/gui/ooo/styleparser.h \
  src/gui/openingtreewidget.h \
  src/gui/plaintextedit.h \
  src/gui/playerlistwidget.h \
  src/gui/qled.h \
  src/gui/shellhelper.h \
  src/gui/simplelabel.h \
  src/gui/style.h \
  src/gui/tableview.h \
  src/gui/testadapter.h \
  src/gui/textbrowserex.h \
  src/gui/textedit.h \
  src/gui/toolmainwindow.h \
  src/gui/translatingslider.h \
  src/quazip/JlCompress.h \
  src/quazip/crypt.h \
  src/quazip/ioapi.h \
  src/quazip/quaadler32.h \
  src/quazip/quachecksum32.h \
  src/quazip/quacrc32.h \
  src/quazip/quagzipfile.h \
  src/quazip/quaziodevice.h \
  src/quazip/quazip.h \
  src/quazip/quazip_global.h \
  src/quazip/quazipdir.h \
  src/quazip/quazipfile.h \
  src/quazip/quazipfileinfo.h \
  src/quazip/quazipnewinfo.h \
  src/quazip/unzip.h \
  src/quazip/zip.h

SOURCES += \
  src/database/analysis.cpp \
  src/database/annotation.cpp \
  src/database/arenabook.cpp \
  src/database/bitboard.cpp \
  src/database/board.cpp \
  src/database/clipboarddatabase.cpp \
  src/database/ctgbookwriter.cpp \
  src/database/ctgdatabase.cpp \
  src/database/database.cpp \
  src/database/databaseconversion.cpp \
  src/database/databaseinfo.cpp \
  src/database/datesearch.cpp \
  src/database/downloadmanager.cpp \
  src/database/duplicatesearch.cpp \
  src/database/ecoinfo.cpp \
  src/database/ecopositions.cpp \
  src/database/editaction.cpp \
  src/database/elosearch.cpp \
  src/database/enginedata.cpp \
  src/database/enginelist.cpp \
  src/database/engineoptiondata.cpp \
  src/database/enginex.cpp \
  src/database/eventinfo.cpp \
  src/database/ficsclient.cpp \
  src/database/ficsdatabase.cpp \
  src/database/filter.cpp \
  src/database/filtermodel.cpp \
  src/database/filtersearch.cpp \
  src/database/gamecursor.cpp \
  src/database/gamex.cpp \
  src/database/historylist.cpp \
  src/database/index.cpp \
  src/database/indexitem.cpp \
  src/database/lichessopening.cpp \
  src/database/lichessopeningdatabase.cpp \
  src/database/memorydatabase.cpp \
  src/database/movedata.cpp \
  src/database/nag.cpp \
  src/database/networkhelper.cpp \
  src/database/numbersearch.cpp \
  src/database/openingtree.cpp \
  src/database/openingtreethread.cpp \
  src/database/output.cpp \
  src/database/outputoptions.cpp \
  src/database/partialdate.cpp \
  src/database/pdbtest.cpp \
  src/database/pgndatabase.cpp \
  src/database/piece.cpp \
  src/database/playerdata.cpp \
  src/database/playerdatabase.cpp \
  src/database/playerinfo.cpp \
  src/database/polyglotdatabase.cpp \
  src/database/polyglotwriter.cpp \
  src/database/positionsearch.cpp \
  src/database/refcount.cpp \
  src/database/result.cpp \
  src/database/search.cpp \
  src/database/settings.cpp \
  src/database/spellchecker.cpp \
  src/database/streamdatabase.cpp \
  src/database/tablebase.cpp \
  src/database/tags.cpp \
  src/database/tagsearch.cpp \
  src/database/telnetclient.cpp \
  src/database/threadedguess.cpp \
  src/database/uciengine.cpp \
  src/database/wbengine.cpp \
  src/dialogs/aboutdlg.cpp \
  src/dialogs/actiondialog.cpp \
  src/dialogs/boardsearchdialog.cpp \
  src/dialogs/commentdialog.cpp \
  src/dialogs/copydialog.cpp \
  src/dialogs/databasetagdialog.cpp \
  src/dialogs/dlgsavebook.cpp \
  src/dialogs/matchparameterdlg.cpp \
  src/dialogs/onlinebase.cpp \
  src/dialogs/preferences.cpp \
  src/dialogs/promotiondialog.cpp \
  src/dialogs/quicksearch.cpp \
  src/dialogs/recipientaddressdialog.cpp \
  src/dialogs/renametagdialog.cpp \
  src/dialogs/savedialog.cpp \
  src/dialogs/tagdialog.cpp \
  src/guess/guess.cpp \
  src/guess/guess_compileeco.cpp \
  src/guess/guess_guessengine.cpp \
  src/guess/guess_misc.cpp \
  src/guess/guess_movelist.cpp \
  src/guess/guess_position.cpp \
  src/guess/guess_recog.cpp \
  src/gui/analysiswidget.cpp \
  src/gui/annotationtimeedit.cpp \
  src/gui/annotationwidget.cpp \
  src/gui/boardsetup.cpp \
  src/gui/boardsetuptoolbutton.cpp \
  src/gui/boardtheme.cpp \
  src/gui/boardview.cpp \
  src/gui/boardviewex.cpp \
  src/gui/chartwidget.cpp \
  src/gui/chessbrowser.cpp \
  src/gui/chessxsettings.cpp \
  src/gui/colorlist.cpp \
  src/gui/databaselist.cpp \
  src/gui/databaselistmodel.cpp \
  src/gui/digitalclock.cpp \
  src/gui/dockwidgetex.cpp \
  src/gui/ecolistwidget.cpp \
  src/gui/engineoptiondialog.cpp \
  src/gui/engineoptionlist.cpp \
  src/gui/engineoptionmodel.cpp \
  src/gui/eventlistwidget.cpp \
  src/gui/exclusiveactiongroup.cpp \
  src/gui/exttool.cpp \
  src/gui/ficsconsole.cpp \
  src/gui/gamelist.cpp \
  src/gui/gamelistsortmodel.cpp \
  src/gui/gamenotationwidget.cpp \
  src/gui/gametoolbar.cpp \
  src/gui/gamewindow.cpp \
  src/gui/helpbrowser.cpp \
  src/gui/helpbrowsershell.cpp \
  src/gui/historylabel.cpp \
  src/gui/htmlitemdelegate.cpp \
  src/gui/kbaction.cpp \
  src/gui/listwidgetex.cpp \
  src/gui/loadquery.cpp \
  src/gui/logstream.cpp \
  src/gui/main.cpp \
  src/gui/mainwindow.cpp \
  src/gui/mainwindowabout.cpp \
  src/gui/mainwindowactions.cpp \
  src/gui/messagedialog.cpp \
  src/gui/ooo/converter.cpp \
  src/gui/ooo/document.cpp \
  src/gui/ooo/formatproperty.cpp \
  src/gui/ooo/kzip.cpp \
  src/gui/ooo/styleinformation.cpp \
  src/gui/ooo/styleparser.cpp \
  src/gui/openingtreewidget.cpp \
  src/gui/plaintextedit.cpp \
  src/gui/playerlistwidget.cpp \
  src/gui/qled.cpp \
  src/gui/shellhelper.cpp \
  src/gui/simplelabel.cpp \
  src/gui/style.cpp \
  src/gui/tableview.cpp \
  src/gui/testadapter.cpp \
  src/gui/textedit.cpp \
  src/gui/toolmainwindow.cpp \
  src/gui/translatingslider.cpp \
  src/quazip/JlCompress.cpp \
  src/quazip/qioapi.cpp \
  src/quazip/quaadler32.cpp \
  src/quazip/quacrc32.cpp \
  src/quazip/quagzipfile.cpp \
  src/quazip/quaziodevice.cpp \
  src/quazip/quazip.cpp \
  src/quazip/quazipdir.cpp \
  src/quazip/quazipfile.cpp \
  src/quazip/quazipnewinfo.cpp \
  src/quazip/unzip.cpp \
  src/quazip/zip.cpp

TEMPLATE = app
INCLUDEPATH += src/compat
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
  win32-g++:LIBS += -lz
}

UI_DIR = src/generated
MOC_DIR = src/generated
RCC_DIR = src/generated

CONFIG(debug, debug|release) {
  DESTDIR = "debug"
  OBJECTS_DIR = "obj_dbg"
  DEFINES += _DEBUG
}

CONFIG(release, debug|release) {
  DESTDIR = "release"
  OBJECTS_DIR = "obj_rel"
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
  TIMESEAL_DATA.files = data/timeseal/mac/timeseal
  TIMESEAL_DATA.path = Contents/MacOS/data/timeseal/mac
  QMAKE_BUNDLE_DATA += TIMESEAL_DATA
  QMAKE_INFO_PLIST = mac_osx/Info.plist
}

RESOURCES = \
  resources.qrc translations.qrc

TRANSLATIONS = i18n/chessx_de.ts

#        i18n/chessx_fr.ts \
#        i18n/chessx_da.ts \
#        i18n/chessx_fr.ts \
#        i18n/chessx_it.ts \
#        i18n/chessx_cz.ts \
#        i18n/chessx_ru.ts \
#        i18n/chessx_es.ts

#   i18n/chessx_nl.ts \
#   i18n/chessx_pl.ts \
#   i18n/chessx_pt_BR.ts \
#   i18n/chessx_ro.ts \
#   i18n/chessx_zh.ts

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
  ChangeLog.md \
  COPYING.md \
  ChangeLog.txt \
  data/help/about.css \
  data/help/about-dark.css \
  data/help/about0.html \
  data/help/about1.html \
  data/help/about1a.html \
  data/help/about2.html \
  data/help/about3.html \
  data/help/about4.html \
  data/help/about5.html \
  setup7-64.iss \
  setup7-32.iss \
  setupXP.iss \
  data/styles/orange.css \
  unix/chessx.desktop

macx {
  OTHER_FILES += \
    mac_osx/Info.plist \
    mac_osx/qt_menu.nib \
    data/timeseal/mac/timeseal
}

win32 {
  OTHER_FILES += \
    src/chessx.rc \
    src/chessx.ico
}

DISTFILES += \
  INSTALL.md \
  TODO.md \
  Doxyfile \
  License.txt \
  README.developers.md \
  unix/make_tarball \
  mac_osx/svnlist \
  mac_osx/fix_paths.sh \
  mac_osx/mac_deploy_dyn \
  mac_osx/qt_menu.nib \
  data/lang/readme-lang.txt


