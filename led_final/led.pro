QT       += core gui network mqtt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connnected.cpp \
    events.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    connnected.h \
    events.h \
    fsmp_board/fsmpBeeper.h \
    fsmp_board/fsmpCamera.h \
    fsmp_board/fsmpEvents.h \
    fsmp_board/fsmpFan.h \
    fsmp_board/fsmpLed.h \
    fsmp_board/fsmpLight.h \
    fsmp_board/fsmpTempHum.h \
    fsmp_board/fsmpVibrator.h \
    widget.h

FORMS += \
    connnected.ui \
    events.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc
