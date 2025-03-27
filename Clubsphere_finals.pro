QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Mainwindow.cpp \
    Navigation.cpp \
    adminClub.cpp \
    adminHome.cpp \
    adminMember.cpp \
    adminNoti.cpp \
    homePage.cpp \
    loginPage.cpp \
    main.cpp \
    registerPage.cpp

HEADERS += \
    Mainwindow.h \
    Navigation.h \
    adminClub.h \
    adminHome.h \
    adminMember.h \
    adminNoti.h \
    database.h \
    homePage.h \
    loginPage.h \
    registerPage.h

FORMS += \
    homePage.ui

# RESOURCES += resources.qrc
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    .gitignore
