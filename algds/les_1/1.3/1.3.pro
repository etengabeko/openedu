TEMPLATE = app
TARGET = test_1.3

CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

DEFINES += STREAM_TYPE=\\\"term\\\"

SOURCES += main.cpp

DESTDIR = .
OBJECTS_DIR = .o