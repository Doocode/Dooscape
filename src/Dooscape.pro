#-------------------------------------------------
#
# Project created by QtCreator 2015-03-22T07:44:46
#
#-------------------------------------------------

QT += widgets webenginewidgets network

TARGET = dooscape
TEMPLATE = app

win32 {
    QT += winextras
    CONFIG += windeployqt
    TARGET = Dooscape
}

macx {
    # Tell Qt to use Mac OS X 10.9 SDK & our app icon
    QMAKE_MAC_SDK = macosx10.9
    ICON = icon.icns
}

TRANSLATIONS = lang_fr.ts\

SOURCES += main.cpp\
        mainwindow.cpp \
    browserwindow.cpp \
    configwindow.cpp \
    addressbar.cpp \
    webview.cpp \
    tabbar.cpp \
    skinmanager.cpp \
    uix.cpp \
    qprogressindicator.cpp \
    lockscreen.cpp \
    fieldeditor.cpp \
    readinglist.cpp \
    historypanel.cpp \
    bookmarkspanel.cpp \
    downloadspanel.cpp \
    menupanel.cpp \
    aboutsoft.cpp \
    screenshots.cpp \
    panelview.cpp \
    application.cpp \
    introducer.cpp

HEADERS  += mainwindow.h \
    browserwindow.h \
    configwindow.h \
    addressbar.h \
    webview.h \
    tabbar.h \
    skinmanager.h \
    uix.h \
    qprogressindicator.h \
    lockscreen.h \
    fieldeditor.h \
    readinglist.h \
    historypanel.h \
    bookmarkspanel.h \
    downloadspanel.h \
    menupanel.h \
    aboutsoft.h \
    screenshots.h \
    panelview.h \
    application.h \
    introducer.h

RESOURCES += \
    res.qrc

RC_FILE = res.rc

DISTFILES += \
    res.rc
