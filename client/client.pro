QT       += core gui
QT       += network
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    player.cpp

HEADERS += \
    mainwindow.h \
    player.h


#INCLUDEPATH += /usr/local/lib/qca-qt5.framework/Versions/2/Headers/

FORMS += \
    mainwindow.ui \
    player.ui

TRANSLATIONS += \
    client_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += qca2-qt5
