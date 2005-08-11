TEMPLATE = app
QT += qt3support
CONFIG = qt debug warn_on static
TARGET = test
SOURCES += main.cpp

INCLUDEPATH += ../../src/database \
               ../../src/compatibility 
							 
win32{
  LIBS += ../../src/database/release/libdatabase.a
}
else{
  LIBS += ../../src/database/libdatabase.a
}
