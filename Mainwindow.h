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
#include "lhomepage.h"
#include "lgroupchat.h"
#include "lnoti.h"
#include "lboardAll.h"

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

    // Leader page navigation methods
    void showLeaderHomePage();
    void showLeaderGroupChat();
    void showLeaderNotifications();
    void showLeaderboard();

    // Authentication handlers
    void handleRegisterSuccess(int userId);
    void handleLoginSuccess(int userId);

private:
    void initializeHomePage();
    void initializeRegisterPage();
    void initializeLoginPage();
    void initializeAdminHomePage();
    void initializeAdminMemberPage();
    void initializeAdminClubPage();
    void initializeAdminNotiPage();

    // Initialize leader pages
    void initializeLeaderHomePage();
    void initializeLeaderGroupChat();
    void initializeLeaderNotifications();
    void initializeLeaderboardPage();

    // Cleanup method
    void cleanupUnusedPages(QWidget* currentPage);

    // Check user role
    bool isUserLeader(int userId);

    // Page pointers, initialized to nullptr
    HomePage* homePage;
    RegisterPage* registerPage;
    LoginPage* loginPage;
    AdminHome* adminHome;
    AdminMember* adminMember;
    AdminClub* adminClub;
    AdminNoti* adminNoti;

    // Leader page pointers
    LHomepage* leaderHomePage;
    LGroupChat* leaderGroupChat;
    LNoti* leaderNotifications;
    LeaderboardAll* leaderboardPage;

    // Currently logged in user ID (0 if not logged in)
    int currentUserId;
};

#endif // MAINWINDOW_H
