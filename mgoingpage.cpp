// mgoingpage.cpp
#include "mgoingpage.h"
#include <QIcon>
#include <QFont>
#include <QDebug>
#include <QFrame>
#include <QScrollArea>

MGoingPage::MGoingPage(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "MGoingPage constructor called";
    setupUI();
    qDebug() << "MGoingPage UI setup completed";
}

MGoingPage::~MGoingPage()
{
    qDebug() << "MGoingPage destructor called";
}

void MGoingPage::setupUI()
{
    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Header with title
    QWidget* headerWidget = new QWidget();
    headerWidget->setFixedHeight(50);
    headerWidget->setStyleSheet("background-color: white;");

    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(12, 4, 12, 4);

    m_titleLabel = new QLabel("Clubsphere");
    QFont titleFont;
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    headerLayout->addStretch(1);
    headerLayout->addWidget(m_titleLabel);
    headerLayout->addStretch(1);

    // Content area with scroll support
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet("background-color: #d8e8cd;");

    QWidget* contentWidget = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setSpacing(15);

    // Title label for upcoming events
    QLabel* upcomingLabel = new QLabel("Upcoming Events for you");
    QFont upcomingFont;
    upcomingFont.setPointSize(16);
    upcomingFont.setBold(true);
    upcomingLabel->setFont(upcomingFont);
    upcomingLabel->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(upcomingLabel);
    contentLayout->addSpacing(20);

    // Add club event items
    contentLayout->addWidget(createClubEventItem("Chemistry Club", "20", "20"));
    contentLayout->addWidget(createSeparator());
    contentLayout->addWidget(createClubEventItem("Science Club", "20", "20"));
    contentLayout->addWidget(createSeparator());
    contentLayout->addWidget(createClubEventItem("Table Tennis Club", "20", "20"));
    contentLayout->addWidget(createSeparator());
    contentLayout->addWidget(createClubEventItem("Table Tennis Club", "20", "20"));

    contentLayout->addStretch(1);

    scrollArea->setWidget(contentWidget);

    // Bottom navigation bar - KEEPING THE EXISTING CODE
    m_bottomNavBar = new QWidget();
    m_bottomNavBar->setFixedHeight(60);
    m_bottomNavBar->setStyleSheet("background-color: white; border-top: 1px solid #e0e0e0;");

    m_bottomNavLayout = new QHBoxLayout(m_bottomNavBar);
    m_bottomNavLayout->setContentsMargins(10, 5, 10, 5);
    m_bottomNavLayout->setSpacing(0);

    // Create navigation buttons with icons (matching MClubPage)
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

    // Profile icon
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
    connect(homeIcon, &QPushButton::clicked, this, &MGoingPage::homeClicked);
    connect(clubIcon, &QPushButton::clicked, this, &MGoingPage::clubClicked);
    connect(eventIcon, &QPushButton::clicked, this, &MGoingPage::eventClicked);
    connect(profileIcon, &QPushButton::clicked, this, &MGoingPage::profileClicked);

    // Add all to main layout
    m_mainLayout->addWidget(headerWidget);
    m_mainLayout->addWidget(scrollArea, 1);
    m_mainLayout->addWidget(m_bottomNavBar);

    // Initialize the toggle buttons and stacked widget as empty but instantiated
    // so they exist but don't cause crashes
    m_joinedButton = new QPushButton(this);
    m_joinedButton->hide();

    m_pendingButton = new QPushButton(this);
    m_pendingButton->hide();

    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->hide();

    // Set fixed width to match window size
    setFixedWidth(350);

    qDebug() << "MGoingPage setupUI completed successfully";
}

QWidget* MGoingPage::createClubEventItem(const QString& name, const QString& rank, const QString& members)
{
    QWidget* itemWidget = new QWidget();
    QVBoxLayout* itemLayout = new QVBoxLayout(itemWidget);
    itemLayout->setContentsMargins(0, 10, 0, 10);
    itemLayout->setSpacing(10);

    // Create top row with club logo and info
    QWidget* topRowWidget = new QWidget();
    QHBoxLayout* topRowLayout = new QHBoxLayout(topRowWidget);
    topRowLayout->setContentsMargins(0, 0, 0, 0);

    // Club logo
    QLabel* logoLabel = new QLabel();
    logoLabel->setFixedSize(80, 80);
    logoLabel->setStyleSheet("background-color: #1f3864; border-radius: 5px;");

    // Try to load the image, if failed, create a placeholder
    QPixmap logoPixmap(":/images/resources/club_icon.png");
    if (logoPixmap.isNull()) {
        // Create a placeholder with an atom symbol
        QPixmap placeholder(80, 80);
        placeholder.fill(Qt::transparent);
        QPainter painter(&placeholder);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(QColor("#1f3864")));
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, 80, 80);

        // Draw circle for atom
        painter.setBrush(QBrush(Qt::white));
        painter.setPen(QPen(Qt::white, 2));
        painter.drawEllipse(40, 40, 30, 30);

        // Draw orbital paths
        painter.drawEllipse(40, 40, 50, 20);
        painter.drawEllipse(40, 40, 20, 50);

        logoLabel->setPixmap(placeholder);
    } else {
        logoLabel->setPixmap(logoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    topRowLayout->addWidget(logoLabel);

    // Club info
    QWidget* infoWidget = new QWidget();
    QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(10, 0, 0, 0);
    infoLayout->setSpacing(2);

    QLabel* nameLabel = new QLabel(name);
    QFont nameFont;
    nameFont.setPointSize(14);
    nameFont.setBold(true);
    nameLabel->setFont(nameFont);

    QLabel* rankLabel = new QLabel("Rank: " + rank);
    QLabel* membersLabel = new QLabel("Members: " + members);

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(rankLabel);
    infoLayout->addWidget(membersLabel);
    infoLayout->addStretch();

    topRowLayout->addWidget(infoWidget, 1);

    // Add top row to main layout
    itemLayout->addWidget(topRowWidget);

    // Create bottom row with input field and enter button
    QWidget* bottomRowWidget = new QWidget();
    QHBoxLayout* bottomRowLayout = new QHBoxLayout(bottomRowWidget);
    bottomRowLayout->setContentsMargins(0, 0, 0, 0);
    bottomRowLayout->setSpacing(10);

    // Input field
    QLineEdit* codeInput = new QLineEdit();
    codeInput->setPlaceholderText("Enter event code here...");
    codeInput->setFixedHeight(40);
    codeInput->setStyleSheet(
        "QLineEdit {"
        "    background-color: white;"
        "    border-radius: 20px;"
        "    padding-left: 10px;"
        "    padding-right: 10px;"
        "}"
        );

    // Enter button
    QPushButton* enterButton = new QPushButton("Enter");
    enterButton->setFixedSize(100, 40);
    enterButton->setStyleSheet(
        "QPushButton {"
        "    background-color: white;"
        "    border-radius: 20px;"
        "    font-weight: bold;"
        "    color: #43a047;"
        "}"
        );

    bottomRowLayout->addWidget(codeInput, 1);
    bottomRowLayout->addWidget(enterButton);

    // Add bottom row to main layout
    itemLayout->addWidget(bottomRowWidget);

    return itemWidget;
}
QWidget* MGoingPage::createSeparator()
{
    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setLineWidth(1);
    separator->setStyleSheet("background-color: #888888;");
    return separator;
}

void MGoingPage::showJoinedView()
{
    // No implementation needed for our simplified version
    qDebug() << "showJoinedView called";
}

void MGoingPage::showPendingView()
{
    // No implementation needed for our simplified version
    qDebug() << "showPendingView called";
}

void MGoingPage::homeClicked()
{
    qDebug() << "Home button clicked";
    emit navigateToHome();
}

void MGoingPage::clubClicked()
{
    qDebug() << "Club button clicked";
    emit navigateToClub();
}

void MGoingPage::eventClicked()
{
    qDebug() << "Event button clicked";
    // Already on events page, just log it
    qDebug() << "Already on events page";
}

void MGoingPage::profileClicked()
{
    qDebug() << "Profile button clicked";
    emit navigateToProfile();
}
