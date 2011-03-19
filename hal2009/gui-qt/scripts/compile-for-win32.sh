export V=$(cat .version)

rm -rf ../win-compile
mkdir -p ../win-compile 2>&1 >/dev/null


export DIR=$(pwd | sed "s@$(dirname $(pwd))@@g")

echo $DIR

for x in *
do
	rm -f ../win-compile/${x}
	ln -s ../${DIR}/${x} ../win-compile/
done

cd ../win-compile

export DEFINES="-D_WIN32_WINNT=0x0500"

export QTPATH=../../windows/qt-4.6.2
export II=" -I../../windows/boost_1_42_0"

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

(/usr/bin/moc-qt4 $II -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. ../gui-qt/src/gui-qt/FreeHALQt.h -o ../gui-qt/moc_FreeHALQt.cpp  || exit 0) 2>&1 | grep -v Warnung
(/usr/bin/moc-qt4 $II -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. src/communicate.cpp -o ../gui-qt/moc_communicate.cpp  || exit 0) 2>&1 | grep -v Warnung

clear; echo ../gui-qt/src/gui-qt/FreeHALQt.cpp
(i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o FreeHALQt.o ../gui-qt/src/gui-qt/FreeHALQt.cpp  || exit 0) 2>&1 | grep -v Warnung

clear; echo ../gui-qt/moc_FreeHALQt.cpp
(i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o moc_FreeHALQt.o ../gui-qt/moc_FreeHALQt.cpp  || exit 0) 2>&1 | grep -v Warnung

clear; echo ../gui-qt/moc_communicate.cpp
(i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o moc_communicate.o ../gui-qt/moc_communicate.cpp  || exit 0) 2>&1 | grep -v Warnung

clear; echo src/communicate.cpp
(i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o communicate.o src/communicate.cpp  || exit 0) 2>&1 | grep -v Warnung


clear; echo src/sentence.cpp
(i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o sentence.o src/sentence.cpp  || exit 0) 2>&1 | grep -v Warnung

clear; echo src/socketload.cpp
(i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o socketload.o src/socketload.cpp  || exit 0) 2>&1 | grep -v Warnung

clear; echo src/util.cpp
(i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o util.o src/util.cpp  || exit 0) 2>&1 | grep -v Warnung

clear; echo ../gui-qt/src/speech/speak_win.cpp
(i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -Iinclude/ms-agent/ -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o speak_win.o ../gui-qt/src/speech/speak_win.cpp  || exit 0) 2>&1 | grep -v Warnung

clear; echo ../gui-qt/src/special_normal.cpp
(i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -Iinclude/ms-agent/ -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o special_normal.o ../gui-qt/src/special_normal.cpp  || exit 0) 2>&1 | grep -v Warnung

clear; echo ../gui-qt/src/special_onlinedemo.cpp
(i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -Iinclude/ms-agent/ -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. -o special_onlinedemo.o ../gui-qt/src/special_onlinedemo.cpp  || exit 0) 2>&1 | grep -v Warnung

clear
echo Compiling FreeHALQt
i586-mingw32msvc-g++ $II -L../../windows/lib/ ../../windows/lib/boost_system.lib ../../windows/lib/boost_thread_win32.lib -Iinclude/ -static -Ilib/asio/ -O1 -O2 -O3 -o FreeHAL-QT.exe FreeHALQt.o special_normal.o speak_win.o sentence.o socketload.o util.o moc_FreeHALQt.o moc_communicate.o communicate.o -L. -L../.. -L$QTPATH/lib/ -lm -lQtXml4 -lQtGui4 -lQtCore4 -lQtUiTools -Iinclude/pthread-win/ -lpthread-win -mthreads -lws2_32 -lmswsock -lwsock32 -L. -lole32 -loleaut32 -lwinmm -luuid -L/usr/lib/gcc/i586-mingw32msvc/4.2.1-sjlj -lstdc++ -lboost_system -lboost_thread_win32
i586-mingw32msvc-g++ $II -L../../windows/lib/ ../../windows/lib/boost_system.lib ../../windows/lib/boost_thread_win32.lib -Iinclude/ -static -Ilib/asio/ -O1 -O2 -O3 -o FreeHAL-Online-Demo.exe FreeHALQt.o special_onlinedemo.o speak_win.o sentence.o socketload.o util.o moc_FreeHALQt.o moc_communicate.o communicate.o -L. -L../.. -L$QTPATH/lib/ -lm -lQtXml4 -lQtGui4 -lQtCore4 -lQtUiTools -Iinclude/pthread-win/ -lpthread-win -mthreads -lws2_32 -lmswsock -lwsock32 -L. -lole32 -loleaut32 -lwinmm -luuid -L/usr/lib/gcc/i586-mingw32msvc/4.2.1-sjlj -lstdc++ -lboost_system -lboost_thread_win32
i586-mingw32msvc-strip FreeHAL-QT.exe
i586-mingw32msvc-strip FreeHAL-Online-Demo.exe
echo Ok


mv FreeHAL-*.exe ../
cd ..
rm -rf ./win-compile
mkdir ./win-compile
mv FreeHAL-*.exe ./win-compile/

cd gui-qt/
echo Upload...
for x in freehal.net cloud-2.freehal.net
do
	scp -C ../win-compile/FreeHAL-QT.exe tobias@cloud-2.freehal.net:/disk/compiler/add2/source/hal2009/FreeHAL-QT.exe
	scp -C ../win-compile/FreeHAL-Online-Demo.exe tobias@cloud-2.freehal.net:/disk/compiler/add2/source/hal2009/
done
echo Upload done.
