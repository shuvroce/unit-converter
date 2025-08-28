# Qt Widgets
QT += widgets

# C++ version
CONFIG += c++17 windows
CONFIG -= console   # <-- Remove console completely
RESOURCES += resources.qrc
RC_FILE = app.rc

TEMPLATE = app
TARGET = converter

SOURCES += main.cpp \
           calcs.cpp

HEADERS += calcs.h
