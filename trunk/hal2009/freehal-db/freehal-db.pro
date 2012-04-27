# -------------------------------------------------
# Project created by QtCreator 2009-08-22T11:14:24
# -------------------------------------------------
QMAKE_CXXFLAGS += -g \
    -O1 \
    -O2 \
    -O3 \
    -I.. \
    -I../perl \
    -I../perl5
QMAKE_CFLAGS += -g \
    -O1 \
    -O2 \
    -O3 \
    -I.. \
    -I../perl \
    -I../perl5
QMAKE_LFLAGS += -lm -lcrypt -lpthread -lperl -ldl
TARGET = freehal-db
TEMPLATE = app
SOURCES += main.cpp \
    freehal_db.cpp \
    freehal-db.c \
    ../hal2009-perl5.c \
    ../hal2009-perl6-dummy.c \
    ../sqlite3.c \
    ../hal2009-wiki-dummy.c
HEADERS += freehal_db.h
FORMS += freehal_db.ui
