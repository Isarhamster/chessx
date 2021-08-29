QT -= gui

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

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
    ../../src/database/bitboard.cpp \
    ../../src/database/board.cpp \
    ../../src/database/piece.cpp

HEADERS += \
    ../../src/database/bitboard.h \
    ../../src/database/bitfind.h \
    ../../src/database/board.h \
    ../../src/database/move.h \
    ../../src/database/piece.h \
    ../../src/database/square.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
