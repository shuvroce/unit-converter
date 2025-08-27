# Qt Widgets
QT += widgets

# C++ version
CONFIG += c++17 windows
CONFIG -= console   # <-- Remove console completely

TEMPLATE = app
TARGET = converter

SOURCES += main.cpp \
           calcs.cpp

HEADERS += calcs.h
