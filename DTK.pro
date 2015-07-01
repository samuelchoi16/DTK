#-------------------------------------------------
#
# Project created by QtCreator 2015-02-14T11:21:04
#
#-------------------------------------------------

QT       -= gui

TARGET = DTK
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
	CONFIG(debug, debug|release) {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -stdlib=libc++ -g -O0

		INCLUDEPATH += $$PWD/../DTK-1.0.0-macos/include
		LIBS += -v -stdlib=libc++ -L$$PWD/../DTK-1.0.0-macos/lib_debug -ldcmtk
	} else {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -stdlib=libc++

		INCLUDEPATH += $$PWD/../DTK-1.0.0-macos/include
		LIBS += -v -stdlib=libc++ -L$$PWD/../DTK-1.0.0-macos/lib -ldcmtk
	}
}
unix:!macx {
	CONFIG(debug, debug|release) {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -std=c++0x -g -O0

		CONFIG(arm, arm|x86) {
			INCLUDEPATH += $$PWD/../DTK-1.0.0-arm/include
			LIBS += -v -L$$PWD/../DTK-1.0.0-arm/lib_debug -ldcmtk
		} else {
			INCLUDEPATH += $$PWD/../DTK-1.0.0-linux/include
			LIBS += -v -L$$PWD/../DTK-1.0.0-linux/lib_debug -ldcmtk
		}
	} else {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -std=c++0x

		CONFIG(arm, arm|x86) {
			INCLUDEPATH += $$PWD/../DTK-1.0.0-arm/include
			LIBS += -v -L$$PWD/../DTK-1.0.0-arm/lib -ldcmtk
		} else {
			INCLUDEPATH += $$PWD/../DTK-1.0.0-linux/include
			LIBS += -v -L$$PWD/../DTK-1.0.0-linux/lib -ldcmtk
		}
	}
}
win32 {
	CONFIG(debug, debug|release) {
		CONFIG += warn_off dll
		INCLUDEPATH += "$$PWD\..\DTK-1.0.0-win32\include"
		LIBS += -L"$$PWD\..\DTK-1.0.0-win32\lib_debug" -ldcmtk \
				wsock32.lib netapi32.lib
	} else {
		CONFIG += warn_off dll
		INCLUDEPATH += "$$PWD\..\DTK-1.0.0-win32\include"
		LIBS += -L"$$PWD\..\DTK-1.0.0-win32\lib" -ldcmtk \
				wsock32.lib netapi32.lib
	}
}
