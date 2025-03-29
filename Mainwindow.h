#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "homePage.h"
#include "registerPage.h"
#include "loginPage.h"
#include "adminHome.h"
#include "adminMember.h"
#include "adminClub.h"
#include "adminNoti.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation methods
    void showHomePage();
    void showRegisterPage();
    void showLoginPage();
    void showAdminHomePage();
    void showAdminMemberPage();
    void showAdminClubPage();
    void showAdminNotiPage();

private:
    void initializeHomePage();
    void initializeRegisterPage();
    void initializeLoginPage();
    void initializeAdminHomePage();
    void initializeAdminMemberPage();
    void initializeAdminClubPage();
    void initializeAdminNotiPage();

    // Cleanup method
    void cleanupUnusedPages(QWidget* currentPage);

    // Page pointers, initialized to nullptr
    HomePage* homePage;
    RegisterPage* registerPage;
    LoginPage* loginPage;
    AdminHome* adminHome;
    AdminMember* adminMember;
    AdminClub* adminClub;
    AdminNoti* adminNoti;
};

#endif // MAINWINDOW_H
