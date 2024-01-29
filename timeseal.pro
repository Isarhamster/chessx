# Main application
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += console
TEMPLATE = app

unix|!macx {
    isEmpty(PREFIX) {
        bsd {
            PREFIX = /usr/local
        }
        PREFIX = /usr
    }
    BINDIR = $$PREFIX/bin

    INSTALLS += binfiles

    binfiles.files = release/timeseal
    binfiles.path = $$BINDIR
}

HEADERS += src/timeseal/config.h
SOURCES +=   src/timeseal/zseal.c
INCLUDEPATH += src/timeseal

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
  
TARGET = timeseal
