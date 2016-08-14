TEMPLATE = lib
TARGET   = curl_cpp
CONFIG += c++14

QT = core

mac* | linux*{
	CONFIG(release, debug|release):CONFIG += Release
	CONFIG(debug, debug|release):CONFIG += Debug
}

Release:OUTPUT_DIR=release
Debug:OUTPUT_DIR=debug

DESTDIR  = ../bin/$${OUTPUT_DIR}
OBJECTS_DIR = ../build/$${OUTPUT_DIR}/$${TARGET}
MOC_DIR     = ../build/$${OUTPUT_DIR}/$${TARGET}
UI_DIR      = ../build/$${OUTPUT_DIR}/$${TARGET}
RCC_DIR     = ../build/$${OUTPUT_DIR}/$${TARGET}

contains(QMAKE_TARGET.arch, x86_64) {
	ARCHITECTURE = x64
} else {
	ARCHITECTURE = x86
}

win*{
	QMAKE_CXXFLAGS += /MP /wd4251
	DEFINES += WIN32_LEAN_AND_MEAN NOMINMAX
	QMAKE_CXXFLAGS_WARN_ON = /W4
}

linux*|mac*{
	QMAKE_CXXFLAGS += -pedantic-errors
	QMAKE_CFLAGS += -pedantic-errors
	QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-c++11-extensions -Wno-local-type-template-args -Wno-deprecated-register

	Release:DEFINES += NDEBUG=1
	Debug:DEFINES += _DEBUG
}

win32*:!*msvc2012:*msvc*:!*msvc2010:*msvc* {
	QMAKE_CXXFLAGS += /FS
}

win*{
	*msvc2013*{
		TOOLSET = v120
	}

	*msvc2015*{
		TOOLSET = v140
	}
}

DEFINES += CURL_STATICLIB

LIBS += -L$${PWD}/../3rdparty/libcurl/lib
Debug:LIBS += -llibcurl_a_debug
Release:LIBS += -llibcurl_a

LIBS += -L$${DESTDIR} -lcpputils

DEFINES += BUILDING_LIBCURL_CPP

INCLUDEPATH += \
	../cpputils \
	../cpp-template-utils \
	../3rdparty/libcurl/include

HEADERS += \
	src/cwebdownloader.h \
	src/export.h

SOURCES += \
    src/cwebdownloader.cpp \
    src/init_library.cpp

