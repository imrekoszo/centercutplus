#-------------------------------------------------
#
# Project created by QtCreator 2010-08-02T16:43:13
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_centercutplusteststest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


INCLUDEPATH += . ..
SOURCES += tst_centercutplusteststest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

# set up library path
SUT = dsp_centercutplus
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
