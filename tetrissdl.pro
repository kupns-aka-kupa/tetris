TARGET = tetrissdl
TEMPLATE = app
DESTDIR = $$PWD/bin

OBJECTS_DIR = $$PWD/build

HEADERS = \
    $$PWD/include/view.h \
    $$PWD/include/board.h \
    $$PWD/include/block.h \
    $$PWD/include/events.h \

SOURCES = \
    $$PWD/src/main.cpp \
    $$PWD/src/view/view.cpp \
    $$PWD/src/board/board.cpp \
    $$PWD/src/block/block.cpp \
    $$PWD/src/events/events.cpp \

INCLUDEPATH = \
    $$PWD/src/view \
    $$PWD/src/board \
    $$PWD/src/block \
    $$PWD/src/events \
    $$PWD/include \

LIBS = \
    -lSDL2
