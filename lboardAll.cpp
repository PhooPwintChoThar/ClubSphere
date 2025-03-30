#include "lboardAll.h"
#include <QPixmap>
#include <QIcon>
#include <QDebug>

LeaderboardAll::LeaderboardAll(QWidget *parent) : QWidget(parent), currentTab(ALL_TAB)
{
    setupUI();
    setupTabBar();
    setupTopRankersSection();
    setupRankingListSection();

    // Load data
    loadMemberData();
    loadClubData();

    // Initialize with All tab - explicitly call onAllTabClicked()
    onAllTabClicked();
}

LeaderboardAll::~LeaderboardAll()
{
}

void LeaderboardAll::setupUI()
{
    // Set background color to light green and fix window size
    this->setStyleSheet("background-color: #D9E9D8;");
    this->setFixedSize(350, 650);

    // Main layout setup
    mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setContentsMargins(10, 20, 10, 10);
    mainLayout->setSpacing(15);

    // Header with back button and title
    headerWidget = new QWidget(this);
    headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    backButton = new QPushButton(this);
    backButton->setIcon(QIcon(":/resources/back.png"));
    backButton->setIconSize(QSize(20, 20));
    backButton->setStyleSheet("QPushButton { border: none; }");

    titleLabel = new QLabel("LeaderBoard", this);
    titleLabel->setFont(QFont("Arial", 20, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(backButton);
    headerLayout->addWidget(titleLabel, 1);
    headerLayout->addSpacing(backButton->sizeHint().width()); // Balance the header

    mainLayout->addWidget(headerWidget);

    // Connect back button
    connect(backButton, &QPushButton::clicked, this, &LeaderboardAll::onBackButtonClicked);
}

void LeaderboardAll::setupTabBar()
{
    // Create tab bar widget
    tabWidget = new QWidget(this);
    tabWidget->setObjectName("tabWidget");
    tabWidget->setStyleSheet("QWidget#tabWidget { background-color: white; border-radius: 22px; }");

    tabLayout = new QHBoxLayout(tabWidget);
    tabLayout->setContentsMargins(8, 8, 8, 8);
    tabLayout->setSpacing(0);

    // Create tab buttons
    QFont tabFont;
    tabFont.setPointSize(12);
    tabFont.setBold(true);

    allTabButton = new QPushButton("All", this);
    allTabButton->setFont(tabFont);
    allTabButton->setCheckable(true);
    allTabButton->setChecked(true);

    membersTabButton = new QPushButton("Members", this);
    membersTabButton->setFont(tabFont);
    membersTabButton->setCheckable(true);

    clubsTabButton = new QPushButton("Clubs", this);
    clubsTabButton->setFont(tabFont);
    clubsTabButton->setCheckable(true);

    // Set initial button styles (will be updated in updateTabButtonStyles())
    updateTabButtonStyles();

    tabLayout->addWidget(allTabButton);
    tabLayout->addWidget(membersTabButton);
    tabLayout->addWidget(clubsTabButton);

    mainLayout->addWidget(tabWidget);

    // Connect tab button signals
    connect(allTabButton, &QPushButton::clicked, this, &LeaderboardAll::onAllTabClicked);
    connect(membersTabButton, &QPushButton::clicked, this, &LeaderboardAll::onMembersTabClicked);
    connect(clubsTabButton, &QPushButton::clicked, this, &LeaderboardAll::onClubsTabClicked);
}

void LeaderboardAll::setupTopRankersSection()
{
    // Create top rankers section (will be populated based on tab)
    topRankersWidget = new QWidget(this);
    topRankersWidget->setStyleSheet("background-color: #D9E9D8; border-radius: 25px;");
    topRankersWidget->setMinimumHeight(180);

    topRankersLayout = new QHBoxLayout(topRankersWidget);
    topRankersLayout->setContentsMargins(20, 20, 20, 20);
    topRankersLayout->setSpacing(10);
    topRankersLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(topRankersWidget);
}

void LeaderboardAll::setupRankingListSection()
{
    // Create scrollable area for rankings list
    rankingsScrollArea = new QScrollArea(this);
    rankingsScrollArea->setWidgetResizable(true);
    rankingsScrollArea->setFrameShape(QFrame::NoFrame);
    rankingsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rankingsScrollArea->setStyleSheet("background-color: #D9E9D8;");

    rankingsListWidget = new QWidget();
    rankingsListLayout = new QVBoxLayout(rankingsListWidget);
    rankingsListLayout->setContentsMargins(10, 10, 10, 10);
    rankingsListLayout->setSpacing(15);
    rankingsListLayout->setAlignment(Qt::AlignTop);

    rankingsScrollArea->setWidget(rankingsListWidget);
    mainLayout->addWidget(rankingsScrollArea, 1);
}

void LeaderboardAll::loadMemberData()
{
    // Sample member data - in a real app this would come from a database or API
    memberList.clear();
    memberList.append({"Ashley", 1500});
    memberList.append({"Mary", 1000});
    memberList.append({"John", 950});
    memberList.append({"Emma", 860});
    memberList.append({"Johnathan", 700});
    memberList.append({"Andrew", 630});
    memberList.append({"Jessica", 600});
    memberList.append({"Anna", 590});
}

void LeaderboardAll::loadClubData()
{
    // Sample club data - in a real app this would come from a database or API
    clubList.clear();
    clubList.append({"Badminton", 1500});
    clubList.append({"Science", 1000});
    clubList.append({"Art", 950});
    clubList.append({"Dance Club", 860});
    clubList.append({"Table Tennis", 700});
    clubList.append({"Astrology", 630});
    clubList.append({"Basketball", 600});
    clubList.append({"Music", 590});
}

void LeaderboardAll::createSectionHeader(QVBoxLayout *layout, const QString &title)
{
    // Add "This Week" label
    QLabel *weekLabel = new QLabel("This Week");
    weekLabel->setStyleSheet("color: #5A5A5A; font-size: 14px;");
    layout->addWidget(weekLabel);

    // Add title label
    QLabel *titleLabel = new QLabel(title);
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);

    // Add some spacing after the title
    layout->addSpacing(10);
}

void LeaderboardAll::updateMembersView()
{
    // Clear existing widgets
    QLayoutItem *item;
    while ((item = topRankersLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    while ((item = rankingsListLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Add top 3 members with medals
    if (memberList.size() > 0) {
        // Second place (displayed on left)
        if (memberList.size() > 1) {
            topRankersLayout->addWidget(createTopRankerWidget("silver", memberList[1].name, memberList[1].score));
        }

        // First place (displayed in center, larger)
        topRankersLayout->addWidget(createTopRankerWidget("gold", memberList[0].name, memberList[0].score));

        // Third place (displayed on right)
        if (memberList.size() > 2) {
            topRankersLayout->addWidget(createTopRankerWidget("bronze", memberList[2].name, memberList[2].score));
        }
    }

    // Add remaining members to scrollable list
    for (int i = 3; i < memberList.size(); i++) {
        rankingsListLayout->addWidget(createRankListItemWidget(memberList[i].name, memberList[i].score));
    }
}

void LeaderboardAll::updateClubsView()
{
    // Clear existing widgets
    QLayoutItem *item;
    while ((item = topRankersLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    while ((item = rankingsListLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Add top 3 clubs with medals
    if (clubList.size() > 0) {
        // Second place (displayed on left)
        if (clubList.size() > 1) {
            topRankersLayout->addWidget(createTopRankerWidget("silver", clubList[1].name, clubList[1].score));
        }

        // First place (displayed in center, larger)
        topRankersLayout->addWidget(createTopRankerWidget("gold", clubList[0].name, clubList[0].score));

        // Third place (displayed on right)
        if (clubList.size() > 2) {
            topRankersLayout->addWidget(createTopRankerWidget("bronze", clubList[2].name, clubList[2].score));
        }
    }

    // Add remaining clubs to scrollable list
    for (int i = 3; i < clubList.size(); i++) {
        rankingsListLayout->addWidget(createRankListItemWidget(clubList[i].name, clubList[i].score));
    }
}

void LeaderboardAll::updateAllView()
{
    // Hide top rankers widget for All tab
    topRankersWidget->hide();

    // Clear existing widgets in scrollable area
    QLayoutItem *item;
    while ((item = rankingsListLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Create section for TOP 3 Clubs
    createSectionHeader(rankingsListLayout, "TOP 3 Clubs");

    // Create individual widgets for each club (instead of one widget containing all clubs)
    if (clubList.size() > 0) {
        QWidget *clubWidget = new QWidget();
        clubWidget->setStyleSheet("background-color: #FFFFFF; border-radius: 15px;");
        QHBoxLayout *clubLayout = new QHBoxLayout(clubWidget);
        clubLayout->setContentsMargins(10, 10, 10, 10);

        QLabel *medalLabel = createMedalLabel("gold");
        clubLayout->addWidget(medalLabel);

        QLabel *nameLabel = new QLabel(clubList[0].name + " Club");
        QFont nameFont;
        nameFont.setPointSize(16);
        nameFont.setBold(true);
        nameLabel->setFont(nameFont);
        clubLayout->addWidget(nameLabel);

        clubLayout->addStretch();

        rankingsListLayout->addWidget(clubWidget);
    }

    if (clubList.size() > 1) {
        QWidget *clubWidget = new QWidget();
        clubWidget->setStyleSheet("background-color: #FFFFFF; border-radius: 15px;");
        QHBoxLayout *clubLayout = new QHBoxLayout(clubWidget);
        clubLayout->setContentsMargins(10, 10, 10, 10);

        QLabel *medalLabel = createMedalLabel("silver");
        clubLayout->addWidget(medalLabel);

        QLabel *nameLabel = new QLabel(clubList[1].name + " Club");
        QFont nameFont;
        nameFont.setPointSize(16);
        nameFont.setBold(true);
        nameLabel->setFont(nameFont);
        clubLayout->addWidget(nameLabel);

        clubLayout->addStretch();

        rankingsListLayout->addWidget(clubWidget);
    }

    if (clubList.size() > 2) {
        QWidget *clubWidget = new QWidget();
        clubWidget->setStyleSheet("background-color: #FFFFFF; border-radius: 15px;");
        QHBoxLayout *clubLayout = new QHBoxLayout(clubWidget);
        clubLayout->setContentsMargins(10, 10, 10, 10);

        QLabel *medalLabel = createMedalLabel("bronze");
        clubLayout->addWidget(medalLabel);

        QLabel *nameLabel = new QLabel(clubList[2].name + " Club");
        QFont nameFont;
        nameFont.setPointSize(16);
        nameFont.setBold(true);
        nameLabel->setFont(nameFont);
        clubLayout->addWidget(nameLabel);

        clubLayout->addStretch();

        rankingsListLayout->addWidget(clubWidget);
    }

    // Add a spacer widget to create a gap between sections
    QWidget *spacerWidget = new QWidget();
    spacerWidget->setFixedHeight(20); // Original spacing
    rankingsListLayout->addWidget(spacerWidget);

    // Create section for TOP 3 Users
    createSectionHeader(rankingsListLayout, "TOP 3 Users");

    // Create individual widgets for each user (instead of one widget containing all users)
    if (memberList.size() > 0) {
        QWidget *userWidget = new QWidget();
        userWidget->setStyleSheet("background-color: #FFFFFF; border-radius: 15px;");
        QHBoxLayout *userLayout = new QHBoxLayout(userWidget);
        userLayout->setContentsMargins(10, 10, 10, 10);

        QLabel *medalLabel = createMedalLabel("gold");
        userLayout->addWidget(medalLabel);

        QLabel *nameLabel = new QLabel(memberList[0].name);
        QFont nameFont;
        nameFont.setPointSize(16);
        nameFont.setBold(true);
        nameLabel->setFont(nameFont);
        userLayout->addWidget(nameLabel);

        userLayout->addStretch();

        rankingsListLayout->addWidget(userWidget);
    }

    if (memberList.size() > 1) {
        QWidget *userWidget = new QWidget();
        userWidget->setStyleSheet("background-color: #FFFFFF; border-radius: 15px;");
        QHBoxLayout *userLayout = new QHBoxLayout(userWidget);
        userLayout->setContentsMargins(10, 10, 10, 10);

        QLabel *medalLabel = createMedalLabel("silver");
        userLayout->addWidget(medalLabel);

        QLabel *nameLabel = new QLabel(memberList[1].name);
        QFont nameFont;
        nameFont.setPointSize(16);
        nameFont.setBold(true);
        nameLabel->setFont(nameFont);
        userLayout->addWidget(nameLabel);

        userLayout->addStretch();

        rankingsListLayout->addWidget(userWidget);
    }

    if (memberList.size() > 2) {
        QWidget *userWidget = new QWidget();
        userWidget->setStyleSheet("background-color: #FFFFFF; border-radius: 15px;");
        QHBoxLayout *userLayout = new QHBoxLayout(userWidget);
        userLayout->setContentsMargins(10, 10, 10, 10);

        QLabel *medalLabel = createMedalLabel("bronze");
        userLayout->addWidget(medalLabel);

        QLabel *nameLabel = new QLabel(memberList[2].name);
        QFont nameFont;
        nameFont.setPointSize(16);
        nameFont.setBold(true);
        nameLabel->setFont(nameFont);
        userLayout->addWidget(nameLabel);

        userLayout->addStretch();

        rankingsListLayout->addWidget(userWidget);
    }
}

QWidget* LeaderboardAll::createTopUserWidget(const QString &medalType, const QString &name)
{
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(5, 5, 5, 5);

    // Medal icon
    QLabel *medalLabel = createMedalLabel(medalType);
    layout->addWidget(medalLabel);

    // User name
    QLabel *nameLabel = new QLabel(name);
    QFont nameFont;
    nameFont.setPointSize(16);
    nameFont.setBold(true);
    nameLabel->setFont(nameFont);
    layout->addWidget(nameLabel);

    layout->addStretch();

    return widget;
}

QWidget* LeaderboardAll::createTopClubWidget(const QString &medalType, const QString &name)
{
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(5, 5, 5, 5);

    // Medal icon
    QLabel *medalLabel = createMedalLabel(medalType);
    layout->addWidget(medalLabel);

    // Club name
    QLabel *nameLabel = new QLabel(name + " Club");
    QFont nameFont;
    nameFont.setPointSize(16);
    nameFont.setBold(true);
    nameLabel->setFont(nameFont);
    layout->addWidget(nameLabel);

    layout->addStretch();

    return widget;
}

QWidget* LeaderboardAll::createTopRankerWidget(const QString &medalType, const QString &name, int score)
{
    QWidget *rankerWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(rankerWidget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(5);

    // Medal image
    QLabel *medalLabel = createMedalLabel(medalType);
    layout->addWidget(medalLabel, 0, Qt::AlignCenter);

    // Avatar (circle with user icon)
    QLabel *avatarLabel = new QLabel();
    QPixmap avatarPixmap(":/resources/user.png");
    if (avatarPixmap.isNull()) {
        // Create a default circular avatar if image not found
        avatarPixmap = QPixmap(60, 60);
        avatarPixmap.fill(Qt::lightGray);
    }
    avatarPixmap = avatarPixmap.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    avatarLabel->setPixmap(avatarPixmap);
    avatarLabel->setFixedSize(60, 60);
    avatarLabel->setStyleSheet("border-radius: 30px; background-color: lightgray;");

    // Make center avatar larger if it's gold (first place)
    if (medalType == "gold") {
        avatarLabel->setFixedSize(80, 80);
        avatarLabel->setStyleSheet("border-radius: 40px; background-color: lightgray;");
    }

    layout->addWidget(avatarLabel, 0, Qt::AlignCenter);

    // Score
    QLabel *scoreLabel = new QLabel(QString::number(score));
    QFont scoreFont;
    scoreFont.setPointSize(14);
    scoreFont.setBold(true);
    scoreLabel->setFont(scoreFont);
    layout->addWidget(scoreLabel, 0, Qt::AlignCenter);

    // Name
    QLabel *nameLabel = new QLabel(name);
    QFont nameFont;
    nameFont.setPointSize(12);
    nameLabel->setFont(nameFont);
    layout->addWidget(nameLabel, 0, Qt::AlignCenter);

    return rankerWidget;
}

QWidget* LeaderboardAll::createRankListItemWidget(const QString &name, int score)
{
    QWidget *listItemWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(listItemWidget);
    layout->setContentsMargins(10, 5, 10, 5);

    // Avatar
    QLabel *avatarLabel = new QLabel();
    QPixmap avatarPixmap(":/resources/user_avatar.png");
    if (avatarPixmap.isNull()) {
        // Create a default circular avatar if image not found
        avatarPixmap = QPixmap(40, 40);
        avatarPixmap.fill(Qt::lightGray);
    }
    avatarPixmap = avatarPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    avatarLabel->setPixmap(avatarPixmap);
    avatarLabel->setFixedSize(40, 40);
    avatarLabel->setStyleSheet("border-radius: 20px; background-color: lightgray;");

    // Name
    QLabel *nameLabel = new QLabel(name);
    QFont nameFont;
    nameFont.setPointSize(12);
    nameFont.setBold(true);
    nameLabel->setFont(nameFont);

    // Score
    QLabel *scoreLabel = new QLabel(QString::number(score));
    QFont scoreFont;
    scoreFont.setPointSize(12);
    scoreFont.setBold(true);
    scoreLabel->setFont(scoreFont);

    layout->addWidget(avatarLabel);
    layout->addWidget(nameLabel);
    layout->addStretch();
    layout->addWidget(scoreLabel);

    return listItemWidget;
}

void LeaderboardAll::onBackButtonClicked()
{
    qDebug() << "Back button clicked in LeaderboardAll";
    emit navigateBack();
}

void LeaderboardAll::onAllTabClicked()
{
    if (currentTab != ALL_TAB) {
        allTabButton->setChecked(true);
        membersTabButton->setChecked(false);
        clubsTabButton->setChecked(false);
        updateTabButtonStyles();

        currentTab = ALL_TAB;
        updateAllView();

        qDebug() << "All tab selected";
    }
}

void LeaderboardAll::onMembersTabClicked()
{
    if (currentTab != MEMBERS_TAB) {
        allTabButton->setChecked(false);
        membersTabButton->setChecked(true);
        clubsTabButton->setChecked(false);
        updateTabButtonStyles();

        // Show top rankers widget if it was hidden
        topRankersWidget->show();

        currentTab = MEMBERS_TAB;
        updateMembersView();

        qDebug() << "Members tab selected";
    }
}

void LeaderboardAll::onClubsTabClicked()
{
    if (currentTab != CLUBS_TAB) {
        allTabButton->setChecked(false);
        membersTabButton->setChecked(false);
        clubsTabButton->setChecked(true);
        updateTabButtonStyles();

        // Show top rankers widget if it was hidden
        topRankersWidget->show();

        currentTab = CLUBS_TAB;
        updateClubsView();

        qDebug() << "Clubs tab selected";
    }
}

void LeaderboardAll::updateTabButtonStyles()
{
    // Default style for unselected tabs
    QString unselectedStyle = "QPushButton { background-color: white; border-radius: 18px; padding: 8px 15px; }";

    // Style for selected tab
    QString selectedStyle = "QPushButton { background-color: #D9E9D8; border-radius: 18px; padding: 8px 15px; }";

    // Apply styles based on checked state
    allTabButton->setStyleSheet(allTabButton->isChecked() ? selectedStyle : unselectedStyle);
    membersTabButton->setStyleSheet(membersTabButton->isChecked() ? selectedStyle : unselectedStyle);
    clubsTabButton->setStyleSheet(clubsTabButton->isChecked() ? selectedStyle : unselectedStyle);
}

QLabel* LeaderboardAll::createMedalLabel(const QString &color)
{
    QLabel* medalLabel = new QLabel();

    // Use image paths based on the referenced code
    QString imagePath;
    if (color == "gold") {
        imagePath = ":/resources/gold_medal.png";
    } else if (color == "silver") {
        imagePath = ":/resources/silver_medal.png";
    } else if (color == "bronze") {
        imagePath = ":/resources/bronze_medal.png";
    }

    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        pixmap = pixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        medalLabel->setPixmap(pixmap);
    } else {
        // Fallback if image isn't found
        medalLabel->setText(color);
    }

    medalLabel->setFixedSize(30, 30);
    return medalLabel;
}
