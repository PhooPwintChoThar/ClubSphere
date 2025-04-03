// mprofilepage.cpp
#include "mprofilepage.h"
#include <QIcon>
#include <QFont>
#include <QGridLayout>
#include <QFrame>
#include <QScrollArea>
#include <QPainter>
#include <QBitmap>
#include <QPixmap>

MProfilePage::MProfilePage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

MProfilePage::~MProfilePage()
{
}

void MProfilePage::setupUI()
{
    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Profile area with light green background
    QWidget* profileBgWidget = new QWidget();
    profileBgWidget->setFixedHeight(200);
    profileBgWidget->setStyleSheet("background-color: #c9e4c5;");

    // Profile picture area with image from resources
    QLabel* profilePicWidget = new QLabel();
    profilePicWidget->setFixedSize(120, 120);
    QPixmap profilePixmap(":/images/resources/user.png"); // Replace with your actual resource path
    if (!profilePixmap.isNull()) {
        // Scale the pixmap to fit the label
        profilePixmap = profilePixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Create a circular mask
        QBitmap mask(120, 120);
        mask.fill(Qt::white);
        QPainter painter(&mask);
        painter.setBrush(Qt::black);
        painter.drawEllipse(0, 0, 120, 120);

        // Apply the mask to make the image circular
        profilePixmap.setMask(mask);
        profilePicWidget->setPixmap(profilePixmap);
    } else {
        // Fallback if image isn't found - keep the gray circle
        profilePicWidget->setStyleSheet(
            "background-color: #dddddd; border-radius: 60px;"
            );

        // Inner white circle fallback
        QWidget* innerCircle = new QWidget(profilePicWidget);
        innerCircle->setFixedSize(70, 70);
        innerCircle->move(25, 25);
        innerCircle->setStyleSheet(
            "background-color: white; border-radius: 35px;"
            );
    }

    // Edit icon button - now using PNG image
    QPushButton* editProfileBtn = new QPushButton();
    editProfileBtn->setFixedSize(30, 30);
    QIcon editIcon(":/images/resources/edit.png");
    if (!editIcon.isNull()) {
        editProfileBtn->setIcon(editIcon);
        editProfileBtn->setIconSize(QSize(24, 24));
        editProfileBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: rgba(128, 128, 128, 0.5);"
            "  border-radius: 15px;"
            "  border: none;"
            "}"
            );
    } else {
        // Fallback if image isn't found
        editProfileBtn->setText("✎");
        editProfileBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: rgba(128, 128, 128, 0.5);"
            "  border-radius: 15px;"
            "  border: none;"
            "  color: white;"
            "  font-weight: bold;"
            "}"
            );
    }

    // Position profile pic and edit button
    QVBoxLayout* profileBgLayout = new QVBoxLayout(profileBgWidget);
    profileBgLayout->setAlignment(Qt::AlignCenter);
    profileBgLayout->addWidget(profilePicWidget, 0, Qt::AlignCenter);

    // Position edit button at the bottom right of profile pic
    editProfileBtn->setParent(profileBgWidget);
    editProfileBtn->move(profileBgWidget->width()/2 + 30, profileBgWidget->height()/2 + 10);

    // User information section
    QWidget* userInfoWidget = new QWidget();
    userInfoWidget->setStyleSheet("background-color: white;");

    QVBoxLayout* userInfoLayout = new QVBoxLayout(userInfoWidget);
    userInfoLayout->setContentsMargins(20, 20, 20, 20);
    userInfoLayout->setSpacing(10);

    // Username row with edit icon
    QWidget* usernameWidget = new QWidget();
    QHBoxLayout* usernameLayout = new QHBoxLayout(usernameWidget);
    usernameLayout->setContentsMargins(0, 5, 0, 5);

    QLabel* usernameLabel = new QLabel("Username");
    QFont regularFont;
    regularFont.setPointSize(11);
    usernameLabel->setFont(regularFont);
    usernameLabel->setStyleSheet("color: #666666;");

    QPushButton* editUsernameBtn = new QPushButton();
    QIcon editUsernameIcon(":/images/resources/edit.png");
    if (!editUsernameIcon.isNull()) {
        editUsernameBtn->setIcon(editUsernameIcon);
        editUsernameBtn->setIconSize(QSize(16, 16));
        editUsernameBtn->setFixedSize(20, 20);
        editUsernameBtn->setStyleSheet("border: none; background: transparent;");
    } else {
        // Fallback if image isn't found
        editUsernameBtn->setText("✎");
        editUsernameBtn->setFixedSize(20, 20);
        editUsernameBtn->setStyleSheet("color: #888888; background: transparent; border: none;");
    }

    QLabel* usernameValue = new QLabel("Johnathan");
    QFont valueFont;
    valueFont.setPointSize(11);
    usernameValue->setFont(valueFont);
    usernameValue->setAlignment(Qt::AlignRight);

    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addStretch(1);
    usernameLayout->addWidget(editUsernameBtn);
    usernameLayout->addWidget(usernameValue);

    // Separator line
    QFrame* line1 = new QFrame();
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    line1->setStyleSheet("background-color: #e0e0e0;");
    line1->setFixedHeight(1);

    // ID row
    QWidget* idWidget = new QWidget();
    QHBoxLayout* idLayout = new QHBoxLayout(idWidget);
    idLayout->setContentsMargins(0, 5, 0, 5);

    QLabel* idLabel = new QLabel("ID");
    idLabel->setFont(regularFont);
    idLabel->setStyleSheet("color: #666666;");

    QLabel* idValue = new QLabel("67011731");
    idValue->setFont(valueFont);
    idValue->setAlignment(Qt::AlignRight);

    idLayout->addWidget(idLabel);
    idLayout->addStretch(1);
    idLayout->addWidget(idValue);

    // Separator line
    QFrame* line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
    line2->setStyleSheet("background-color: #e0e0e0;");
    line2->setFixedHeight(1);

    // Points row
    QWidget* pointsWidget = new QWidget();
    QHBoxLayout* pointsLayout = new QHBoxLayout(pointsWidget);
    pointsLayout->setContentsMargins(0, 5, 0, 5);

    QLabel* pointsLabel = new QLabel("Points");
    pointsLabel->setFont(regularFont);
    pointsLabel->setStyleSheet("color: #666666;");

    QLabel* pointsValue = new QLabel("350");
    pointsValue->setFont(valueFont);
    pointsValue->setAlignment(Qt::AlignRight);

    pointsLayout->addWidget(pointsLabel);
    pointsLayout->addStretch(1);
    pointsLayout->addWidget(pointsValue);

    // Separator line
    QFrame* line3 = new QFrame();
    line3->setFrameShape(QFrame::HLine);
    line3->setFrameShadow(QFrame::Sunken);
    line3->setStyleSheet("background-color: #e0e0e0;");
    line3->setFixedHeight(1);

    // Total Clubs Joined row
    QWidget* clubsWidget = new QWidget();
    QHBoxLayout* clubsLayout = new QHBoxLayout(clubsWidget);
    clubsLayout->setContentsMargins(0, 5, 0, 5);

    QLabel* clubsLabel = new QLabel("Total Clubs Joined");
    clubsLabel->setFont(regularFont);
    clubsLabel->setStyleSheet("color: #666666;");

    QLabel* clubsValue = new QLabel("5");
    clubsValue->setFont(valueFont);
    clubsValue->setAlignment(Qt::AlignRight);

    clubsLayout->addWidget(clubsLabel);
    clubsLayout->addStretch(1);
    clubsLayout->addWidget(clubsValue);

    // Add rows to user info layout
    userInfoLayout->addWidget(usernameWidget);
    userInfoLayout->addWidget(line1);
    userInfoLayout->addWidget(idWidget);
    userInfoLayout->addWidget(line2);
    userInfoLayout->addWidget(pointsWidget);
    userInfoLayout->addWidget(line3);
    userInfoLayout->addWidget(clubsWidget);

    // Rankings section
    QLabel* rankingsLabel = new QLabel("Your Rankings:");
    rankingsLabel->setFont(regularFont);
    rankingsLabel->setContentsMargins(20, 15, 0, 10);

    // Create a scrollable rankings area
    QScrollArea* rankingsScrollArea = new QScrollArea();
    rankingsScrollArea->setWidgetResizable(true);
    rankingsScrollArea->setStyleSheet("background-color: #dddddd; border: none;");
    rankingsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rankingsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Create the widget that will be inside the scroll area
    QWidget* rankingsWidget = new QWidget();
    rankingsWidget->setStyleSheet("background-color: #dddddd;");

    QVBoxLayout* rankingsLayout = new QVBoxLayout(rankingsWidget);
    rankingsLayout->setContentsMargins(20, 10, 20, 10);
    rankingsLayout->setSpacing(0);

    // Music Club ranking
    QWidget* musicRankWidget = new QWidget();
    musicRankWidget->setFixedHeight(50);
    QHBoxLayout* musicRankLayout = new QHBoxLayout(musicRankWidget);
    musicRankLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* musicClubLabel = new QLabel("Music Club");
    musicClubLabel->setFont(regularFont);

    QLabel* musicRankValue = new QLabel("Top 1");
    musicRankValue->setFont(valueFont);
    musicRankValue->setAlignment(Qt::AlignRight);

    musicRankLayout->addWidget(musicClubLabel);
    musicRankLayout->addStretch(1);
    musicRankLayout->addWidget(musicRankValue);

    // Separator line
    QFrame* rankLine1 = new QFrame();
    rankLine1->setFrameShape(QFrame::HLine);
    rankLine1->setFrameShadow(QFrame::Sunken);
    rankLine1->setStyleSheet("background-color: #cccccc;");
    rankLine1->setFixedHeight(1);

    // Science Club ranking
    QWidget* scienceRankWidget = new QWidget();
    scienceRankWidget->setFixedHeight(50);
    QHBoxLayout* scienceRankLayout = new QHBoxLayout(scienceRankWidget);
    scienceRankLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* scienceClubLabel = new QLabel("Science Club");
    scienceClubLabel->setFont(regularFont);

    QLabel* scienceRankValue = new QLabel("Top 10");
    scienceRankValue->setFont(valueFont);
    scienceRankValue->setAlignment(Qt::AlignRight);

    scienceRankLayout->addWidget(scienceClubLabel);
    scienceRankLayout->addStretch(1);
    scienceRankLayout->addWidget(scienceRankValue);

    // Separator line
    QFrame* rankLine2 = new QFrame();
    rankLine2->setFrameShape(QFrame::HLine);
    rankLine2->setFrameShadow(QFrame::Sunken);
    rankLine2->setStyleSheet("background-color: #cccccc;");
    rankLine2->setFixedHeight(1);

    // Table Tennis Club ranking
    QWidget* tennisRankWidget = new QWidget();
    tennisRankWidget->setFixedHeight(50);
    QHBoxLayout* tennisRankLayout = new QHBoxLayout(tennisRankWidget);
    tennisRankLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* tennisClubLabel = new QLabel("Table Tennis Club");
    tennisClubLabel->setFont(regularFont);

    QLabel* tennisRankValue = new QLabel("Top 12");
    tennisRankValue->setFont(valueFont);
    tennisRankValue->setAlignment(Qt::AlignRight);

    tennisRankLayout->addWidget(tennisClubLabel);
    tennisRankLayout->addStretch(1);
    tennisRankLayout->addWidget(tennisRankValue);

    // Add a few more items to demonstrate scrolling
    // Chess Club ranking
    QWidget* chessRankWidget = new QWidget();
    chessRankWidget->setFixedHeight(50);
    QHBoxLayout* chessRankLayout = new QHBoxLayout(chessRankWidget);
    chessRankLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* chessClubLabel = new QLabel("Chess Club");
    chessClubLabel->setFont(regularFont);

    QLabel* chessRankValue = new QLabel("Top 15");
    chessRankValue->setFont(valueFont);
    chessRankValue->setAlignment(Qt::AlignRight);

    chessRankLayout->addWidget(chessClubLabel);
    chessRankLayout->addStretch(1);
    chessRankLayout->addWidget(chessRankValue);

    // Separator line
    QFrame* rankLine3 = new QFrame();
    rankLine3->setFrameShape(QFrame::HLine);
    rankLine3->setFrameShadow(QFrame::Sunken);
    rankLine3->setStyleSheet("background-color: #cccccc;");
    rankLine3->setFixedHeight(1);

    // Art Club ranking
    QWidget* artRankWidget = new QWidget();
    artRankWidget->setFixedHeight(50);
    QHBoxLayout* artRankLayout = new QHBoxLayout(artRankWidget);
    artRankLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* artClubLabel = new QLabel("Art Club");
    artClubLabel->setFont(regularFont);

    QLabel* artRankValue = new QLabel("Top 20");
    artRankValue->setFont(valueFont);
    artRankValue->setAlignment(Qt::AlignRight);

    artRankLayout->addWidget(artClubLabel);
    artRankLayout->addStretch(1);
    artRankLayout->addWidget(artRankValue);

    // Add rankings to layout
    rankingsLayout->addWidget(musicRankWidget);
    rankingsLayout->addWidget(rankLine1);
    rankingsLayout->addWidget(scienceRankWidget);
    rankingsLayout->addWidget(rankLine2);
    rankingsLayout->addWidget(tennisRankWidget);
    rankingsLayout->addWidget(rankLine3);
    rankingsLayout->addWidget(chessRankWidget);

    // Fix the clone() error by creating a new QFrame
    QFrame* rankLine4 = new QFrame();
    rankLine4->setFrameShape(QFrame::HLine);
    rankLine4->setFrameShadow(QFrame::Sunken);
    rankLine4->setStyleSheet("background-color: #cccccc;");
    rankLine4->setFixedHeight(1);
    rankingsLayout->addWidget(rankLine4);

    rankingsLayout->addWidget(artRankWidget);

    // Add stretch to keep widgets at the top
    rankingsLayout->addStretch(1);

    // Set the widget to the scroll area
    rankingsScrollArea->setWidget(rankingsWidget);

    // Bottom navigation bar - keeping as in the original code
    m_bottomNavBar = new QWidget();
    m_bottomNavBar->setFixedHeight(60);
    m_bottomNavBar->setStyleSheet("background-color: white; border-top: 1px solid #e0e0e0;");

    m_bottomNavLayout = new QHBoxLayout(m_bottomNavBar);
    m_bottomNavLayout->setContentsMargins(10, 5, 10, 5);
    m_bottomNavLayout->setSpacing(0);

    // Create navigation buttons
    // Home icon
    QPushButton* homeIcon = new QPushButton();
    QIcon homeIconImage(":/images/resources/home_logo.png");
    if (!homeIconImage.isNull()) {
        homeIcon->setIcon(homeIconImage);
        homeIcon->setIconSize(QSize(24, 24));
    } else {
        homeIcon->setText("Home");
    }
    homeIcon->setFixedSize(40, 40);
    homeIcon->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Club icon
    QPushButton* clubIcon = new QPushButton();
    QIcon clubIconImage(":/images/resources/club_logo.png");
    if (!clubIconImage.isNull()) {
        clubIcon->setIcon(clubIconImage);
        clubIcon->setIconSize(QSize(24, 24));
    } else {
        clubIcon->setText("Club");
    }
    clubIcon->setFixedSize(40, 40);
    clubIcon->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Event icon
    QPushButton* eventIcon = new QPushButton();
    QIcon eventIconImage(":/images/resources/event.png");
    if (!eventIconImage.isNull()) {
        eventIcon->setIcon(eventIconImage);
        eventIcon->setIconSize(QSize(24, 24));
    } else {
        eventIcon->setText("Event");
    }
    eventIcon->setFixedSize(40, 40);
    eventIcon->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Profile icon - Using your PNG image
    QPushButton* profileIcon = new QPushButton();
    QIcon profileIconImage(":/images/resources/user.png");
    if (!profileIconImage.isNull()) {
        profileIcon->setIcon(profileIconImage);
        profileIcon->setIconSize(QSize(24, 24));
    } else {
        profileIcon->setText("Profile");
    }
    profileIcon->setFixedSize(40, 40);
    profileIcon->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    // Add navigation buttons to layout
    m_bottomNavLayout->addStretch(1);
    m_bottomNavLayout->addWidget(homeIcon);
    m_bottomNavLayout->addStretch(1);
    m_bottomNavLayout->addWidget(clubIcon);
    m_bottomNavLayout->addStretch(1);
    m_bottomNavLayout->addWidget(eventIcon);
    m_bottomNavLayout->addStretch(1);
    m_bottomNavLayout->addWidget(profileIcon);
    m_bottomNavLayout->addStretch(1);

    // Connect signals to slots
    connect(homeIcon, &QPushButton::clicked, this, &MProfilePage::homeClicked);
    connect(clubIcon, &QPushButton::clicked, this, &MProfilePage::clubClicked);
    connect(eventIcon, &QPushButton::clicked, this, &MProfilePage::eventClicked);
    connect(profileIcon, &QPushButton::clicked, this, &MProfilePage::profileClicked);

    // Set edit profile pic button position after layout is set
    QTimer::singleShot(0, [this, profileBgWidget, editProfileBtn]() {
        int centerX = profileBgWidget->width() / 2;
        int centerY = profileBgWidget->height() / 2;
        editProfileBtn->move(centerX + 25, centerY + 25);
    });

    // Add all components to main layout
    m_mainLayout->addWidget(profileBgWidget);
    m_mainLayout->addWidget(userInfoWidget);
    m_mainLayout->addWidget(rankingsLabel);
    m_mainLayout->addWidget(rankingsScrollArea, 1); // Give it stretch to take available space
    m_mainLayout->addWidget(m_bottomNavBar);

    // Set fixed width to match window size
    setFixedWidth(350);

    // Set background color for the whole page
    setStyleSheet("background-color: white;");
}

void MProfilePage::homeClicked()
{
    emit navigateToHome();
}

void MProfilePage::clubClicked()
{
    emit navigateToClub();
}

void MProfilePage::eventClicked()
{
    emit navigateToGoing();
}

void MProfilePage::profileClicked()
{
    // Already on profile page, no need to navigate
    emit navigateToProfile();
}
