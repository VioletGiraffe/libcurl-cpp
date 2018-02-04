TEMPLATE = lib
TARGET   = curl_cpp
CONFIG += c++14 staticlib

QT = core

mac* | linux*{
	CONFIG(release, debug|release):CONFIG += Release
	CONFIG(debug, debug|release):CONFIG += Debug
}

contains(QT_ARCH, x86_64) {
	ARCHITECTURE = x64
} else {
	ARCHITECTURE = x86
}

exists(../local_settings.pri){
	include(../local_settings.pri)
}

Release:OUTPUT_DIR=release/$${ARCHITECTURE}
Debug:OUTPUT_DIR=debug/$${ARCHITECTURE}

DESTDIR  = ../bin/$${OUTPUT_DIR}
OBJECTS_DIR = ../build/$${OUTPUT_DIR}/$${TARGET}
MOC_DIR     = ../build/$${OUTPUT_DIR}/$${TARGET}
UI_DIR      = ../build/$${OUTPUT_DIR}/$${TARGET}
RCC_DIR     = ../build/$${OUTPUT_DIR}/$${TARGET}

win*{
	QMAKE_CXXFLAGS += /MP /wd4251
	QMAKE_CXXFLAGS += /FS

	DEFINES += WIN32_LEAN_AND_MEAN NOMINMAX
	QMAKE_CXXFLAGS_WARN_ON = /W4

	QMAKE_LFLAGS += /DEBUG:FASTLINK

	Debug:QMAKE_LFLAGS += /INCREMENTAL
	Release:QMAKE_LFLAGS += /OPT:REF /OPT:ICF
}

linux*|mac*{
	QMAKE_CXXFLAGS += -pedantic-errors
	QMAKE_CFLAGS += -pedantic-errors
	QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-c++11-extensions -Wno-local-type-template-args -Wno-deprecated-register

	Release:DEFINES += NDEBUG=1
	Debug:DEFINES += _DEBUG
}

!CONFIG(staticlib): DEFINES += LIBCURL_CPP_DYNAMIC_LIB

Release:LIBS += -L$${VCPKG_INSTALLS_PATH}/lib
Debug:LIBS += -L$${VCPKG_INSTALLS_PATH}/debug/lib
LIBS += -llibcurl

LIBS += -L$${DESTDIR} -lcpputils

DEFINES += BUILDING_LIBCURL_CPP

INCLUDEPATH += \
	../cpputils \
	../cpp-template-utils \
	$${VCPKG_INSTALLS_PATH}/include

HEADERS += \
	src/cwebdownloader.h \
	src/export.h

SOURCES += \
	src/cwebdownloader.cpp \
	src/init_library.cpp

