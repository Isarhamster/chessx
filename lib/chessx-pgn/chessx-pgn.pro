QT -= gui
TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32:CONFIG(debug, debug|release) {
  DESTDIR = "debug"
}

win32:CONFIG(release, debug|release) {
  DESTDIR = "release"
}

SOURCES += \
    ../../src/database/annotation.cpp \
    ../../src/database/database.cpp \
    ../../src/database/filter.cpp \
    ../../src/database/gamecursor.cpp \
    ../../src/database/gamex.cpp \
    ../../src/database/index.cpp \
    ../../src/database/indexitem.cpp \
    ../../src/database/memorydatabase.cpp \
    ../../src/database/nag.cpp \
    ../../src/database/output.cpp \
    ../../src/database/outputoptions.cpp \
    ../../src/database/partialdate.cpp \
    ../../src/database/pgndatabase.cpp \
    ../../src/database/refcount.cpp \
    ../../src/database/result.cpp \
    ../../src/database/search.cpp \
    ../../src/database/settings.cpp \
    ../../src/database/streamdatabase.cpp \
    ../../src/database/tags.cpp

HEADERS += \
    ../../src/database/annotation.h \
    ../../src/database/database.h \
    ../../src/database/filter.h \
    ../../src/database/gamecursor.h \
    ../../src/database/gameid.h \
    ../../src/database/gamex.h \
    ../../src/database/index.h \
    ../../src/database/indexitem.h \
    ../../src/database/memorydatabase.h \
    ../../src/database/nag.h \
    ../../src/database/output.h \
    ../../src/database/outputoptions.h \
    ../../src/database/partialdate.h \
    ../../src/database/pgndatabase.h \
    ../../src/database/refcount.h \
    ../../src/database/result.h \
    ../../src/database/search.h \
    ../../src/database/settings.h \
    ../../src/database/streamdatabase.h \
    ../../src/database/tags.h \
    ../../src/database/version.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../chessx-eco/release/ -lchessx-eco
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../chessx-eco/debug/ -lchessx-eco
else:unix: LIBS += -L$$OUT_PWD/../chessx-eco/ -lchessx-eco

INCLUDEPATH += $$PWD/../../src/database
INCLUDEPATH += $$PWD/../../src/guess
DEPENDPATH += $$PWD/../../src/database
DEPENDPATH += $$PWD/../../src/guess

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-eco/release/libchessx-eco.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-eco/debug/libchessx-eco.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-eco/release/chessx-eco.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-eco/debug/chessx-eco.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../chessx-eco/libchessx-eco.a
