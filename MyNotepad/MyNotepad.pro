QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    codeeditor.cpp \
    finddialog.cpp \
    main.cpp \
    mainwindow.cpp \
    replacedialog.cpp

HEADERS += \
    aboutdialog.h \
    codeeditor.h \
    finddialog.h \
    mainwindow.h \
    replacedialog.h

FORMS += \
    aboutdialog.ui \
    finddialog.ui \
    mainwindow.ui \
    replacedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    notepad.qrc

DISTFILES += \
    img/cut.png \
    img/cut.png \
    img/delete.png \
    img/find.png \
    img/find.png \
    img/font.png \
    img/font.png \
    img/goto.png \
    img/goto.png \
    img/help.png \
    img/logo.png \
    img/logo.png \
    img/new.png \
    img/new.png \
    img/next.png \
    img/open.png \
    img/open.png \
    img/paste.png \
    img/paste.png \
    img/print.png \
    img/print.png \
    img/redo.png \
    img/redo.png \
    img/replace.png \
    img/replace.png \
    img/save.png \
    img/save.png \
    img/saveas.png \
    img/saveas.png \
    img/status.png \
    img/status.png \
    img/statusbar.png \
    img/tool.png \
    img/tool.png \
    img/toolbar.png \
    img/undo.png \
    img/undo.png \
    img/wrap.png
