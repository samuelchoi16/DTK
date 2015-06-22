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

		INCLUDEPATH += $$PWD/../dcmtk-3.6.1_20150217-macos_debug/include
		LIBS += -v -stdlib=libc++ -L$$PWD/../dcmtk-3.6.1_20150217-macos_debug/lib -loflog -lofstd -lcharls -ldcmdata -ldcmimgle -ldcmimage -ldcmjpeg -lijg8 -lijg12 -lijg16 -ldcmjpls -ldcmnet -lz -liconv
	} else {
		target.path = /usr/lib
		INSTALLS += target

		QMAKE_CXXFLAGS += -stdlib=libc++

		INCLUDEPATH += $$PWD/../dcmtk-3.6.1_20150217-macos/include
		LIBS += -v -stdlib=libc++ -L$$PWD/../dcmtk-3.6.1_20150217-macos/lib -loflog -lofstd -lcharls -ldcmdata -ldcmimgle -ldcmimage -ldcmjpeg -lijg8 -lijg12 -lijg16 -ldcmjpls -ldcmnet -lz -liconv
	}
}
win32 {
	CONFIG(debug, debug|release) {
		CONFIG += warn_off dll
		INCLUDEPATH += "$$PWD\..\dcmtk-3.6.1_20150217-VS12_MD_debug\include" "$$PWD\..\dcmtk-3.6.0-win32-i386-support_MD\zlib-1.2.5\include"
		LIBS += -L"$$PWD\..\dcmtk-3.6.1_20150217-VS12_MD_debug\lib" -loflog -lofstd -lcharls -ldcmdata -ldcmimgle -ldcmimage -ldcmjpeg -lijg8 -lijg12 -lijg16 -ldcmjpls -ldcmnet \
				-L"$$PWD\..\dcmtk-3.6.0-win32-i386-support_MD\zlib-1.2.5\lib" -lzlib_d \
				wsock32.lib netapi32.lib
	} else {
		CONFIG += warn_off dll
		INCLUDEPATH += "$$PWD\..\dcmtk-3.6.1_20150217-VS12_MD\include" "$$PWD\..\dcmtk-3.6.0-win32-i386-support_MD\zlib-1.2.5\include"
		LIBS += -L"$$PWD\..\dcmtk-3.6.1_20150217-VS12_MD\lib" -loflog -lofstd -lcharls -ldcmdata -ldcmimgle -ldcmimage -ldcmjpeg -lijg8 -lijg12 -lijg16 -ldcmjpls -ldcmnet \
				-L"$$PWD\..\dcmtk-3.6.0-win32-i386-support_MD\zlib-1.2.5\lib" -lzlib_o \
				wsock32.lib netapi32.lib
	}
}
