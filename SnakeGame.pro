TEMPLATE = app
TARGET = SnakeGame
DESTDIR = ./Win32/Release
QT += core network widgets gui
CONFIG += release c++11
DEFINES += QT_DLL QT_NETWORK_LIB QT_WIDGETS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Release
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(SnakeGame.pri)

