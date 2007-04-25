# Guess the move library

HEADERS += attacks.h \
           common.h \
           dstring.h \
           engine.h \
           error.h \
           guess.h \
           hash.h \
           misc.h \
           movelist.h \
           myassert.h \
           position.h \
           recog.h \
           sqlist.h \
           sqmove.h \
           sqset.h \
           timer.h \
           tokens.h
SOURCES += dstring.cpp \
           engine.cpp \
           guess.cpp \
           misc.cpp \
           movelist.cpp \
           myassert.cpp \
           position.cpp \
           recog.cpp

include(../common.pro)
TARGET = guess
CONFIG += staticlib
TEMPLATE = lib

