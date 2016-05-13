#-------------------------------------------------
#
# Project created by QtCreator 2015-02-14T11:21:04
#
#-------------------------------------------------

QT		+= xml
QT		-= gui

TARGET = DTK
VERSION = 1.2.0
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
    ServiceList.cpp

HEADERS += DTK.h\
    DTKinternal.h \
    DTK_global.h

macx {
	DEFINES += DCMTK_LOG4CPLUS_HAVE_C99_VARIADIC_MACROS
	QMAKE_MAC_SDK = macosx10.11

	CONFIG(debug, debug|release) {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -stdlib=libc++ -g -O0

		INCLUDEPATH += $$PWD/../lib/DTK-1.2.0-macos/include $$PWD/../dcmj2k
		LIBS += -v -stdlib=libc++ -L$$PWD/../lib/DTK-1.2.0-macos/lib_debug -ldcmtk -L$$PWD/../build-dcmj2k/Debug -ldcmj2k
	} else {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -stdlib=libc++

		INCLUDEPATH += $$PWD/../lib/DTK-1.2.0-macos/include $$PWD/../dcmj2k
		LIBS += -v -stdlib=libc++ -L$$PWD/../lib/DTK-1.2.0-macos/lib -ldcmtk -L$$PWD/../build-dcmj2k/Release -ldcmj2k
	}
}
unix:!macx {
	CONFIG(debug, debug|release) {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -std=c++0x -g -O0

		CONFIG(arm, arm|x86) {
			INCLUDEPATH += $$PWD/../lib/DTK-1.2.0-armv7/include
			LIBS += -v -L$$PWD/../lib/DTK-1.2.0-armv7/lib_debug -ldcmtk
		} else {
			INCLUDEPATH += $$PWD/../lib/DTK-1.2.0-linux/include
			LIBS += -v -L$$PWD/../lib/DTK-1.2.0-linux/lib_debug -ldcmtk
		}
	} else {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -std=c++0x

		CONFIG(arm, arm|x86) {
			INCLUDEPATH += $$PWD/../lib/DTK-1.2.0-armv7/include
			LIBS += -v -L$$PWD/../lib/DTK-1.2.0-armv7/lib -ldcmtk
		} else {
			INCLUDEPATH += $$PWD/../lib/DTK-1.2.0-linux/include
			LIBS += -v -L$$PWD/../lib/DTK-1.2.0-linux/lib -ldcmtk
		}
	}
}
win32 {
	CONFIG(debug, debug|release) {
		CONFIG += warn_off dll
		INCLUDEPATH += "$$PWD\..\lib\DTK-1.2.0-win32\include"
		LIBS += -L"$$PWD\..\lib\DTK-1.2.0-win32\lib_debug" -ldcmtk wsock32.lib netapi32.lib
	} else {
		CONFIG += warn_off dll
		INCLUDEPATH += "$$PWD\..\lib\DTK-1.2.0-win32\include"
		LIBS += -L"$$PWD\..\lib\DTK-1.2.0-win32\lib" -ldcmtk wsock32.lib netapi32.lib
	}
}
