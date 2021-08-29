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
    ../../src/database/ecopositions.cpp \
    ../../src/guess/guess_compileeco.cpp

HEADERS += \
    ../../src/database/ecopositions.h \
    ../../src/guess/guess_compileeco.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../chessx-board/release/ -lchessx-board
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../chessx-board/debug/ -lchessx-board
else:unix: LIBS += -L$$OUT_PWD/../chessx-board/ -lchessx-board

INCLUDEPATH += $$PWD/../../src/database
INCLUDEPATH += $$PWD/../../src/guess
DEPENDPATH += $$PWD/../../src/database
DEPENDPATH += $$PWD/../../src/guess

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/release/libchessx-board.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/debug/libchessx-board.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/release/chessx-board.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/debug/chessx-board.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/libchessx-board.a
