QT += widgets

CONFIG += c++17 console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = converter

SOURCES += main.cpp \
           calcs.cpp

HEADERS += calcs.h
