CONFIG += qt \
    thread \
    phonon
CONFIG += uitools
QT += network
QMAKE_CXXFLAGS += -g \
    -O1 \
    -O2 \
    -O3 \
    -Ilib/asio/
QMAKE_LFLAGS += -g \
    -O1 \
    -O2 \
    -O3 \
    -Ilib/asio/
win32 { 
    QMAKE_LFLAGS += -L. \
        -lwinmm \
        -luuid \
        -lws2_32 \
        -lmswsock \
        -lwsock32 \
        -lole32 \
        -DBOOST_WINDOWS=1
    QMAKE_CXXFLAGS += -g \
        -O1 \
        -O2 \
        -O3 \
        -Ilib/asio/ \
        -IZ:\temp\Symlink-data\workspace\FreeHAL\gui-qt\lib\asio
    QMAKE_LFLAGS += -lws2_32 \
        -g \
        -O1 \
        -O2 \
        -O3 \
        -Ilib/asio/ \
        -IZ:\temp\Symlink-data\workspace\FreeHAL\gui-qt\lib\asio
}
unix { 
    INCLUDEPATH += /usr/include/python2.5/
    INCLUDEPATH += /usr/include/qt4/
    LIBS += -lm \
        -lboost_thread-mt \
        -lboost_system-mt
    TARGET = bin/freehal
    SOURCES += src/speech/speak_linux.cpp
    QMAKE_CXXFLAGS += -g \
        -O1 \
        -Ilib/ \
        -Ilib/boost
    QMAKE_LFLAGS += -g \
        -O1 \
        -O2 \
        -O3 \
        -Ilib/ \
        -Ilib/boost
    QMAKE_CXXFLAGS += -g \
        -O1 \
        -Ilib/asio/ \
        -Dlinux=1
    QMAKE_LFLAGS += -g \
        -O1 \
        -Ilib/asio/ \
        -Dlinux=1
}
macx:QMAKE_CXXFLAGS += -I../python/Include/
TEMPLATE = app
DEPENDPATH += . \
    include \
    src/gui-qt \
    src/modules
INCLUDEPATH += . \
    src
MOCSRC += src/gui-qt/FreeHALQt.cpp
MOCSRC += src/communicate.cpp
MOCSRC += src/socketload.cpp
TRANSLATIONS = freehal_de.ts \
    freehal_en.ts \
    freehal_sp.ts
FORMS += ui/freehal.ui \
    ui/view.ui \
    ui/more-info.ui
FORMS += ui/options.ui
FORMS += 
FORMS += ui/help.ui
FORMS += ui/changelog.ui
FORMS += ui/startup.ui
FORMS += ui/ee.ui
FORMS += ui/connect.ui
FORMS += ui/view.ui
FORMS += 

# Input
HEADERS += src/gui-qt/FreeHALQt.h \
    src/communicate.h \
    src/socketload.h
SOURCES += src/sentence.cpp \
    src/socketload.cpp \
    src/util.cpp \
    src/communicate.cpp \
    src/gui-qt/FreeHALQt.cpp
OTHER_FILES += ui/view.png \
    ui/logo.png \
    ui/bs.gif
RESOURCES += freehalqt.qrc
