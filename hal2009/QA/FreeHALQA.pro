# -------------------------------------------------
# Project created by QtCreator 2009-05-28T17:52:24
# -------------------------------------------------
QT += opengl
TARGET = FreeHALQA
TEMPLATE = app
LIBS += -lm \
    -lperl
SOURCES += ../sqlite3.c \
    main.cpp \
    qa.cpp \
    qa-c.c \
    ../hal2009-perl5.c \
    ../hal2009-perl6-dummy.c
HEADERS += qa.h
FORMS += qa.ui
