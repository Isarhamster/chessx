# Main application

include(../common.pro)
CONFIG += uic 
QT += xml

FORMS += playerdialog.ui \
         preferences.ui \
         boardsetup.ui \
         savedialog.ui \
         tipoftheday.ui
HEADERS += mainwindow.h \
           playerdialog.h \
           preferences.h \
           helpwindow.h \
           boardview.h \
           boardtheme.h \
           boardsetup.h \
           savedialog.h \
           gamelist.h \
           chessbrowser.h \
           ecothread.h \
           tipoftheday.h
SOURCES += mainwindow.cpp \
           playerdialog.cpp \
           preferences.cpp \
           boardview.cpp \
           boardtheme.cpp \
           boardsetup.cpp \
           helpwindow.cpp \
           savedialog.cpp \
           gamelist.cpp \
	   chessbrowser.cpp \
           tipoftheday.cpp \
	   main.cpp
TEMPLATE = app
INCLUDEPATH += ../database 
INCLUDEPATH += ../guess
RESOURCES += helpwindow.qrc \
                 tipoftheday.qrc

win32 {
  TARGET = ../../../bin/chessx
  release {
     LIBS += ../database/release/libdatabase.a
     PRE_TARGETDEPS += ../database/release/libdatabase.a
  }
  else {
     LIBS += ../database/debug/libdatabase.a
     PRE_TARGETDEPS += ../database/debug/libdatabase.a
  }
}
else { 
  TARGET = ../../bin/chessx
  LIBS += ../database/libdatabase.a
  PRE_TARGETDEPS += ../database/libdatabase.a
}
LIBS += ../guess/libguess.a
PRE_TARGETDEPS += ../guess/libguess.a

macx {
     ICON = ../../data/images/chessx.icns
}
