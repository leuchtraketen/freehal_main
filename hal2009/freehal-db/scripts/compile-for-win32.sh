export V=$(cat .version)

rm -rf ../win-compile-freehal-db
mkdir -p ../win-compile-freehal-db 2>&1 >/dev/null


export DIR=$(pwd | sed "s@$(dirname $(pwd))@@g")

echo $DIR

for x in $(ls); do
	rm ../win-compile-freehal-db/${x}
	ln -s ../${DIR}/${x} ../win-compile-freehal-db/
done
rm ../win-compile-freehal-db/*.o

cd ../win-compile-freehal-db

export DEFINES="-D_WIN32_WINNT=0x0500"

export QTPATH=$1
export II=" -I../../JEliza/boost_1_37_0/ -I../perl5 -I../perl5/win32 -I../perl -UMYSWAP -DHAS_HTONL=1 -DHALWINDOWS=1 "

#uic ../freehal-db/ui/freehal.ui | grep -v NMAESPACE > ui_freehal.h
#uic ../freehal-db/ui/changelog.ui | grep -v MNAESPACE > ui_changelog.h
#uic ../freehal-db/ui/db_edit.ui | grep -v MNAESPACE > ui_db_edit.h
#uic ../freehal-db/ui/help.ui | grep -v MNAESPACE > ui_help.h
#uic ../freehal-db/ui/irc_mode.ui | grep -v MNAESPACE  > ui_irc_mode.h
#uic ../freehal-db/ui/options.ui | grep -v MNAESPACE  > ui_options.h
#uic ../freehal-db/ui/startup.ui | grep -v MNAESPACE > ui_startup.h
#uic ../freehal-db/ui/ee.ui | grep -v MNAESPACE > ui_ee.h
#uic ../freehal-db/ui/connect.ui | grep -v MNAESPACE > ui_connect.h
#uic ../freehal-db/ui/view.ui | grep -v MNAESPACE > ui_view.h

/usr/bin/moc-qt4 $II -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -Iinclude/python-win/Include/ -I/usr/include/qt4 -I. -Isrc -I. -I. ../freehal-db/freehal_db.h -o ../freehal-db/moc_freehal_db.cpp

   true \
&& i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o freehal_db.o ../freehal-db/freehal_db.cpp	\
&& i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o moc_freehal_db.cpp.o ../freehal-db/moc_freehal_db.cpp	\
&& i586-mingw32msvc-g++ $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o main.cpp.o ../freehal-db/main.cpp	\
&& i586-mingw32msvc-gcc $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o hal2009-perl5.c.o ../hal2009-perl5.c	\
&& i586-mingw32msvc-gcc $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o hal2009-perl6-dummy.c.o ../hal2009-perl6-dummy.c		\
&& i586-mingw32msvc-gcc $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o sqlite3.c.o ../sqlite3.c		\
&& i586-mingw32msvc-gcc $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o freehal-db.c.o freehal-db.c	\
&& i586-mingw32msvc-gcc $II -Iinclude/ -Ilib/asio/ -O1 -O2 -O3 -I$QTPATH/include -I$QTPATH/include/QtGui -I$QTPATH/include/QtCore -c -pipe  -g -D_REENTRANT -Wall -W  -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -Iinclude/python-win/Include/ -Iqt4-win/mkspecs/win32-g++/ -I. -Isrc -I. -I. -o hal2009-wiki-dummy.c.o ../hal2009-wiki-dummy.c	\
&& true


echo Compiling FreeHAL-DB
i586-mingw32msvc-g++ $II -L../${DIR}/stuff/windows/ -Iinclude/ -static -Ilib/asio/ -O1 -O2 -O3 -o FreeHAL-DB.exe \
	*.o \
	 -L. -L../.. -L$QTPATH/lib/ -lm -lQtXml4 -lQtGui4 -lQtCore4 -lQtUiTools -Iinclude/pthread-win/ -lpthread-win -mthreads -lws2_32 -lmswsock -lwsock32 -L. -lole32 -loleaut32 -lwinmm -luuid -L/usr/lib/gcc/i586-mingw32msvc/4.2.1-sjlj -lstdc++ -lperl510
echo Ok


