HEADERS += ./common/direction.h \
    ./common/snake.h \
    ./mainwidget.h \
    ./client/board.h \
    ./client/clientgame.h \
    ./client/clientnetwork.h \
    ./client/clientsocketadapter.h \
    ./connectionwidget.h \
    ./settingswidget.h \
    ./server/serversocketadapter.h \
    ./server/servernetwork.h \
    ./server/servergame.h \
    ./common/socketadapter.h \
    ./common/parser.h \
    ./common/isocketadapter.h
SOURCES += ./connectionwidget.cpp \
    ./settingswidget.cpp \
    ./client/board.cpp \
    ./client/clientgame.cpp \
    ./client/clientnetwork.cpp \
    ./client/clientsocketadapter.cpp \
    ./common/direction.cpp \
    ./common/parser.cpp \
    ./common/snake.cpp \
    ./common/socketadapter.cpp \
    ./main.cpp \
    ./mainwidget.cpp \
    ./server/servergame.cpp \
    ./server/servernetwork.cpp
RESOURCES += snakegame.qrc

