#-------------------------------------------------
#
# Project created by QtCreator 2015-02-14T11:21:04
#
#-------------------------------------------------

QT		+= xml
QT		-= gui

TARGET = DTK
VERSION = 1.3.0
TEMPLATE = lib

DEFINES += DTK_LIBRARY HAVE_STL HAVE_STD_STRING
DEFINES -= UNICODE

SOURCES += \
    AppEntity.cpp \
    Association.cpp \
    AssociationListener.cpp \
    AssociationRequestor.cpp \
    Dataset.cpp \
    File.cpp \
    Item.cpp \
    Message.cpp \
    MetaInfo.cpp \
    Service.cpp \
    UIDHelper.cpp \
    DateTime.cpp \
    DTK.cpp \
    NLS.cpp \
    DirRecord.cpp \
    Dir.cpp \
    ServiceList.cpp \
    dcmjp2k/djp2kcodecd.cpp \
    dcmjp2k/djp2kcodece.cpp \
    dcmjp2k/djp2kcparam.cpp \
    dcmjp2k/djp2kdecode.cpp \
    dcmjp2k/djp2kencode.cpp \
    dcmjp2k/djp2krparam.cpp \
    dcmjp2k/djp2kutils.cpp \
    dcmjp2k/djp2kcodecb.cpp

HEADERS += DTK.h\
    DTKinternal.h \
    DTK_global.h \
    dcmjp2k/djp2kcodecd.h \
    dcmjp2k/djp2kcodece.h \
    dcmjp2k/djp2kcparam.h \
    dcmjp2k/djp2kdecode.h \
    dcmjp2k/djp2kdefine.h \
    dcmjp2k/djp2kencode.h \
    dcmjp2k/djp2kerror.h \
    dcmjp2k/djp2krparam.h \
    dcmjp2k/djp2kutil.h \
    dcmjp2k/djp2kcodecb.h \
    dcmjp2k/djp2kinternal.h

macx {
	DEFINES += DCMTK_LOG4CPLUS_HAVE_C99_VARIADIC_MACROS
	QMAKE_MAC_SDK = macosx10.11

	CONFIG(debug, debug|release) {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -stdlib=libc++ -g -O0

		INCLUDEPATH += $$PWD/../lib/dcmtk-3.6.1_20161102-debug/include $$PWD/../lib/openjpeg-v2.1.2-osx-x86_64/include/openjpeg-2.1
		LIBS += -v -stdlib=libc++ -L$$PWD/../lib/dcmtk-3.6.1_20161102-debug/lib -ldcmtk -L$$PWD/../lib/openjpeg-v2.1.2-osx-x86_64/lib -lopenjp2
	} else {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -stdlib=libc++

		INCLUDEPATH += $$PWD/../lib/dcmtk-3.6.1_20161102-release/include $$PWD/../lib/openjpeg-v2.1.2-osx-x86_64/include/openjpeg-2.1
		LIBS += -v -stdlib=libc++ -L$$PWD/../lib/dcmtk-3.6.1_20161102-release/lib -ldcmtk -L$$PWD/../lib/openjpeg-v2.1.2-osx-x86_64/lib -lopenjp2
	}
}
unix:!macx {
	CONFIG(debug, debug|release) {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -std=c++0x -g -O0

		CONFIG(arm, arm|x86) {
			INCLUDEPATH += $$PWD/../lib/DTK-1.2.1-armv7/include
			LIBS += -v -L$$PWD/../lib/DTK-1.2.1-armv7/lib_debug -ldcmtk
		} else {
			INCLUDEPATH += $$PWD/../lib/DTK-1.2.1-linux/include
			LIBS += -v -L$$PWD/../lib/DTK-1.2.1-linux/lib_debug -ldcmtk
		}
	} else {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -std=c++0x

		CONFIG(arm, arm|x86) {
			INCLUDEPATH += $$PWD/../lib/DTK-1.2.1-armv7/include
			LIBS += -v -L$$PWD/../lib/DTK-1.2.1-armv7/lib -ldcmtk
		} else {
			INCLUDEPATH += $$PWD/../lib/DTK-1.2.1-linux/include
			LIBS += -v -L$$PWD/../lib/DTK-1.2.1-linux/lib -ldcmtk
		}
	}
}
win32 {
	DEFINES += _USING_V110_SDK71_
	QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.1
	QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.1

	CONFIG(debug, debug|release) {
		CONFIG += warn_off dll
		INCLUDEPATH += "$$PWD\..\lib\DTK-1.3.0-win32\include" "$$PWD\..\lib\openjpeg-v2.1.2-windows-x86\include\openjpeg-2.1"
		LIBS += -L"$$PWD\..\lib\DTK-1.3.0-win32\lib_debug" -ldcmtk wsock32.lib netapi32.lib -L"$$PWD\..\lib\openjpeg-v2.1.2-windows-x86\lib" -lopenjp2
	} else {
		CONFIG += warn_off dll
		INCLUDEPATH += "$$PWD\..\lib\DTK-1.3.0-win32\include" "$$PWD\..\lib\openjpeg-v2.1.2-windows-x86\include\openjpeg-2.1"
		LIBS += -L"$$PWD\..\lib\DTK-1.3.0-win32\lib" -ldcmtk wsock32.lib netapi32.lib -L"$$PWD\..\lib\openjpeg-v2.1.2-windows-x86\lib" -lopenjp2
	}
}

DISTFILES += \
    ../build-DTK/publish-msvc2013-debug.bat \
    ../build-DTK/publish-msvc2013-release.bat
