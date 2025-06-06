QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AddClubDialog.cpp \
    Club.cpp \
    Event.cpp \
    Mainwindow.cpp \
    Navigation.cpp \
    User.cpp \
    adminClub.cpp \
    adminHome.cpp \
    adminMember.cpp \
    adminNoti.cpp \
    homePage.cpp \
    lboardAll.cpp \
    lgroupchat.cpp \
    lhomepage.cpp \
    lnoti.cpp \
    loginPage.cpp \
    main.cpp \
    mboardpage.cpp \
    mchatpage.cpp \
    mclubpage.cpp \
    meventpage.cpp \
    mgoingpage.cpp \
    mhomepage.cpp \
    mprofilepage.cpp \
    registerPage.cpp \
    searchclubs.cpp

HEADERS += \
    AddClubDialog.h \
    Club.h \
    Event.h \
    Mainwindow.h \
    Navigation.h \
    User.h \
    adminClub.h \
    adminHome.h \
    adminMember.h \
    adminNoti.h \
    database.h \
    homePage.h \
    lboardAll.h \
    lgroupchat.h \
    lhomepage.h \
    lnoti.h \
    loginPage.h \
    mboardpage.h \
    mchatpage.h \
    mclubpage.h \
    meventpage.h \
    mgoingpage.h \
    mhomepage.h \
    mprofilepage.h \
    registerPage.h \
    searchclubs.h

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
