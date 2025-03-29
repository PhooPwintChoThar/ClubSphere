#include "adminHome.h"
#include <QPixmap>
#include <QIcon>
#include <QPainter>
#include <QDateTime>
#include <QWidget>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include<database.h>

AdminHome::AdminHome(QWidget *parent) : QWidget(parent)
{
    Database::initialize();
    setupUI();
    setupStatsSection();
    setupTopClubsSection();
    setupTopUsersSection();
    setupNavigation();
}

AdminHome::~AdminHome()
{
}

void AdminHome::setupUI()
{
    // Set background color to white and fix window size
    this->setStyleSheet("background-color: white;");
    this->setFixedSize(350, 650);

    // Main layout setup
    mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setContentsMargins(20, 30, 20, 20);
    mainLayout->setSpacing(15);

    // Title
    titleLabel = new QLabel("Welcome to\nClubsphere!", this);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignLeft);

    mainLayout->addWidget(titleLabel);
}



int AdminHome::getTotalUsersCount()
{   Database::initialize();
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users_list");

    if (query.exec() && query.next()) {
        int totalUsers = query.value(0).toInt();
        return totalUsers;
    } else {
        qDebug() << "Error fetching total users count:" << query.lastError().text();
        return 0;
    }
}

int AdminHome::getTotalClubsCount()
{   Database::initialize();
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM clubs_list");

    if (query.exec() && query.next()) {
        int totalClubs = query.value(0).toInt();
        return totalClubs;
    } else {
        qDebug() << "Error fetching total clubs count:" << query.lastError().text();
        return 0;
    }
}

void AdminHome::setupStatsSection()
{
    // Stats section with two cards
    QHBoxLayout *statsLayout = new QHBoxLayout();
    statsLayout->setContentsMargins(0, 0, 0, 0);
    statsLayout->setSpacing(12);

    // Define fonts for stats
    QFont statsLabelFont;
    statsLabelFont.setBold(true);
    statsLabelFont.setPointSize(9);

    QFont countFont;
    countFont.setBold(true);
    countFont.setPointSize(18);

    // Total Clubs card
    clubsFrame = createRoundedFrame();
    clubsFrame->setMinimumHeight(80);

    QVBoxLayout *clubsLayout = new QVBoxLayout(clubsFrame);
    clubsLayout->setContentsMargins(10, 10, 10, 10);

    totalClubsLabel = new QLabel("Total clubs", this);
    totalClubsLabel->setFont(statsLabelFont);

    int totalClubs=getTotalClubsCount();
    clubsCountLabel = new QLabel(QString::number(totalClubs), this);  // This can also be queried from a clubs table if exists
    clubsCountLabel->setFont(countFont);

    clubsLayout->addWidget(totalClubsLabel);
    clubsLayout->addWidget(clubsCountLabel);
    statsLayout->addWidget(clubsFrame);

    // Total Users card
    usersFrame = createRoundedFrame();
    usersFrame->setMinimumHeight(80);

    QVBoxLayout *usersLayout = new QVBoxLayout(usersFrame);
    usersLayout->setContentsMargins(10, 10, 10, 10);

    totalUsersLabel = new QLabel("Total Users", this);
    totalUsersLabel->setFont(statsLabelFont);

    // Get total users count from database
    int totalUsers = getTotalUsersCount();
    usersCountLabel = new QLabel(QString::number(totalUsers), this);
    usersCountLabel->setFont(countFont);

    usersLayout->addWidget(totalUsersLabel);
    usersLayout->addWidget(usersCountLabel);
    statsLayout->addWidget(usersFrame);

    mainLayout->addLayout(statsLayout);
}

void AdminHome::setupTopClubsSection()
{
    // Top 3 Clubs section
    topClubsFrame = createRoundedFrame();
    topClubsFrame->setMinimumWidth(310);
    QVBoxLayout *topClubsLayout = new QVBoxLayout(topClubsFrame);
    topClubsLayout->setContentsMargins(15, 15, 15, 15);

    thisWeekClubsLabel = new QLabel("This Week", this);
    thisWeekClubsLabel->setFont(QFont("Arial", 10));

    topClubsLabel = new QLabel("TOP 3 Clubs", this);
    QFont topLabelFont;
    topLabelFont.setBold(true);
    topLabelFont.setPointSize(14);
    topClubsLabel->setFont(topLabelFont);

    // Define club font
    QFont clubFont;
    clubFont.setBold(true);
    clubFont.setPointSize(10);

    // Create club entries with medals
    QHBoxLayout *musicClubLayout = new QHBoxLayout();
    goldMedal1 = createMedalLabel("gold", true);
    musicClubLabel = new QLabel("Music Club", this);
    musicClubLabel->setFont(clubFont);
    musicClubLayout->addWidget(goldMedal1);
    musicClubLayout->addWidget(musicClubLabel);
    musicClubLayout->addStretch();

    QHBoxLayout *basketballClubLayout = new QHBoxLayout();
    silverMedal1 = createMedalLabel("silver", true);
    basketballClubLabel = new QLabel("BasketBall Club", this);
    basketballClubLabel->setFont(clubFont);
    basketballClubLayout->addWidget(silverMedal1);
    basketballClubLayout->addWidget(basketballClubLabel);
    basketballClubLayout->addStretch();

    QHBoxLayout *badmintonClubLayout = new QHBoxLayout();
    bronzeMedal1 = createMedalLabel("bronze", true);
    badmintonClubLabel = new QLabel("Badminton Club", this);
    badmintonClubLabel->setFont(clubFont);
    badmintonClubLayout->addWidget(bronzeMedal1);
    badmintonClubLayout->addWidget(badmintonClubLabel);
    badmintonClubLayout->addStretch();

    topClubsLayout->addWidget(thisWeekClubsLabel);
    topClubsLayout->addWidget(topClubsLabel);
    topClubsLayout->addLayout(musicClubLayout);
    topClubsLayout->addLayout(basketballClubLayout);
    topClubsLayout->addLayout(badmintonClubLayout);

    mainLayout->addWidget(topClubsFrame, 0, Qt::AlignHCenter);
}

void AdminHome::setupTopUsersSection()
{
    // TOP 3 Users section
    topUsersFrame = createRoundedFrame();
    topUsersFrame->setMinimumWidth(310);
    QVBoxLayout *topUsersLayout = new QVBoxLayout(topUsersFrame);
    topUsersLayout->setContentsMargins(15, 15, 15, 15);

    thisWeekUsersLabel = new QLabel("This Week", this);
    thisWeekUsersLabel->setFont(QFont("Arial", 10));

    topUsersLabel = new QLabel("TOP 3 Users", this);
    QFont topLabelFont;
    topLabelFont.setBold(true);
    topLabelFont.setPointSize(14);
    topUsersLabel->setFont(topLabelFont);

    // Define user font
    QFont userFont;
    userFont.setBold(true);
    userFont.setPointSize(10);

    // Create user entries with medals
    QHBoxLayout *maryLayout = new QHBoxLayout();
    goldMedal2 = createMedalLabel("gold", true);
    maryLabel = new QLabel("Mary", this);
    maryLabel->setFont(userFont);
    maryLayout->addWidget(goldMedal2);
    maryLayout->addWidget(maryLabel);
    maryLayout->addStretch();

    QHBoxLayout *michelLayout = new QHBoxLayout();
    silverMedal2 = createMedalLabel("silver", true);
    michelLabel = new QLabel("Michel", this);
    michelLabel->setFont(userFont);
    michelLayout->addWidget(silverMedal2);
    michelLayout->addWidget(michelLabel);
    michelLayout->addStretch();

    QHBoxLayout *christinaLayout = new QHBoxLayout();
    bronzeMedal2 = createMedalLabel("bronze", true);
    christinaLabel = new QLabel("Christina", this);
    christinaLabel->setFont(userFont);
    christinaLayout->addWidget(bronzeMedal2);
    christinaLayout->addWidget(christinaLabel);
    christinaLayout->addStretch();

    topUsersLayout->addWidget(thisWeekUsersLabel);
    topUsersLayout->addWidget(topUsersLabel);
    topUsersLayout->addLayout(maryLayout);
    topUsersLayout->addLayout(michelLayout);
    topUsersLayout->addLayout(christinaLayout);

    mainLayout->addWidget(topUsersFrame, 0, Qt::AlignHCenter);

    // Add spacer to push navigation bar to bottom
    mainLayout->addStretch();
}

void AdminHome::setupNavigation()
{
    // Bottom navigation bar
    navigationFrame = new QFrame(this);
    navigationFrame->setFrameShape(QFrame::NoFrame);
    navigationFrame->setLineWidth(0);
    navigationFrame->setStyleSheet("QFrame { background-color: white; }");

    QHBoxLayout *navLayout = new QHBoxLayout(navigationFrame);
    navLayout->setContentsMargins(15, 10, 15, 10);
    navLayout->setSpacing(45);  // Increased spacing to fit the wider window

    // Create navigation icons
    homeButton = new QPushButton("", this);
    homeButton->setIcon(QIcon(":/images/resources/home_logo.png"));
    homeButton->setIconSize(QSize(20, 20));
    // Highlight the home button since we're on the home page
    homeButton->setStyleSheet("QPushButton { border: none; background-color: #D9E9D8; border-radius: 10px; padding: 5px; }");

    // Initialize the profileButton
    profileButton = new QPushButton("", this);
    profileButton->setObjectName("profileButton");
    profileButton->setIcon(QIcon(":/images/resources/member_logo.png"));
    profileButton->setIconSize(QSize(20, 20));
    profileButton->setStyleSheet("QPushButton { border: none; }");

    groupsButton = new QPushButton("", this);
    groupsButton->setIcon(QIcon(":/images/resources/club_logo.png"));
    groupsButton->setIconSize(QSize(30, 30));
    groupsButton->setStyleSheet("QPushButton { border: none; }");

    notificationsButton = new QPushButton("", this);
    notificationsButton->setIcon(QIcon(":/images/resources/noti_logo.png"));
    notificationsButton->setIconSize(QSize(20, 20));
    notificationsButton->setStyleSheet("QPushButton { border: none; }");

    navLayout->addWidget(homeButton);
    navLayout->addWidget(profileButton);
    navLayout->addWidget(groupsButton);
    navLayout->addWidget(notificationsButton);

    mainLayout->addWidget(navigationFrame);

    // Set styles for frames - Light green background
    QString frameStyle = "QFrame { background-color: #D9E9D8; border-radius: 12px; }";
    clubsFrame->setStyleSheet(frameStyle);
    usersFrame->setStyleSheet(frameStyle);
    topClubsFrame->setStyleSheet(frameStyle);
    topUsersFrame->setStyleSheet(frameStyle);

    // Connect navigation buttons
    connect(homeButton, &QPushButton::clicked, this, &AdminHome::onHomeButtonClicked);
    connect(profileButton, &QPushButton::clicked, this, &AdminHome::onProfileButtonClicked);
    connect(groupsButton, &QPushButton::clicked, this, &AdminHome::onGroupsButtonClicked);
    connect(notificationsButton, &QPushButton::clicked, this, &AdminHome::onNotificationsButtonClicked);
}

void AdminHome::onHomeButtonClicked()
{
    qDebug() << "Home button clicked in AdminHome - already on this page";
    // No navigation needed as we're already on this page
}

void AdminHome::onProfileButtonClicked()
{
    qDebug() << "Profile button clicked in AdminHome";
    emit navigateToMembers();
}

void AdminHome::onGroupsButtonClicked()
{
    qDebug() << "Groups button clicked in AdminHome";
    emit navigateToClubs();
}

void AdminHome::onNotificationsButtonClicked()
{
    qDebug() << "Notifications button clicked in AdminHome";
    emit navigateToNotifications();
}

QLabel* AdminHome::createMedalLabel(const QString &color, bool withRibbon)
{
    QLabel* medalLabel = new QLabel(this);

    // Use image files instead of drawing medals
    QString imagePath;
    if (color == "gold") {
        imagePath = ":/images/resources/gold_medal.png";
    } else if (color == "silver") {
        imagePath = ":/images/resources/silver_medal.png";
    } else if (color == "bronze") {
        imagePath = ":/images/resources/bronze_medal.png";
    }

    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        pixmap = pixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        medalLabel->setPixmap(pixmap);
    } else {
        // Fallback if image isn't found
        medalLabel->setText(color);
    }

    medalLabel->setFixedSize(24, 24);

    return medalLabel;
}

QFrame* AdminHome::createRoundedFrame()
{
    QFrame* frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setLineWidth(0);
    frame->setMinimumWidth(130);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    return frame;
}
