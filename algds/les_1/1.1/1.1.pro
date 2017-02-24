TEMPLATE = app
TARGET = test_1.1

CONFIG -= qt
CONFIG += warn_on

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Werror -Wextra -pedantic-errors

#DEFINES += STREAM_TYPE=\\\"term\\\"

SOURCES += main.cpp

DESTDIR = .
OBJECTS_DIR = .o
