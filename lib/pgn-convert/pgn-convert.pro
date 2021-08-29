QT -= gui
CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

RESOURCES = \
  ../../resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG(debug, debug|release) {
  DESTDIR = "debug"
}

CONFIG(release, debug|release) {
  DESTDIR = "release"
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../chessx-board/release/ -lchessx-board
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../chessx-board/debug/ -lchessx-board
else:unix: LIBS += -L$$OUT_PWD/../chessx-board/ -lchessx-board

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../chessx-board/release/ -lchessx-board
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../chessx-board/debug/ -lchessx-board
else:unix: LIBS += -L$$OUT_PWD/../chessx-board/ -lchessx-board

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../chessx-eco/release/ -lchessx-eco
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../chessx-eco/debug/ -lchessx-eco
else:unix: LIBS += -L$$OUT_PWD/../chessx-eco/ -lchessx-eco

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../chessx-pgn/release/ -lchessx-pgn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../chessx-pgn/debug/ -lchessx-pgn
else:unix: LIBS += -L$$OUT_PWD/../chessx-pgn/ -lchessx-pgn

INCLUDEPATH += $$PWD/../../src/database
INCLUDEPATH += $$PWD/../../src/guess
DEPENDPATH += $$PWD/../../src/database
DEPENDPATH += $$PWD/../../src/guess

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/release/libchessx-board.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/debug/libchessx-board.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/release/chessx-board.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/debug/chessx-board.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/libchessx-board.a

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/release/libchessx-board.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/debug/libchessx-board.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/release/chessx-board.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/debug/chessx-board.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../chessx-board/libchessx-board.a

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-eco/release/libchessx-eco.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-eco/debug/libchessx-eco.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-eco/release/chessx-eco.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-eco/debug/chessx-eco.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../chessx-eco/libchessx-eco.a

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-pgn/release/libchessx-pgn.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-pgn/debug/libchessx-pgn.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-pgn/release/chessx-pgn.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../chessx-pgn/debug/chessx-pgn.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../chessx-pgn/libchessx-pgn.a
