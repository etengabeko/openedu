TEMPLATE = app
TARGET = test_4.2

CONFIG -= qt
CONFIG += warn_on

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Werror -Wextra -pedantic-errors

#DEFINES += STREAM_TYPE=\\\"term\\\"

SOURCES += main.cxx

DESTDIR = .
OBJECTS_DIR = .o
