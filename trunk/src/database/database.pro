HEADERS += board.h \
           move.h \
	   common.h
SOURCES += board.cpp \
           move.cpp 

TARGET = database 
CONFIG += release warn_on qt staticlib 
TEMPLATE = lib 
