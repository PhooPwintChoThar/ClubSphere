#include "mclubpage.h"
#include "database.h"
#include "user.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QLineEdit>
#include <QStackedWidget>
#include <QPainter>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>

MClubPage::MClubPage(int userId, QWidget *parent) : QWidget(parent), m_userId(userId)
{
    setupUI();
    loadUserClubs();
}

MClubPage::~MClubPage()
{
}

void MClubPage::setupUI()
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

    QLabel* titleLabel = new QLabel("Clubsphere");
    QFont titleFont;
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    headerLayout->addStretch(1);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch(1);

    // Toggle buttons for Joined/Pending
    QWidget* toggleWidget = new QWidget();
    toggleWidget->setFixedHeight(50);
    toggleWidget->setStyleSheet("background-color: white;");

    QHBoxLayout* toggleLayout = new QHBoxLayout(toggleWidget);
    toggleLayout->setContentsMargins(20, 5, 20, 5);
    toggleLayout->setSpacing(0);

    // Create a container widget for the toggle buttons
    QWidget* toggleContainer = new QWidget();
    toggleContainer->setFixedHeight(40);
    toggleContainer->setStyleSheet("background-color: #f0f8f0; border-radius: 20px;");

    QHBoxLayout* toggleContainerLayout = new QHBoxLayout(toggleContainer);
    toggleContainerLayout->setContentsMargins(5, 5, 5, 5);
    toggleContainerLayout->setSpacing(0);

    m_joinedButton = new QPushButton("Joined");
    m_joinedButton->setFixedSize(130, 30);
    m_joinedButton->setStyleSheet(
        "QPushButton {"
        "    background-color: white;"
        "    color: black;"
        "    border-radius: 15px;"
        "    font-weight: bold;"
        "}"
        );

    m_pendingButton = new QPushButton("Pending");
    m_pendingButton->setFixedSize(130, 30);
    m_pendingButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: black;"
        "    border-radius: 15px;"
        "    font-weight: bold;"
        "}"
        );

    toggleContainerLayout->addWidget(m_joinedButton);
    toggleContainerLayout->addWidget(m_pendingButton);

    toggleLayout->addWidget(toggleContainer);

    // Search bar
    QWidget* searchWidget = new QWidget();
    searchWidget->setFixedHeight(70);
    searchWidget->setStyleSheet("background-color: #d8e8cd;");

    QHBoxLayout* searchLayout = new QHBoxLayout(searchWidget);
    searchLayout->setContentsMargins(20, 15, 20, 15);

    // Create a search box with icon inside
    m_searchInput = new QLineEdit();
    m_searchInput->setPlaceholderText("Search");
    m_searchInput->setFixedHeight(40);
    m_searchInput->setStyleSheet(
        "QLineEdit {"
        "    background-color: #f0f0f0;"
        "    border-radius: 20px;"
        "    padding-left: 35px;"  // Add padding for the icon
        "    font-size: 14px;"
        "}"
        );

    // Add icon to search box
    QAction* searchAction = new QAction(m_searchInput);
    QPixmap searchIcon(":/images/resources/search.png");
    if (searchIcon.isNull()) {
        // Create a search icon if resource is not available
        QPixmap icon(20, 20);
        icon.fill(Qt::transparent);
        QPainter painter(&icon);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(Qt::gray, 1.5));
        painter.drawEllipse(3, 3, 10, 10);
        painter.drawLine(12, 12, 17, 17);
        searchAction->setIcon(QIcon(icon));
    } else {
        searchAction->setIcon(QIcon(searchIcon));
    }
    m_searchInput->addAction(searchAction, QLineEdit::LeadingPosition);

    searchLayout->addWidget(m_searchInput);

    // Create stacked widget for joined and pending views
    m_stackedWidget = new QStackedWidget();
    m_stackedWidget->setStyleSheet("background-color: #d8e8cd;");

    // Create Joined view
    QWidget* joinedWidget = new QWidget();
    QVBoxLayout* joinedLayout = new QVBoxLayout(joinedWidget);
    joinedLayout->setContentsMargins(0, 0, 0, 0);
    joinedLayout->setSpacing(0);

    QScrollArea* joinedScrollArea = new QScrollArea();
    joinedScrollArea->setWidgetResizable(true);
    joinedScrollArea->setFrameShape(QFrame::NoFrame);
    joinedScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    joinedScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    joinedScrollArea->setStyleSheet(
        "QScrollArea {"
        "    background-color: #d8e8cd;"
        "}"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #d8e8cd;"
        "    width: 10px;"
        "    margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #9aba85;"
        "    min-height: 20px;"
        "    border-radius: 5px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        );

    m_joinedContentWidget = new QWidget();
    m_joinedContentLayout = new QVBoxLayout(m_joinedContentWidget);
    m_joinedContentLayout->setContentsMargins(0, 0, 0, 20);
    m_joinedContentLayout->setSpacing(0);
    m_joinedContentLayout->addStretch(1);  // This will be replaced by club items

    joinedScrollArea->setWidget(m_joinedContentWidget);
    joinedLayout->addWidget(joinedScrollArea);

    // Create Pending view
    QWidget* pendingWidget = new QWidget();
    QVBoxLayout* pendingLayout = new QVBoxLayout(pendingWidget);
    pendingLayout->setContentsMargins(0, 0, 0, 0);
    pendingLayout->setSpacing(0);

    QScrollArea* pendingScrollArea = new QScrollArea();
    pendingScrollArea->setWidgetResizable(true);
    pendingScrollArea->setFrameShape(QFrame::NoFrame);
    pendingScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pendingScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pendingScrollArea->setStyleSheet(
        "QScrollArea {"
        "    background-color: #d8e8cd;"
        "}"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #d8e8cd;"
        "    width: 10px;"
        "    margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #9aba85;"
        "    min-height: 20px;"
        "    border-radius: 5px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        );

    m_pendingContentWidget = new QWidget();
    m_pendingContentLayout = new QVBoxLayout(m_pendingContentWidget);
    m_pendingContentLayout->setContentsMargins(0, 0, 0, 20);
    m_pendingContentLayout->setSpacing(0);
    m_pendingContentLayout->addStretch(1);  // This will be replaced by club items

    pendingScrollArea->setWidget(m_pendingContentWidget);
    pendingLayout->addWidget(pendingScrollArea);

    m_stackedWidget->addWidget(joinedWidget);
    m_stackedWidget->addWidget(pendingWidget);

    // Bottom navigation bar
    m_bottomNavBar = new QWidget();
    m_bottomNavBar->setFixedHeight(60);
    m_bottomNavBar->setStyleSheet("background-color: white; border-top: 1px solid #e0e0e0;");

    m_bottomNavLayout = new QHBoxLayout(m_bottomNavBar);
    m_bottomNavLayout->setContentsMargins(10, 5, 10, 5);
    m_bottomNavLayout->setSpacing(0);

    // Create navigation buttons with icons
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
    connect(homeIcon, &QPushButton::clicked, this, &MClubPage::homeClicked);
    connect(clubIcon, &QPushButton::clicked, this, &MClubPage::clubClicked);
    connect(eventIcon, &QPushButton::clicked, this, &MClubPage::eventClicked);
    connect(profileIcon, &QPushButton::clicked, this, &MClubPage::profileClicked);
    connect(m_joinedButton, &QPushButton::clicked, this, &MClubPage::showJoinedView);
    connect(m_pendingButton, &QPushButton::clicked, this, &MClubPage::showPendingView);
    connect(m_searchInput, &QLineEdit::textChanged, this, &MClubPage::filterClubs);

    // Add all to main layout
    m_mainLayout->addWidget(headerWidget);
    m_mainLayout->addWidget(toggleWidget);
    m_mainLayout->addWidget(searchWidget);
    m_mainLayout->addWidget(m_stackedWidget, 1);
    m_mainLayout->addWidget(m_bottomNavBar);

    // Set fixed width to match window size
    setFixedWidth(350);
    setFixedHeight(650);


    // Show joined view by default
    showJoinedView();
}

void MClubPage::loadUserClubs()
{
    // Clear existing club items
    clearContentLayout(m_joinedContentLayout);
    clearContentLayout(m_pendingContentLayout);

    // Load user data from database
    User currentUser = User::loadFromDatabase(m_userId);

    if (currentUser.getId() == 0) {
        qDebug() << "Failed to load user with ID:" << m_userId;
        return;
    }

    // Get joined and pending clubs
    QVector<int> joinedClubIds = currentUser.getJoinedClubs();
    QVector<int> pendingClubIds = currentUser.getPendingClubs();

    // Load and display joined clubs
    loadClubs(joinedClubIds, true);

    // Load and display pending clubs
    loadClubs(pendingClubIds, false);
}

void MClubPage::loadClubs(const QVector<int>& clubIds, bool isJoined)
{
    QSqlQuery query;

    for (int clubId : clubIds) {
        query.prepare("SELECT club_name, club_members FROM clubs_list WHERE club_id = :club_id");
        query.bindValue(":club_id", clubId);

        if (query.exec() && query.next()) {
            QString clubName = query.value(0).toString();
            QString clubMembers = query.value(1).toString();

            // Get member count
            QVector<int> memberIds = Database::deserializeUserIds(clubMembers);
            int memberCount = memberIds.size();

            // Placeholder for rank (could be calculated based on points or other metrics)
            int rank = clubId % 50 + 1; // Just a placeholder calculation

            QWidget* clubItem = createClubItem(clubId, clubName, QString::number(rank),
                                               QString::number(memberCount),
                                               !isJoined, isJoined);

            QWidget* separator = createSeparator();

            if (isJoined) {
                m_joinedContentLayout->insertWidget(m_joinedContentLayout->count() - 1, clubItem);
                m_joinedContentLayout->insertWidget(m_joinedContentLayout->count() - 1, separator);
            } else {
                m_pendingContentLayout->insertWidget(m_pendingContentLayout->count() - 1, clubItem);
                m_pendingContentLayout->insertWidget(m_pendingContentLayout->count() - 1, separator);
            }
        }
    }
}

void MClubPage::clearContentLayout(QLayout* layout)
{
    if (!layout)
        return;

    QLayoutItem* item;
    while ((item = layout->takeAt(0))) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    // Add back the stretch only for box layouts
    QBoxLayout* boxLayout = qobject_cast<QBoxLayout*>(layout);
    if (boxLayout) {
        boxLayout->addStretch(1);
    }
}

void MClubPage::filterClubs(const QString& filterText)
{
    // First make all clubs visible
    showAllClubs(m_joinedContentLayout);
    showAllClubs(m_pendingContentLayout);

    if (filterText.isEmpty()) {
        return;
    }

    // Then hide those that don't match the filter
    filterClubsByText(m_joinedContentLayout, filterText);
    filterClubsByText(m_pendingContentLayout, filterText);
}

void MClubPage::showAllClubs(QLayout* layout)
{
    if (!layout)
        return;

    for (int i = 0; i < layout->count() - 1; i++) { // Exclude the stretch at the end
        QLayoutItem* item = layout->itemAt(i);
        if (item && item->widget()) {
            item->widget()->setVisible(true);
        }
    }
}

void MClubPage::filterClubsByText(QLayout* layout, const QString& filterText)
{
    if (!layout)
        return;

    // We need to check club items which are at even indices (0, 2, 4...)
    // Separators are at odd indices
    for (int i = 0; i < layout->count() - 1; i += 2) {
        QLayoutItem* clubItem = layout->itemAt(i);
        QLayoutItem* separatorItem = (i + 1 < layout->count()) ? layout->itemAt(i + 1) : nullptr;

        if (clubItem && clubItem->widget()) {
            // Find the club name label which is a child of this widget
            QLabel* nameLabel = clubItem->widget()->findChild<QLabel*>();
            if (nameLabel) {
                bool visible = nameLabel->text().contains(filterText, Qt::CaseInsensitive);
                clubItem->widget()->setVisible(visible);
                if (separatorItem && separatorItem->widget()) {
                    separatorItem->widget()->setVisible(visible);
                }
            }
        }
    }
}

QWidget* MClubPage::createClubItem(int clubId, const QString& name, const QString& rank,
                                   const QString& members, bool isPending, bool isJoinedView)
{
    QWidget* itemWidget = new QWidget();
    itemWidget->setProperty("club_id", clubId); // Store clubId as a property

    QVBoxLayout* itemLayout = new QVBoxLayout(itemWidget);
    itemLayout->setContentsMargins(20, 20, 20, 20);
    itemLayout->setSpacing(10);

    // Create top row with club logo and info
    QWidget* topRowWidget = new QWidget();
    QHBoxLayout* topRowLayout = new QHBoxLayout(topRowWidget);
    topRowLayout->setContentsMargins(0, 0, 0, 0);
    topRowLayout->setSpacing(10);

    // Club logo
    QLabel* logoLabel = new QLabel();
    logoLabel->setFixedSize(60, 60);
    logoLabel->setStyleSheet("background-color: #1f3864; border-radius: 5px;");

    // Try to load the image, if failed, create a placeholder
    QPixmap logoPixmap(":/images/resources/club_icon.png");
    if (logoPixmap.isNull()) {
        QPixmap placeholder(60, 60);
        placeholder.fill(Qt::transparent);
        QPainter painter(&placeholder);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(QColor("#1f3864")));
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, 60, 60);
        painter.setBrush(QBrush(Qt::white));
        painter.setPen(QPen(Qt::white, 2));
        painter.drawEllipse(30, 30, 20, 20);
        painter.drawLine(20, 20, 40, 40);
        painter.drawLine(20, 40, 40, 20);
        logoLabel->setPixmap(placeholder);
    } else {
        logoLabel->setPixmap(logoPixmap.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    topRowLayout->addWidget(logoLabel);

    // Club info
    QWidget* infoWidget = new QWidget();
    QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(0, 0, 0, 0);
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

    if (!isJoinedView) {
        // Status button for Pending view
        QPushButton* statusButton = new QPushButton(isPending ? "Pending" : "Cancel");
        statusButton->setFixedSize(80, 30);
        statusButton->setStyleSheet(
            isPending ?
                "QPushButton {"
                "    background-color: #f0f0f0;"
                "    color: #43a047;"
                "    border-radius: 15px;"
                "    font-weight: bold;"
                "}"
                      :
                "QPushButton {"
                "    background-color: #f0f0f0;"
                "    color: #f44336;"
                "    border-radius: 15px;"
                "    font-weight: bold;"
                "}"
            );

        // Connect the cancel button to remove the club request
        if (!isPending) {
            connect(statusButton, &QPushButton::clicked, [this, clubId]() {
                cancelClubRequest(clubId);
            });
        }

        topRowLayout->addWidget(statusButton);
    } else {
        // Create action buttons layout for Joined view
        QWidget* actionWidget = new QWidget();
        QHBoxLayout* actionLayout = new QHBoxLayout(actionWidget);
        actionLayout->setContentsMargins(0, 0, 0, 0);
        actionLayout->setSpacing(5);

        // Calendar button with PNG from resources
        QPushButton* calendarButton = new QPushButton();
        calendarButton->setFixedSize(30, 30);
        calendarButton->setStyleSheet(
            "QPushButton {"
            "    background-color: transparent;"
            "    border: none;"
            "}"
            );

        QIcon calendarIcon(":/images/resources/event.png");
        if (calendarIcon.isNull()) {
            // Fallback to drawing if resource is not available
            QPixmap calendarPixmap(30, 30);
            calendarPixmap.fill(Qt::transparent);
            QPainter calendarPainter(&calendarPixmap);
            calendarPainter.setRenderHint(QPainter::Antialiasing);
            calendarPainter.setPen(QPen(Qt::black, 1));
            calendarPainter.drawRect(5, 5, 20, 20);
            calendarPainter.drawLine(5, 10, 25, 10);
            calendarPainter.drawLine(10, 5, 10, 10);
            calendarPainter.drawLine(20, 5, 20, 10);
            calendarButton->setIcon(QIcon(calendarPixmap));
        } else {
            calendarButton->setIcon(calendarIcon);
        }
        calendarButton->setIconSize(QSize(24, 24));

        // Connect with clubId parameter
        connect(calendarButton, &QPushButton::clicked, [this, clubId]() {
            emit navigateToEvent(clubId);
        });

        // Chat button with PNG from resources
        QPushButton* chatButton = new QPushButton();
        chatButton->setFixedSize(30, 30);
        chatButton->setStyleSheet(
            "QPushButton {"
            "    background-color: transparent;"
            "    border: none;"
            "}"
            );

        QIcon chatIcon(":/images/resources/chat.png");
        if (chatIcon.isNull()) {
            // Fallback to drawing if resource is not available
            QPixmap chatPixmap(30, 30);
            chatPixmap.fill(Qt::transparent);
            QPainter chatPainter(&chatPixmap);
            chatPainter.setRenderHint(QPainter::Antialiasing);
            chatPainter.setPen(QPen(Qt::black, 1));
            chatPainter.drawRoundedRect(5, 5, 20, 15, 5, 5);
            chatPainter.drawEllipse(10, 10, 3, 3);
            chatPainter.drawEllipse(15, 10, 3, 3);
            chatPainter.drawEllipse(20, 10, 3, 3);
            chatButton->setIcon(QIcon(chatPixmap));
        } else {
            chatButton->setIcon(chatIcon);
        }
        chatButton->setIconSize(QSize(24, 24));

        // Connect with clubId parameter
        connect(chatButton, &QPushButton::clicked, [this, clubId]() {
            emit navigateToChat(clubId);
        });

        // Leave button with PNG from resources
        QPushButton* leaveButton = new QPushButton();
        leaveButton->setFixedSize(30, 30);
        leaveButton->setStyleSheet(
            "QPushButton {"
            "    background-color: transparent;"
            "    border: none;"
            "}"
            );

        QIcon leaveIcon(":/images/resources/close.png");
        if (leaveIcon.isNull()) {
            // Fallback to drawing if resource is not available
            QPixmap leavePixmap(30, 30);
            leavePixmap.fill(Qt::transparent);
            QPainter leavePainter(&leavePixmap);
            leavePainter.setRenderHint(QPainter::Antialiasing);
            leavePainter.setPen(QPen(QColor("#f44336"), 2));
            leavePainter.drawLine(5, 5, 25, 25);
            leavePainter.drawLine(25, 5, 5, 25);
            leaveButton->setIcon(QIcon(leavePixmap));
        } else {
            leaveButton->setIcon(leaveIcon);
        }
        leaveButton->setIconSize(QSize(24, 24));

        // Connect the leave button to leave the club
        connect(leaveButton, &QPushButton::clicked, [this, clubId]() {
            leaveClub(clubId);
        });

        actionLayout->addWidget(calendarButton);
        actionLayout->addWidget(chatButton);
        actionLayout->addWidget(leaveButton);

        topRowLayout->addWidget(actionWidget);
    }

    // Add top row to main layout
    itemLayout->addWidget(topRowWidget);

    return itemWidget;
}

void MClubPage::leaveClub(int clubId)
{
    User user = User::loadFromDatabase(m_userId);
    if (user.getId() == 0) {
        qDebug() << "Failed to load user for leaving club";
        return;
    }

    user.leaveClub(clubId);
    if (user.saveToDatabase()) {
        qDebug() << "Successfully left club with ID:" << clubId;
        // Refresh the clubs view
        loadUserClubs();
    } else {
        qDebug() << "Failed to leave club with ID:" << clubId;
    }
}

void MClubPage::cancelClubRequest(int clubId)
{
    User user = User::loadFromDatabase(m_userId);
    if (user.getId() == 0) {
        qDebug() << "Failed to load user for canceling club request";
        return;
    }

    user.removeClubRequest(clubId);
    if (user.saveToDatabase()) {
        qDebug() << "Successfully canceled request for club with ID:" << clubId;
        // Refresh the clubs view
        loadUserClubs();
    } else {
        qDebug() << "Failed to cancel request for club with ID:" << clubId;
    }
}

QWidget* MClubPage::createSeparator()
{
    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setLineWidth(1);
    separator->setStyleSheet("background-color: #888888;");
    return separator;
}

void MClubPage::showJoinedView()
{
    m_joinedButton->setStyleSheet(
        "QPushButton {"
        "    background-color: white;"
        "    color: black;"
        "    border-radius: 15px;"
        "    font-weight: bold;"
        "}"
        );
    m_pendingButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: black;"
        "    border-radius: 15px;"
        "    font-weight: bold;"
        "}"
        );

    m_stackedWidget->setCurrentIndex(0);
    qDebug() << "Showing joined clubs view";
}

void MClubPage::showPendingView()
{
    m_pendingButton->setStyleSheet(
        "QPushButton {"
        "    background-color: white;"
        "    color: black;"
        "    border-radius: 15px;"
        "    font-weight: bold;"
        "}"
        );
    m_joinedButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: black;"
        "    border-radius: 15px;"
        "    font-weight: bold;"
        "}"
        );

    m_stackedWidget->setCurrentIndex(1);
    qDebug() << "Showing pending clubs view";
}

void MClubPage::homeClicked()
{
    emit navigateToHome();
}

void MClubPage::clubClicked()
{
    emit navigateToClub();
}



void MClubPage::eventClicked()
{
    emit navigateToGoing();
}

void MClubPage::profileClicked()
{
    emit navigateToProfile();
}

void MClubPage::calendarClicked()
{
    emit navigateToEvent(-1);
}

void MClubPage::chatClicked()
{
    emit navigateToChat(-1 );
}
