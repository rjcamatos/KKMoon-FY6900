QT       += core gui widgets serialport

CONFIG += c++11

VERSION = 0.1

RC_ICONS = icon.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    protocol.cpp \
    readwrite.cpp \
    waveform_editor.cpp \
    mainwindow.cpp


HEADERS += \
    mainwindow.h \
    protocol.h \
    readwrite.h \
    types.h \
    waveform_editor.h


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ScreenShot-01.jpg \
    ScreenShot-02.jpg \
    icon.ico \
    icon.png
