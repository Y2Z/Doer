VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_PATCH = 0

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_PATCH=$$VERSION_PATCH"

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}

QT          += core gui widgets
CONFIG      += c++11
TEMPLATE     = app

SRC_DIR      = src
INC_DIR      = inc
FORMS_DIR    = ui

OBJECTS_DIR  = .objs
MOC_DIR      = .mocs
UI_DIR       = .uis
RCC_DIR      = .qrcs

DESTDIR      = bin
TARGET       = doer

INCLUDEPATH += $${INC_DIR}

SOURCES     += src/main.cpp \
               src/mainwindow.cpp \
               src/traymenu.cpp \
               src/runguard.cpp

HEADERS     += inc/mainwindow.h \
               inc/traymenu.h \
               inc/runguard.h

FORMS       += ui/mainwindow.ui

RESOURCES    = res/resources.qrc

OTHER_FILES += dist/$${TARGET}.desktop \
               res/images/background.png \
               res/images/$${TARGET}.svg \
               res/images/tray.png \
               res/styles/default.qss

QMAKE_CLEAN += -r $${DESTDIR}/$${TARGET}

unix:!mac {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }
    BINDIR = $${PREFIX}/bin
    DATADIR =$${PREFIX}/share

    target.path = $${BINDIR}

    INSTALLS += target

    desktop.path = $${DATADIR}/applications
    eval(desktop.files += dist/$${TARGET}.desktop)

    INSTALLS += desktop

    icon.path = $${DATADIR}/icons/hicolor/scalable/apps
    eval(icon.files += res/images/$${TARGET}.svg)

    INSTALLS += icon
}
