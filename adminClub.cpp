#include "adminClub.h"
#include <QPixmap>
#include <QIcon>
#include <QDebug>
#include<QPainter>
AdminClub::AdminClub(QWidget *parent) : QWidget(parent)
{
    setupUI();
    setupClubList();
    setupNavigation();
}

AdminClub::~AdminClub()
{
}

void AdminClub::setupUI()
{
    // Set background color to white and fix window size
    this->setStyleSheet("background-color: white;");
    this->setFixedSize(350, 650);  // Changed from 340x680 to 350x650

    // Main layout setup
    mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setContentsMargins(20, 30, 20, 20);  // Reduced vertical margins
    mainLayout->setSpacing(15);  // Reduced spacing

    // Title
    titleLabel = new QLabel("Clubsphere", this);
    titleLabel->setFont(QFont("Arial", 16, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Search bar
    searchFrame = new QFrame(this);
    searchFrame->setFrameShape(QFrame::StyledPanel);
    searchFrame->setStyleSheet("QFrame { background-color: #F0F0F0; border-radius: 12px; }");

    QHBoxLayout *searchLayout = new QHBoxLayout(searchFrame);
    searchLayout->setContentsMargins(15, 8, 15, 8);  // Reduced vertical padding

    QLabel *searchIcon = new QLabel(this);
    searchIcon->setPixmap(QIcon(":/images/resources/search_logo.png").pixmap(20, 20));

    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Search");
    searchEdit->setStyleSheet("QLineEdit { border: none; background-color: transparent; }");

    searchLayout->addWidget(searchIcon);
    searchLayout->addWidget(searchEdit);

    mainLayout->addWidget(searchFrame);

    // Clubs label and add button
    clubsHeaderLayout = new QHBoxLayout();

    clubsLabel = new QLabel("Clubs:", this);
    clubsLabel->setFont(QFont("Arial", 18, QFont::Bold));

    addClubButton = new QPushButton(this);
    addClubButton->setIcon(QIcon(":/images/resources/plus_logo.png"));
    addClubButton->setIconSize(QSize(15, 15));
    addClubButton->setFixedSize(40, 40);  // Slightly smaller button
    addClubButton->setStyleSheet("QPushButton { background-color: #D9E9D8; border-radius: 20px; }");

    clubsHeaderLayout->addWidget(clubsLabel);
    clubsHeaderLayout->addStretch();
    clubsHeaderLayout->addWidget(addClubButton);

    mainLayout->addLayout(clubsHeaderLayout);

    // Scroll area for club list
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollContent = new QWidget(scrollArea);
    clubsLayout = new QVBoxLayout(scrollContent);
    clubsLayout->setAlignment(Qt::AlignTop);
    clubsLayout->setContentsMargins(0, 0, 0, 0);
    clubsLayout->setSpacing(15);  // Reduced spacing

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    // Connect add club button
    connect(addClubButton, &QPushButton::clicked, this, &AdminClub::onAddClubClicked);
}

void AdminClub::setupClubList()
{
    // Add club cards
    createClubCard("Music Club", 14, "John");
    createClubCard("Music Club", 14, "John");
    createClubCard("Music Club", 14, "John");
    createClubCard("Music Club", 14, "John");

    // Add spacer to push navigation bar to bottom
    mainLayout->addStretch();
}

void AdminClub::createClubCard(const QString &name, int members, const QString &leader)
{
    QFrame *clubFrame = new QFrame(this);
    clubFrame->setFrameShape(QFrame::NoFrame);
    clubFrame->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *cardLayout = new QHBoxLayout(clubFrame);
    cardLayout->setContentsMargins(0, 8, 0, 8);  // Reduced vertical padding

    // Club image
    QLabel *clubImage = new QLabel(this);
    QPixmap defaultClubImage(":/images/resources/default_club.png");
    if (defaultClubImage.isNull()) {
        // Create a placeholder if image is not found
        defaultClubImage = QPixmap(80, 80);  // Slightly smaller image
        defaultClubImage.fill(Qt::lightGray);

        // Draw placeholder image icon
        QPainter painter(&defaultClubImage);
        painter.setPen(Qt::darkGray);
        painter.drawRect(20, 20, 50, 40);
        painter.drawLine(20, 30, 40, 50);
        painter.drawEllipse(50, 30, 10, 10);
    }

    clubImage->setPixmap(defaultClubImage);
    clubImage->setFixedSize(80, 80);  // Slightly smaller image
    clubImage->setStyleSheet("background-color: #E0E0E0;");

    // Club info
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(3);  // Reduced spacing

    QLabel *nameLabel = new QLabel(name, this);
    nameLabel->setFont(QFont("Arial", 14, QFont::Bold));  // Smaller font

    QLabel *membersLabel = new QLabel(QString::number(members) + " members", this);
    membersLabel->setFont(QFont("Arial", 11));  // Smaller font

    QLabel *leaderLabel = new QLabel("Leader: " + leader, this);
    leaderLabel->setFont(QFont("Arial", 11));  // Smaller font

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(membersLabel);
    infoLayout->addWidget(leaderLabel);

    // Action buttons
    QVBoxLayout *actionsLayout = new QVBoxLayout();
    actionsLayout->setSpacing(8);  // Reduced spacing

    QPushButton *assignLeadersButton = new QPushButton("Assign Leaders", this);
    assignLeadersButton->setStyleSheet("QPushButton { background-color: #D9E9D8; border-radius: 12px; padding: 6px 10px; }");  // Smaller padding

    QPushButton *deleteButton = new QPushButton("Delete", this);
    deleteButton->setStyleSheet("QPushButton { background-color: #E0E0E0; border-radius: 12px; padding: 6px 10px; color: #FF0000; }");  // Smaller padding

    actionsLayout->addWidget(assignLeadersButton);
    actionsLayout->addWidget(deleteButton);

    cardLayout->addWidget(clubImage);
    cardLayout->addLayout(infoLayout, 1);
    cardLayout->addLayout(actionsLayout);

    clubsLayout->addWidget(clubFrame);

    // Add separator line except for the last item
    if (clubsLayout->count() < 4) {
        QFrame *line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setStyleSheet("background-color: #E0E0E0;");
        line->setMaximumHeight(1);
        clubsLayout->addWidget(line);
    }
}

void AdminClub::setupNavigation()
{
    // Bottom navigation bar
    navigationFrame = new QFrame(this);
    navigationFrame->setFrameShape(QFrame::NoFrame);
    navigationFrame->setLineWidth(0);
    navigationFrame->setStyleSheet("QFrame { background-color: white; }");

    QHBoxLayout *navLayout = new QHBoxLayout(navigationFrame);
    navLayout->setContentsMargins(16, 10, 16, 5);  // Reduced padding
    navLayout->setSpacing(45);  // Adjusted spacing for new width

    // Create navigation icons
    homeButton = new QPushButton("", this);
    homeButton->setIcon(QIcon(":/images/resources/home_logo.png"));
    homeButton->setIconSize(QSize(20, 20));
    homeButton->setStyleSheet("QPushButton { border: none; }");

    profileButton = new QPushButton("", this);
    profileButton->setIcon(QIcon(":/images/resources/member_logo.png"));
    profileButton->setIconSize(QSize(20, 20));
    profileButton->setStyleSheet("QPushButton { border: none; }");

    groupsButton = new QPushButton("", this);
    groupsButton->setIcon(QIcon(":/images/resources/club_logo.png"));
    groupsButton->setIconSize(QSize(30, 30));
    groupsButton->setStyleSheet("QPushButton { border: none; background-color: #D9E9D8; border-radius: 5px; padding: 5px; }");

    notificationsButton = new QPushButton("", this);
    notificationsButton->setIcon(QIcon(":/images/resources/noti_logo.png"));
    notificationsButton->setIconSize(QSize(20, 20));
    notificationsButton->setStyleSheet("QPushButton { border: none; }");

    navLayout->addWidget(homeButton);
    navLayout->addWidget(profileButton);
    navLayout->addWidget(groupsButton);
    navLayout->addWidget(notificationsButton);

    mainLayout->addWidget(navigationFrame);

    // Connect navigation buttons
    connect(homeButton, &QPushButton::clicked, this, &AdminClub::onHomeButtonClicked);
    connect(profileButton, &QPushButton::clicked, this, &AdminClub::onProfileButtonClicked);
    connect(groupsButton, &QPushButton::clicked, this, &AdminClub::onGroupsButtonClicked);
    connect(notificationsButton, &QPushButton::clicked, this, &AdminClub::onNotificationsButtonClicked);
}

void AdminClub::onHomeButtonClicked()
{
    qDebug() << "Home button clicked in AdminClub";
    emit navigateToHome();
}

void AdminClub::onProfileButtonClicked()
{
    qDebug() << "Profile button clicked in AdminClub";
    emit navigateToMembers();
}

void AdminClub::onGroupsButtonClicked()
{
    qDebug() << "Groups button clicked in AdminClub - already on this page";
    // No navigation needed as we're already on this page
}

void AdminClub::onNotificationsButtonClicked()
{
    qDebug() << "Notifications button clicked in AdminClub";
    emit navigateToNotifications();
}

void AdminClub::onAddClubClicked()
{
    qDebug() << "Add club button clicked in AdminClub";
    // Implement club addition functionality here
}

QFrame* AdminClub::createRoundedFrame()
{
    QFrame* frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setLineWidth(0);
    frame->setMinimumWidth(130);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    return frame;
}
