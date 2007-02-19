# Main application

include(../common.pro)
CONFIG += uic

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
RESOURCES += helpwindow.qrc \
                 tipoftheday.qrc

win32 {
  TARGET = ../../../bin/chessdatabase
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
  TARGET = ../../bin/chessdatabase
  LIBS += ../database/libdatabase.a
  PRE_TARGETDEPS += ../database/libdatabase.a
}

macx {
     ICON = ../../data/images/chessx.icns
}
