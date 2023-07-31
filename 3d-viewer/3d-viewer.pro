QT       += core gui opengl openglwidgets widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(QtGifImage/src/gifimage/qtgifimage.pri)

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    glmodule.cpp \
    main.cpp \
    mainwindow.cpp \
    parser.c

HEADERS += \
    glmodule.h \
    mainwindow.h \
    parser.h

ICON = img/atf0y-vvqnj.icns

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    19.jpg \
    3D.ico

RESOURCES += \
    res.qrc

