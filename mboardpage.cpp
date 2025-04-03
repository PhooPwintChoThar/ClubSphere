#include "mboardpage.h"
#include <QIcon>
#include <QFont>
#include <QFrame>
#include <QRadioButton>
#include <QPainter>
#include <QPixmap>

MBoardPage::MBoardPage(QWidget *parent)
    : QWidget(parent), m_showingMembers(true)
{
    setupUI();
    showMembersLeaderboard();
}

MBoardPage::~MBoardPage()
{
}

void MBoardPage::setupUI()
{
    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Set background color for the entire page to match the image
    setStyleSheet("background-color: #d6e3c6;");

    // Header with app name
    QWidget* headerWidget = new QWidget();
    headerWidget->setFixedHeight(50);
    headerWidget->setStyleSheet("background-color: #d6e3c6;");

    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(12, 4, 12, 4);

    // Back button using PNG
    QPushButton* backButton = new QPushButton();
    backButton->setIcon(QIcon(":/images/resources/back.png"));
    backButton->setIconSize(QSize(32, 32));
    backButton->setFixedSize(32, 32);
    backButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
    connect(backButton, &QPushButton::clicked, this, &MBoardPage::homeClicked);

    // Page title
    m_titleLabel = new QLabel("LeaderBoard");
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(backButton, 0, Qt::AlignLeft);
    headerLayout->addWidget(m_titleLabel, 1, Qt::AlignCenter);
    headerLayout->addSpacing(32); // Balance the back button

    // Tab switching widget - adjust to match the image
    QWidget* tabWidget = new QWidget();
    tabWidget->setStyleSheet("background-color: #d6e3c6;");
    QHBoxLayout* tabLayout = new QHBoxLayout(tabWidget);
    tabLayout->setContentsMargins(20, 10, 20, 10);

    // Create the tab container with more rounded corners to match the image
    QWidget* tabContainer = new QWidget();
    tabContainer->setStyleSheet(
        "QWidget { background-color: white; border-radius: 25px; }"
        );
    tabContainer->setFixedHeight(50); // Set fixed height for the container

    QHBoxLayout* tabContainerLayout = new QHBoxLayout(tabContainer);
    tabContainerLayout->setContentsMargins(5, 5, 5, 5);
    tabContainerLayout->setSpacing(0);

    // Tab group for exclusive selection
    m_tabGroup = new QButtonGroup(this);

    // Members tab - adjusted to match image
    m_membersTab = new QRadioButton("Members");
    m_membersTab->setStyleSheet(
        "QRadioButton { background-color: white; border-radius: 20px; padding: 8px 30px; font-weight: bold; font-size: 16px; }"
        "QRadioButton:checked { background-color: #eaf5dd; }"
        "QRadioButton::indicator { width: 0; height: 0; }"
        );
    m_membersTab->setFixedHeight(40); // Set fixed height to match image
    m_membersTab->setChecked(true);

    // Clubs tab - adjusted to match image
    m_clubsTab = new QRadioButton("Clubs");
    m_clubsTab->setStyleSheet(
        "QRadioButton { background-color: white; border-radius: 20px; padding: 8px 30px; font-weight: bold; font-size: 16px; }"
        "QRadioButton:checked { background-color: #eaf5dd; }"
        "QRadioButton::indicator { width: 0; height: 0; }"
        );
    m_clubsTab->setFixedHeight(40); // Set fixed height to match image

    m_tabGroup->addButton(m_membersTab);
    m_tabGroup->addButton(m_clubsTab);

    tabContainerLayout->addWidget(m_membersTab, 1);
    tabContainerLayout->addWidget(m_clubsTab, 1);

    tabLayout->addWidget(tabContainer);

    // Connect tab signals
    connect(m_membersTab, &QRadioButton::clicked, this, &MBoardPage::onMembersTabClicked);
    connect(m_clubsTab, &QRadioButton::clicked, this, &MBoardPage::onClubsTabClicked);

    // Container for top 3 leaders
    m_topThreeWidget = new QWidget();
    m_topThreeWidget->setStyleSheet("background-color: #d6e3c6; border-bottom-left-radius: 30px; border-bottom-right-radius: 30px;");

    // Scroll area only for the list items below top 3
    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setStyleSheet("background-color: transparent; border: none;");

    // Container for the list items that will be inside the scroll area
    QWidget* listContainer = new QWidget();
    listContainer->setStyleSheet("background-color: #d6e3c6;");
    m_leaderListLayout = new QVBoxLayout(listContainer);
    m_leaderListLayout->setContentsMargins(10, 10, 10, 10);
    m_leaderListLayout->setSpacing(20);

    // Set the list container as the scroll area widget
    m_scrollArea->setWidget(listContainer);

    // Add all to main layout
    m_mainLayout->addWidget(headerWidget);
    m_mainLayout->addWidget(tabWidget);
    m_mainLayout->addWidget(m_topThreeWidget);
    m_mainLayout->addWidget(m_scrollArea, 1); // Scroll area takes remaining space

    // Set fixed width to match window size
    setFixedWidth(350);
}

void MBoardPage::setupTopLeaders()
{
    // Clear previous content
    if (m_topThreeWidget->layout()) {
        QLayoutItem* item;
        while ((item = m_topThreeWidget->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete m_topThreeWidget->layout();
    }

    QHBoxLayout* topLeadersLayout = new QHBoxLayout(m_topThreeWidget);
    topLeadersLayout->setContentsMargins(10, 20, 10, 30);
    topLeadersLayout->setSpacing(0);

    if (m_showingMembers) {
        // Second place - left
        QWidget* secondPlace = createLeaderItem("Mary", 1000, true, 2);

        // First place - center
        QWidget* firstPlace = createLeaderItem("Ashley", 1500, true, 1);

        // Third place - right
        QWidget* thirdPlace = createLeaderItem("John", 950, true, 3);

        topLeadersLayout->addWidget(secondPlace, 1, Qt::AlignBottom);
        topLeadersLayout->addWidget(firstPlace, 1, Qt::AlignBottom);
        topLeadersLayout->addWidget(thirdPlace, 1, Qt::AlignBottom);
    } else {
        // Second place - left
        QWidget* secondPlace = createLeaderItem("Science", 1000, true, 2);

        // First place - center
        QWidget* firstPlace = createLeaderItem("Badminton", 1500, true, 1);

        // Third place - right
        QWidget* thirdPlace = createLeaderItem("Art", 950, true, 3);

        topLeadersLayout->addWidget(secondPlace, 1, Qt::AlignBottom);
        topLeadersLayout->addWidget(firstPlace, 1, Qt::AlignBottom);
        topLeadersLayout->addWidget(thirdPlace, 1, Qt::AlignBottom);
    }
}

void MBoardPage::setupListItems()
{
    // Clear previous items
    QLayoutItem* item;
    while ((item = m_leaderListLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    if (m_showingMembers) {
        // Add member items
        m_leaderListLayout->addWidget(createLeaderItem("Emma", 860));
        m_leaderListLayout->addWidget(createLeaderItem("Johnathan", 700));
        m_leaderListLayout->addWidget(createLeaderItem("Andrew", 630));
        m_leaderListLayout->addWidget(createLeaderItem("Jessica", 600));
        m_leaderListLayout->addWidget(createLeaderItem("Anna", 590));
    } else {
        // Add club items
        m_leaderListLayout->addWidget(createLeaderItem("Dance Club", 860));
        m_leaderListLayout->addWidget(createLeaderItem("Table Tennis", 700));
        m_leaderListLayout->addWidget(createLeaderItem("Astrology", 630));
        m_leaderListLayout->addWidget(createLeaderItem("Basketball", 600));
        m_leaderListLayout->addWidget(createLeaderItem("Music", 590));
    }

    // Add stretch to push items to the top
    m_leaderListLayout->addStretch(1);
}

QWidget* MBoardPage::createLeaderItem(const QString& name, int points, bool isTopThree, int rank)
{
    QWidget* itemWidget = new QWidget();
    itemWidget->setStyleSheet("background-color: transparent;");

    QVBoxLayout* itemLayout = new QVBoxLayout(itemWidget);
    itemLayout->setContentsMargins(5, 5, 5, 5);
    itemLayout->setSpacing(5);
    itemLayout->setAlignment(Qt::AlignCenter);

    if (isTopThree) {
        // Create medal widget based on rank
        QLabel* medalLabel = new QLabel();

        // Use PNG images for medals instead of drawing them
        QString medalPath;
        if (rank == 1) {
            medalPath = ":/images/resources/gold_medal.png";
        } else if (rank == 2) {
            medalPath = ":/images/resources/silver_medal.png";
        } else {
            medalPath = ":/images/resources/bronze_medal.png";
        }

        medalLabel->setPixmap(QPixmap(medalPath).scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        medalLabel->setFixedSize(40, 40);
        itemLayout->addWidget(medalLabel, 0, Qt::AlignCenter);

        // Profile picture using PNG
        QLabel* profilePic = new QLabel();
        profilePic->setFixedSize(80, 80);
        profilePic->setPixmap(QPixmap(":/images/resources/user.png").scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        itemLayout->addWidget(profilePic, 0, Qt::AlignCenter);

        // Points
        QLabel* pointsLabel = new QLabel(QString::number(points));
        QFont pointsFont;
        pointsFont.setPointSize(16);
        pointsFont.setBold(true);
        pointsLabel->setFont(pointsFont);
        pointsLabel->setAlignment(Qt::AlignCenter);
        itemLayout->addWidget(pointsLabel);

        // Name
        QLabel* nameLabel = new QLabel(name);
        nameLabel->setAlignment(Qt::AlignCenter);
        itemLayout->addWidget(nameLabel);
    } else {
        // Regular list item (not top three)
        QHBoxLayout* rowLayout = new QHBoxLayout();
        rowLayout->setContentsMargins(10, 5, 10, 5);
        rowLayout->setSpacing(10);

        // Profile picture using PNG
        QLabel* profilePic = new QLabel();
        profilePic->setFixedSize(50, 50);
        profilePic->setPixmap(QPixmap(":/images/resources/user.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        rowLayout->addWidget(profilePic);

        // Name with larger text
        QLabel* nameLabel = new QLabel(name);
        QFont nameFont;
        nameFont.setPointSize(14);
        nameFont.setBold(true);
        nameLabel->setFont(nameFont);
        rowLayout->addWidget(nameLabel, 1);

        // Points with large text
        QLabel* pointsLabel = new QLabel(QString::number(points));
        QFont pointsFont;
        pointsFont.setPointSize(16);
        pointsFont.setBold(true);
        pointsLabel->setFont(pointsFont);
        rowLayout->addWidget(pointsLabel);

        itemLayout->addLayout(rowLayout);
    }

    return itemWidget;
}

void MBoardPage::clearLeaderboard()
{
    // This will be called before switching between Members and Clubs view
    setupTopLeaders();
    setupListItems();
}

void MBoardPage::showMembersLeaderboard()
{
    m_showingMembers = true;
    clearLeaderboard();
}

void MBoardPage::showClubsLeaderboard()
{
    m_showingMembers = false;
    clearLeaderboard();
}

void MBoardPage::onMembersTabClicked()
{
    showMembersLeaderboard();
}

void MBoardPage::onClubsTabClicked()
{
    showClubsLeaderboard();
}

void MBoardPage::homeClicked()
{
    emit navigateToHome();
}

void MBoardPage::clubClicked()
{
    emit navigateToClub();
}

void MBoardPage::eventClicked()
{
    emit navigateToGoing();
}

void MBoardPage::profileClicked()
{
    emit navigateToProfile();
}

void MBoardPage::boardClicked()
{
    // Already on board page, no need to navigate
    emit navigateToBoard();
}
