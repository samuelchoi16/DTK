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
    Dir.cpp

HEADERS += DTK.h\
    DTKinternal.h \
    DTK_global.h

unix: {
	target.path = /usr/lib
	INSTALLS += target

	QMAKE_CXXFLAGS += -stdlib=libc++

	INCLUDEPATH += /usr/local/include
	LIBS += -v -stdlib=libc++ -L/usr/local/lib -loflog -lofstd -lcharls -ldcmdata -ldcmimgle -ldcmimage -ldcmjpeg -lijg8 -lijg12 -lijg16 -ldcmjpls -ldcmnet -lz -liconv
}
win32: {
	CONFIG += warn_off dll
	INCLUDEPATH += "..\dcmtk-3.6.1_20150217-VS12_MD_debug\include" "..\dcmtk-3.6.0-win32-i386-support_MD\zlib-1.2.5\include"
	LIBS += -L"..\dcmtk-3.6.1_20150217-VS12_MD_debug\lib" -loflog -lofstd -lcharls -ldcmdata -ldcmimgle -ldcmimage -ldcmjpeg -lijg8 -lijg12 -lijg16 -ldcmjpls -ldcmnet \
			-L"..\dcmtk-3.6.0-win32-i386-support_MD\zlib-1.2.5\lib" -lzlib_d \
			wsock32.lib netapi32.lib
}

# libdcmdata.a	libdcmimgle.a	libdcmnet.a	libdcmrt.a	libdcmwlm.a
# libdcmdsig.a	libdcmjpeg.a	libdcmpstat.a	libdcmsr.a
# libdcmimage.a	libdcmjpls.a	libdcmqrdb.a	libdcmtls.a
