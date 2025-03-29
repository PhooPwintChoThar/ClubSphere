#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    homePage(nullptr), registerPage(nullptr), loginPage(nullptr),
    adminHome(nullptr), adminMember(nullptr), adminClub(nullptr), adminNoti(nullptr)
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
}

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
        connect(registerPage, &RegisterPage::openadminHome, this, &MainWindow::showAdminHomePage);
    }
}

void MainWindow::initializeLoginPage()
{
    if (!loginPage) {
        loginPage = new LoginPage();
        // Connect LoginPage signals
        connect(loginPage, &LoginPage::navigateToHome, this, &MainWindow::showHomePage);
        connect(loginPage, &LoginPage::openadminHome, this, &MainWindow::showAdminHomePage);
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
}

// Navigation methods
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
