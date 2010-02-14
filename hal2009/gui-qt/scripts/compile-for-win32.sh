export V=$(cat .version)

rm -rf ../win-compile
mkdir -p ../win-compile 2>&1 >/dev/null


export DIR=$(pwd | sed "s@$(dirname $(pwd))@@g")

echo $DIR

for x in $(ls); do
	rm ../win-compile/${x}
	ln -s ../${DIR}/${x} ../win-compile/
done

cd ../win-compile

export DEFINES="-D_WIN32_WINNT=0x0500"

export QTPATH=$1
export II=" -I../../JEliza/boost_1_37_0/ "

#uic ../gui-qt/ui/freehal.ui | grep -v NMAESPACE > ui_freehal.h
#uic ../gui-qt/ui/changelog.ui | grep -v MNAESPACE > ui_changelog.h
#uic ../gui-qt/ui/db_edit.ui | grep -v MNAESPACE > ui_db_edit.h
#uic ../gui-qt/ui/help.ui | grep -v MNAESPACE > ui_help.h
#uic ../gui-qt/ui/irc_mode.ui | grep -v MNAESPACE  > ui_irc_mode.h
#uic ../gui-qt/ui/options.ui | grep -v MNAESPACE  > ui_options.h
#uic ../gui-qt/ui/startup.ui | grep -v MNAESPACE > ui_startup.h
#uic ../gui-qt/ui/ee.ui | grep -v MNAESPACE > ui_ee.h
#uic ../gui-qt/ui/connect.ui | grep -v MNAESPACE > ui_connect.h
#uic ../gui-qt/ui/view.ui | grep -v MNAESPACE > ui_view.h

/usr/bin/moc-qt4 $II -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. ../gui-qt/src/gui-qt/FreeHALQt.h -o ../gui-qt/moc_FreeHALQt.cpp
/usr/bin/moc-qt4 $II -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. src/communicate.cpp -o ../gui-qt/moc_communicate.cpp

i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o FreeHALQt.o ../gui-qt/src/gui-qt/FreeHALQt.cpp
i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o moc_FreeHALQt.o ../gui-qt/moc_FreeHALQt.cpp
i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o moc_communicate.o ../gui-qt/moc_communicate.cpp
i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o communicate.o src/communicate.cpp


#i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o freehal.o src/freehal.cpp

#i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -Iinclude/ms-agent/ -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o cMMControl.o ../gui-qt/src/windows/msagent/cMMControl.cpp
#i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -Iinclude/ms-agent/ -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o cAgentSink.o ../gui-qt/src/windows/msagent/cAgentSink.cpp
#i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -Iinclude/ms-agent/ -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o cAgentCommands.o ../gui-qt/src/windows/msagent/cAgentCommands.cpp
#i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -Iinclude/ms-agent/ -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o cAgent.o ../gui-qt/src/windows/msagent/cAgent.cpp

#i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o freehalmanager.o src/freehalmanager.cpp
i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o sentence.o src/sentence.cpp
i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o socketload.o src/socketload.cpp
i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o util.o src/util.cpp
#i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o wikipedia.o src/wikipedia.cpp

i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -Iinclude/ms-agent/ -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o speak_win.o ../gui-qt/src/speech/speak_win.cpp

echo Compiling FreeHALQt
i586-mingw32msvc-g++ $II -L../${DIR}/stuff/windows/ -Iinclude/ -static -Ilib/asio/ -O1 -O2 -O3 -o FreeHAL-QT.exe FreeHALQt.o speak_win.o sentence.o socketload.o util.o moc_FreeHALQt.o -lboost_system-mgw-mt-1_37 ../$DIR/src/boost/* moc_communicate.o communicate.o -L. -L../.. -L$QTPATH/lib/ -lm -lQtXml4 -lQtGui4 -lQtCore4 -lQtUiTools -Iinclude/pthread-win/ -lpthread-win -mthreads -lws2_32 -lmswsock -lwsock32 -L. -lole32 -loleaut32 -lwinmm -luuid -L/usr/lib/gcc/i586-mingw32msvc/4.2.1-sjlj -lstdc++ 
echo Ok


