# Main application

include(../common.pro)
CONFIG += uic 
QT += xml

FORMS += playerdialog.ui \
	preferences.ui \
	boardsetup.ui \
	quicksearch.ui \
	savedialog.ui \
	tipoftheday.ui \
	analysiswidget.ui
HEADERS += mainwindow.h \
	playerdialog.h \
	preferences.h \
	helpwindow.h \
	boardview.h \
	boardtheme.h \
	boardsetup.h \
	colorlist.h \
	savedialog.h \
	gamelist.h \
	chessbrowser.h \
	ecothread.h \
	quicksearch.h \
	tipoftheday.h \
	analysiswidget.h \
 tableview.h
SOURCES += mainwindow.cpp \
	playerdialog.cpp \
	colorlist.cpp \
	preferences.cpp \
	boardview.cpp \
	boardtheme.cpp \
	boardsetup.cpp \
	helpwindow.cpp \
	savedialog.cpp \
	gamelist.cpp \
	quicksearch.cpp \
	chessbrowser.cpp \
	tipoftheday.cpp \
	analysiswidget.cpp \
	main.cpp \
 tableview.cpp

TEMPLATE = app
INCLUDEPATH += ../database 
INCLUDEPATH += ../guess
DEPENDPATH =../database
RESOURCES += helpwindow.qrc \
                 tipoftheday.qrc

win32 {
    TARGET = ../../../bin/chessx
    release {
        LIBS += ../database/release/libdatabase.a
        PRE_TARGETDEPS += ../database/release/libdatabase.a
        LIBS += ../guess/release/libguess.a
        PRE_TARGETDEPS += ../guess/release/libguess.a
    }
    else {
        LIBS += ../database/debug/libdatabase.a
        PRE_TARGETDEPS += ../database/debug/libdatabase.a
        LIBS += ../guess/debug/libguess.a
        PRE_TARGETDEPS += ../guess/debug/libguess.a
    }
}
else {
    TARGET = ../../bin/chessx
    LIBS += ../database/libdatabase.a
    PRE_TARGETDEPS += ../database/libdatabase.a
    LIBS += ../guess/libguess.a
    PRE_TARGETDEPS += ../guess/libguess.a
}

macx {
    ICON = ../../data/images/chessx.icns
}

TRANSLATIONS = chessx_pl.ts \
               chessx_de.ts
