#-------------------------------------------------
#
# Project created by QtCreator 2010-07-20T15:00:50
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_calculatortest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += . ..
SOURCES += tst_calculatortest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

# set up library path
SUT = calculatorcore
SUTDIR = ../$${SUT}
CONFIG(debug, debug|release) {
    SUTOUTDIR = $${SUTDIR}/debug
}
CONFIG(release, debug|release) {
    SUTOUTDIR = $${SUTDIR}/release
}
SUTLIB = $${SUTOUTDIR}/lib$${SUT}.a
PRE_TARGETDEPS += $${SUTLIB}

LIBS += $${SUTLIB}
DESTDIR = $${SUTOUTDIR}

OTHER_FILES += \
    multiply0.log \
    manipulate0.log \
    add0.log

QMAKE_POST_LINK = copy /Y \"$${PWD}/*0.log\" \"$${DESTDIR}\\\"
