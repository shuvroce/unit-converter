QT += widgets

CONFIG += c++17 windows
CONFIG -= console
RESOURCES += resources.qrc
RC_FILE = app.rc

TEMPLATE = app
TARGET = ell

SOURCES += main.cpp

HEADERS += calcs.h
