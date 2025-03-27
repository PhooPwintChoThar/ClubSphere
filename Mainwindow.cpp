#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Create all pages
    homePage = new HomePage();
    registerPage = new RegisterPage();
    loginPage = new LoginPage();
    adminHome = new AdminHome();
    adminMember = new AdminMember();
    adminClub = new AdminClub();
    adminNoti = new AdminNoti();

    qDebug() << "All windows initialized";

    // Set up all connections
    setupConnections();

    // Set initial window properties and show the home page
    this->setWindowTitle("Club Management System");
    this->resize(375, 800);

    // Show the initial page
    showHomePage();
}

MainWindow::~MainWindow()
{
    // Clean up all pages
    delete homePage;
    delete registerPage;
    delete loginPage;
    delete adminHome;
    delete adminMember;
    delete adminClub;
    delete adminNoti;
}

void MainWindow::setupConnections()
{
    // HomePage connections
    connect(homePage, &HomePage::openRegisterPage, this, &MainWindow::showRegisterPage);
    connect(homePage, &HomePage::openLoginPage, this, &MainWindow::showLoginPage);

    // RegisterPage connections
    connect(registerPage, &RegisterPage::navigateToHome, this, &MainWindow::showHomePage);
    connect(registerPage, &RegisterPage::openadminHome, this, &MainWindow::showAdminHomePage);

    // LoginPage connections
    connect(loginPage, &LoginPage::navigateToHome, this, &MainWindow::showHomePage);
    connect(loginPage, &LoginPage::openadminHome, this, &MainWindow::showAdminHomePage);


    // AdminHome connections
    connect(adminHome, &AdminHome::navigateToMembers, this, &MainWindow::showAdminMemberPage);
    connect(adminHome, &AdminHome::navigateToClubs, this, &MainWindow::showAdminClubPage);
    connect(adminHome, &AdminHome::navigateToNotifications, this, &MainWindow::showAdminNotiPage);

    // AdminMember connections
    connect(adminMember, &AdminMember::navigateToHome, this, &MainWindow::showAdminHomePage);
    connect(adminMember, &AdminMember::navigateToClubs, this, &MainWindow::showAdminClubPage);
    connect(adminMember, &AdminMember::navigateToNotifications, this, &MainWindow::showAdminNotiPage);

    // AdminClub connections
    connect(adminClub, &AdminClub::navigateToHome, this, &MainWindow::showAdminHomePage);
    connect(adminClub, &AdminClub::navigateToMembers, this, &MainWindow::showAdminMemberPage);
    connect(adminClub, &AdminClub::navigateToNotifications, this, &MainWindow::showAdminNotiPage);

    // AdminNoti connections
    connect(adminNoti, &AdminNoti::navigateToHome, this, &MainWindow::showAdminHomePage);
    connect(adminNoti, &AdminNoti::navigateToMembers, this, &MainWindow::showAdminMemberPage);
    connect(adminNoti, &AdminNoti::navigateToClubs, this, &MainWindow::showAdminClubPage);
}

// Navigation slot methods
void MainWindow::showHomePage()
{
    qDebug() << "Showing Home Page";
    homePage->show();
    registerPage->hide();
    loginPage->hide();
    adminHome->hide();
    adminMember->hide();
    adminClub->hide();
    adminNoti->hide();
}

void MainWindow::showRegisterPage()
{
    qDebug() << "Showing Register Page";
    homePage->hide();
    registerPage->show();
    loginPage->hide();
    adminHome->hide();
    adminMember->hide();
    adminClub->hide();
    adminNoti->hide();
}

void MainWindow::showLoginPage()
{
    qDebug() << "Showing Login Page";
    homePage->hide();
    registerPage->hide();
    loginPage->show();
    adminHome->hide();
    adminMember->hide();
    adminClub->hide();
    adminNoti->hide();
}

void MainWindow::showAdminHomePage()
{
    qDebug() << "Showing Admin Home Page";
    homePage->hide();
    registerPage->hide();
    loginPage->hide();
    adminHome->show();
    adminHome->raise();
    adminHome->activateWindow();
    adminMember->hide();
    adminClub->hide();
    adminNoti->hide();
}

void MainWindow::showAdminMemberPage()
{
    qDebug() << "Showing Admin Member Page";
    homePage->hide();
    registerPage->hide();
    loginPage->hide();
    adminHome->hide();
    adminMember->show();
    adminMember->raise();
    adminMember->activateWindow();
    adminClub->hide();
    adminNoti->hide();
}

void MainWindow::showAdminClubPage()
{
    qDebug() << "Showing Admin Club Page";
    homePage->hide();
    registerPage->hide();
    loginPage->hide();
    adminHome->hide();
    adminMember->hide();
    adminClub->show();
    adminClub->raise();
    adminClub->activateWindow();
    adminNoti->hide();
}

void MainWindow::showAdminNotiPage()
{
    qDebug() << "Showing Admin Notification Page";
    homePage->hide();
    registerPage->hide();
    loginPage->hide();
    adminHome->hide();
    adminMember->hide();
    adminClub->hide();
    adminNoti->show();
    adminNoti->raise();
    adminNoti->activateWindow();
}
