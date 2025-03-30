#include "mainwindow.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    homePage(nullptr), registerPage(nullptr), loginPage(nullptr),
    adminHome(nullptr), adminMember(nullptr), adminClub(nullptr), adminNoti(nullptr),
    leaderHomePage(nullptr), leaderGroupChat(nullptr), leaderNotifications(nullptr), leaderboardPage(nullptr),
    currentUserId(0)
{
    // Set initial window properties
    this->setWindowTitle("Club Management System");
    this->resize(375, 800);

    // Initialize only the home page on startup
    initializeHomePage();
    homePage->show();
}

MainWindow::~MainWindow()
{
    // Clean up any pages that might still exist
    if (homePage) delete homePage;
    if (registerPage) delete registerPage;
    if (loginPage) delete loginPage;
    if (adminHome) delete adminHome;
    if (adminMember) delete adminMember;
    if (adminClub) delete adminClub;
    if (adminNoti) delete adminNoti;
    if (leaderHomePage) delete leaderHomePage;
    if (leaderGroupChat) delete leaderGroupChat;
    if (leaderNotifications) delete leaderNotifications;
    if (leaderboardPage) delete leaderboardPage;
}

// Initialize admin pages
void MainWindow::initializeHomePage()
{
    if (!homePage) {
        homePage = new HomePage();
        // Connect HomePage signals
        connect(homePage, &HomePage::openRegisterPage, this, &MainWindow::showRegisterPage);
        connect(homePage, &HomePage::openLoginPage, this, &MainWindow::showLoginPage);
    }
}

void MainWindow::initializeRegisterPage()
{
    if (!registerPage) {
        registerPage = new RegisterPage();
        // Connect RegisterPage signals
        connect(registerPage, &RegisterPage::navigateToHome, this, &MainWindow::showHomePage);
        connect(registerPage, &RegisterPage::openadminHome, this, &MainWindow::handleRegisterSuccess);
    }
}

void MainWindow::initializeLoginPage()
{
    if (!loginPage) {
        loginPage = new LoginPage();
        // Connect LoginPage signals
        connect(loginPage, &LoginPage::navigateToHome, this, &MainWindow::showHomePage);
        connect(loginPage, &LoginPage::openadminHome, this, &MainWindow::handleLoginSuccess);
    }
}

void MainWindow::initializeAdminHomePage()
{
    if (!adminHome) {
        adminHome = new AdminHome();
        // Connect AdminHome signals
        connect(adminHome, &AdminHome::navigateToMembers, this, &MainWindow::showAdminMemberPage);
        connect(adminHome, &AdminHome::navigateToClubs, this, &MainWindow::showAdminClubPage);
        connect(adminHome, &AdminHome::navigateToNotifications, this, &MainWindow::showAdminNotiPage);
    }
}

void MainWindow::initializeAdminMemberPage()
{
    if (!adminMember) {
        adminMember = new AdminMember();
        // Connect AdminMember signals
        connect(adminMember, &AdminMember::navigateToHome, this, &MainWindow::showAdminHomePage);
        connect(adminMember, &AdminMember::navigateToClubs, this, &MainWindow::showAdminClubPage);
        connect(adminMember, &AdminMember::navigateToNotifications, this, &MainWindow::showAdminNotiPage);
    }
}

void MainWindow::initializeAdminClubPage()
{
    if (!adminClub) {
        adminClub = new AdminClub();
        // Connect AdminClub signals
        connect(adminClub, &AdminClub::navigateToHome, this, &MainWindow::showAdminHomePage);
        connect(adminClub, &AdminClub::navigateToMembers, this, &MainWindow::showAdminMemberPage);
        connect(adminClub, &AdminClub::navigateToNotifications, this, &MainWindow::showAdminNotiPage);
    }
}

void MainWindow::initializeAdminNotiPage()
{
    if (!adminNoti) {
        adminNoti = new AdminNoti();
        // Connect AdminNoti signals
        connect(adminNoti, &AdminNoti::navigateToHome, this, &MainWindow::showAdminHomePage);
        connect(adminNoti, &AdminNoti::navigateToMembers, this, &MainWindow::showAdminMemberPage);
        connect(adminNoti, &AdminNoti::navigateToClubs, this, &MainWindow::showAdminClubPage);
    }
}

// Initialize leader pages
void MainWindow::initializeLeaderHomePage()
{
    if (!leaderHomePage) {
        qDebug() << "Initializing Leader Home Page";
        leaderHomePage = new LHomepage();
        // Connect signals
        connect(leaderHomePage, &LHomepage::showGroupChat, this, &MainWindow::showLeaderGroupChat);
        connect(leaderHomePage, &LHomepage::showNotifications, this, &MainWindow::showLeaderNotifications);
        connect(leaderHomePage, &LHomepage::showLeaderboard, this, &MainWindow::showLeaderboard);
    }
}

void MainWindow::initializeLeaderGroupChat()
{
    if (!leaderGroupChat) {
        qDebug() << "Initializing Leader Group Chat";
        leaderGroupChat = new LGroupChat();
        // Connect signals
        connect(leaderGroupChat, &LGroupChat::navigateToHomePage, this, &MainWindow::showLeaderHomePage);
    }
}

void MainWindow::initializeLeaderNotifications()
{
    if (!leaderNotifications) {
        qDebug() << "Initializing Leader Notifications Page";
        leaderNotifications = new LNoti();
        // Connect signals
        connect(leaderNotifications, &LNoti::navigateBack, this, &MainWindow::showLeaderHomePage);
    }
}

void MainWindow::initializeLeaderboardPage()
{
    if (!leaderboardPage) {
        qDebug() << "Initializing Leaderboard Page";
        leaderboardPage = new LeaderboardAll();
        // Connect signals
        connect(leaderboardPage, &LeaderboardAll::navigateBack, this, &MainWindow::showLeaderHomePage);
    }
}

// Navigation methods for admin pages
void MainWindow::showHomePage()
{
    qDebug() << "Showing Home Page";
    initializeHomePage();
    homePage->show();
    // Clean up unused pages
    cleanupUnusedPages(homePage);
}

void MainWindow::showRegisterPage()
{
    qDebug() << "Showing Register Page";
    initializeRegisterPage();
    registerPage->show();
    // Clean up unused pages
    cleanupUnusedPages(registerPage);
}

void MainWindow::showLoginPage()
{
    qDebug() << "Showing Login Page";
    initializeLoginPage();
    loginPage->show();
    // Clean up unused pages
    cleanupUnusedPages(loginPage);
}

void MainWindow::showAdminHomePage()
{
    qDebug() << "Showing Admin Home Page";
    initializeAdminHomePage();
    adminHome->show();
    adminHome->raise();
    adminHome->activateWindow();
    // Clean up unused pages
    cleanupUnusedPages(adminHome);
}

void MainWindow::showAdminMemberPage()
{
    qDebug() << "Showing Admin Member Page";
    initializeAdminMemberPage();
    adminMember->show();
    adminMember->raise();
    adminMember->activateWindow();
    // Clean up unused pages
    cleanupUnusedPages(adminMember);
}

void MainWindow::showAdminClubPage()
{
    qDebug() << "Showing Admin Club Page";
    initializeAdminClubPage();
    adminClub->show();
    adminClub->raise();
    adminClub->activateWindow();
    // Clean up unused pages
    cleanupUnusedPages(adminClub);
}

void MainWindow::showAdminNotiPage()
{
    qDebug() << "Showing Admin Notification Page";
    initializeAdminNotiPage();
    adminNoti->show();
    adminNoti->raise();
    adminNoti->activateWindow();
    // Clean up unused pages
    cleanupUnusedPages(adminNoti);
}

// Navigation methods for leader pages
void MainWindow::showLeaderHomePage()
{
    qDebug() << "Showing Leader Home Page";
    initializeLeaderHomePage();
    leaderHomePage->show();
    leaderHomePage->raise();
    leaderHomePage->activateWindow();
    // Clean up unused pages
    cleanupUnusedPages(leaderHomePage);
}

void MainWindow::showLeaderGroupChat()
{
    qDebug() << "Showing Leader Group Chat";
    initializeLeaderGroupChat();
    leaderGroupChat->show();
    leaderGroupChat->raise();
    leaderGroupChat->activateWindow();
    // Clean up unused pages
    cleanupUnusedPages(leaderGroupChat);
}

void MainWindow::showLeaderNotifications()
{
    qDebug() << "Showing Leader Notifications";
    initializeLeaderNotifications();
    leaderNotifications->show();
    leaderNotifications->raise();
    leaderNotifications->activateWindow();
    // Clean up unused pages
    cleanupUnusedPages(leaderNotifications);
}

void MainWindow::showLeaderboard()
{
    qDebug() << "Showing Leaderboard";
    initializeLeaderboardPage();
    leaderboardPage->show();
    leaderboardPage->raise();
    leaderboardPage->activateWindow();
    // Clean up unused pages
    cleanupUnusedPages(leaderboardPage);
}

// Authentication handlers
void MainWindow::handleRegisterSuccess(int userId)
{
    qDebug() << "Registration successful for user ID:" << userId;
    currentUserId = userId;

    if (isUserLeader(userId)) {
        qDebug() << "User is a leader, navigating to leader home page";
        showLeaderHomePage();
    } else {
        qDebug() << "User is not a leader, navigating to admin home page";
        showAdminHomePage();
    }
}

void MainWindow::handleLoginSuccess(int userId)
{
    qDebug() << "Login successful for user ID:" << userId;
    currentUserId = userId;

    if (isUserLeader(userId)) {
        qDebug() << "User is a leader, navigating to leader home page";
        showLeaderHomePage();
    } else {
        qDebug() << "User is not a leader, navigating to admin home page";
        showAdminHomePage();
    }
}

// Check if user is a leader
bool MainWindow::isUserLeader(int userId)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM clubleaders_list WHERE leader_id = :id");
    query.bindValue(":id", userId);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    qDebug() << "Error checking leader status:" << query.lastError().text();
    return false;
}

// Function to destroy unused pages to free memory
void MainWindow::cleanupUnusedPages(QWidget* currentPage)
{
    // Don't destroy the current page
    if (homePage && homePage != currentPage) {
        delete homePage;
        homePage = nullptr;
        qDebug() << "Home page destroyed";
    }

    if (registerPage && registerPage != currentPage) {
        delete registerPage;
        registerPage = nullptr;
        qDebug() << "Register page destroyed";
    }

    if (loginPage && loginPage != currentPage) {
        delete loginPage;
        loginPage = nullptr;
        qDebug() << "Login page destroyed";
    }

    if (adminHome && adminHome != currentPage) {
        delete adminHome;
        adminHome = nullptr;
        qDebug() << "Admin home page destroyed";
    }

    if (adminMember && adminMember != currentPage) {
        delete adminMember;
        adminMember = nullptr;
        qDebug() << "Admin member page destroyed";
    }

    if (adminClub && adminClub != currentPage) {
        delete adminClub;
        adminClub = nullptr;
        qDebug() << "Admin club page destroyed";
    }

    if (adminNoti && adminNoti != currentPage) {
        delete adminNoti;
        adminNoti = nullptr;
        qDebug() << "Admin notification page destroyed";
    }

    if (leaderHomePage && leaderHomePage != currentPage) {
        delete leaderHomePage;
        leaderHomePage = nullptr;
        qDebug() << "Leader home page destroyed";
    }

    if (leaderGroupChat && leaderGroupChat != currentPage) {
        delete leaderGroupChat;
        leaderGroupChat = nullptr;
        qDebug() << "Leader group chat destroyed";
    }

    if (leaderNotifications && leaderNotifications != currentPage) {
        delete leaderNotifications;
        leaderNotifications = nullptr;
        qDebug() << "Leader notifications destroyed";
    }

    if (leaderboardPage && leaderboardPage != currentPage) {
        delete leaderboardPage;
        leaderboardPage = nullptr;
        qDebug() << "Leaderboard page destroyed";
    }
}
