######################################################################
# Automatically generated by qmake (3.0) Fri Feb 17 12:20:31 2017
######################################################################

TEMPLATE = app
TARGET = DrawingBoard
INCLUDEPATH += . drawing_lib

# Input
HEADERS += drawing_lib/com_interface.h \
           drawing_lib/emitter_board.h \
           drawing_lib/receiver_board.h
SOURCES += main.cpp \
           drawing_lib/com_interface.cpp \
           drawing_lib/emitter_board.cpp \
           drawing_lib/receiver_board.cpp
QT += widgets
CONFIG += c++11
LIBS += -L/usr/local/include -lwiringPi