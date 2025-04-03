#include "searchclubs.h"
#include "database.h" // For database interaction
#include <QIcon>
#include <QFont>
#include <QPixmap>
#include <QFrame>
#include <QPainter>
#include <QSqlDatabase>
#include <QBuffer>
#include <QDateTime>

ClubCard::ClubCard(int clubId, const QString& clubName, int memberCount, const QString& leaderName,
                   const QByteArray& photoData, bool isJoined, bool isPending, QWidget* parent)
    : QFrame(parent), m_clubId(clubId), m_clubName(clubName), m_isPending(isPending), m_isJoined(isJoined)
{
    setFrameShape(QFrame::NoFrame);
    setContentsMargins(0, 0, 0, 0);

    QHBoxLayout* cardLayout = new QHBoxLayout(this);
    cardLayout->setContentsMargins(0, 8, 0, 8);

    // Club image
    m_clubImageLabel = new QLabel(this);
    QPixmap clubPhotoPixmap;

    if (!photoData.isEmpty()) {
        clubPhotoPixmap.loadFromData(photoData);
    } else {
        clubPhotoPixmap = QPixmap(":/images/resources/default_club.png");
    }

    if (clubPhotoPixmap.isNull()) {
        // Create a placeholder if image is not found
        clubPhotoPixmap = QPixmap(80, 80);
        clubPhotoPixmap.fill(Qt::lightGray);

        // Draw placeholder image icon
        QPainter painter(&clubPhotoPixmap);
        painter.setPen(Qt::darkGray);
        painter.drawRect(20, 20, 50, 40);
        painter.drawLine(20, 30, 40, 50);
        painter.drawEllipse(50, 30, 10, 10);
    }

    m_clubImageLabel->setPixmap(clubPhotoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_clubImageLabel->setFixedSize(80, 80);
    m_clubImageLabel->setStyleSheet("background-color: #E0E0E0;");

    // Club info
    QVBoxLayout* infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(3);

    // Display club name with ID
    QString nameWithId = clubName + " (ID: " + QString::number(clubId) + ")";
    m_clubNameLabel = new QLabel(nameWithId, this);
    m_clubNameLabel->setFont(QFont("Arial", 15, QFont::Bold));

    m_memberCountLabel = new QLabel(QString::number(memberCount) + " members", this);
    m_memberCountLabel->setFont(QFont("Arial", 11));

    m_leaderLabel = new QLabel("Leader: " + leaderName, this);
    m_leaderLabel->setFont(QFont("Arial", 11));

    infoLayout->addWidget(m_clubNameLabel);
    infoLayout->addWidget(m_memberCountLabel);
    infoLayout->addWidget(m_leaderLabel);

    // Join button
    m_joinButton = new QPushButton(this);
    updateButtonState(m_isJoined, m_isPending);

    connect(m_joinButton, &QPushButton::clicked, this, &ClubCard::onJoinButtonClicked);

    cardLayout->addWidget(m_clubImageLabel);
    cardLayout->addLayout(infoLayout, 1);
    cardLayout->addWidget(m_joinButton);

    setFixedWidth(330);
}

void ClubCard::updateButtonState(bool isJoined, bool isPending)
{
    m_isJoined = isJoined;
    m_isPending = isPending;

    if (isJoined) {
        m_joinButton->setText("Joined");
        m_joinButton->setStyleSheet("QPushButton { background-color: #E0E0E0; border-radius: 12px; padding: 6px 10px; color: #555555; }");
        m_joinButton->setEnabled(false);
    } else if (isPending) {
        m_joinButton->setText("Pending");
        m_joinButton->setStyleSheet("QPushButton { background-color: #D9E9D8; border-radius: 12px; padding: 6px 10px; color: #4CAF50; }");
        m_joinButton->setEnabled(true);
    } else {
        m_joinButton->setText("Join");
        m_joinButton->setStyleSheet("QPushButton { background-color: #D9E9D8; border-radius: 12px; padding: 6px 10px; color: #4CAF50; }");
        m_joinButton->setEnabled(true);
    }
}

void ClubCard::onJoinButtonClicked()
{
    emit joinClicked(m_clubId, m_isPending);
}

SearchClubs::SearchClubs(int currentUserId, QWidget *parent)
    : QWidget(parent), m_currentUserId(currentUserId)
{
    setupUI();

    // Load the user's joined and pending clubs
    m_joinedClubs = getUserJoinedClubs();
    m_pendingClubs = getUserPendingClubs();

    // Load clubs from database
    loadClubsFromDatabase();
}

SearchClubs::~SearchClubs()
{
}

void SearchClubs::setupUI()
{
    // Set background color to white and fix window size
    setStyleSheet("background-color: white;");
    setFixedSize(350, 650);

    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setAlignment(Qt::AlignTop);
    m_mainLayout->setContentsMargins(20, 30, 20, 20);
    m_mainLayout->setSpacing(15);

    // Title
    QLabel* titleLabel = new QLabel("Clubsphere", this);
    titleLabel->setFont(QFont("Arial", 16, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    m_mainLayout->addWidget(titleLabel);

    // Back button
    QPushButton* backButton = new QPushButton(this);
    backButton->setIcon(QIcon(":/images/resources/back.png"));
    backButton->setIconSize(QSize(20, 20));
    backButton->setFixedSize(40, 40);
    backButton->setStyleSheet("QPushButton { background-color: #F0F0F0; border-radius: 20px; }");
    connect(backButton, &QPushButton::clicked, this, &SearchClubs::navigateToHome);

    // Add back button to a separate layout above title
    QHBoxLayout* navLayout = new QHBoxLayout();
    navLayout->addWidget(backButton);
    navLayout->addStretch();
    m_mainLayout->insertLayout(0, navLayout);

    // Search bar
    QFrame* searchFrame = new QFrame(this);
    searchFrame->setFrameShape(QFrame::StyledPanel);
    searchFrame->setStyleSheet("QFrame { background-color: #F0F0F0; border-radius: 12px; }");

    QHBoxLayout* searchLayout = new QHBoxLayout(searchFrame);
    searchLayout->setContentsMargins(15, 8, 15, 8);

    QLabel* searchIcon = new QLabel(this);
    searchIcon->setPixmap(QIcon(":/images/resources/search_logo.png").pixmap(20, 20));

    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText("Search");
    m_searchBar->setStyleSheet("QLineEdit { border: none; background-color: transparent; }");

    searchLayout->addWidget(searchIcon);
    searchLayout->addWidget(m_searchBar);

    m_mainLayout->addWidget(searchFrame);

    // Connect search bar
    connect(m_searchBar, &QLineEdit::textChanged, this, &SearchClubs::filterClubs);

    // Clubs header
    QHBoxLayout* clubsHeaderLayout = new QHBoxLayout();
    QLabel* clubsLabel = new QLabel("Available Clubs:", this);
    clubsLabel->setFont(QFont("Arial", 18, QFont::Bold));
    clubsHeaderLayout->addWidget(clubsLabel);
    m_mainLayout->addLayout(clubsHeaderLayout);

    // Scroll area for club list
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_scrollContent = new QWidget(m_scrollArea);
    m_clubsLayout = new QVBoxLayout(m_scrollContent);
    m_clubsLayout->setAlignment(Qt::AlignTop);
    m_clubsLayout->setContentsMargins(0, 0, 0, 0);
    m_clubsLayout->setSpacing(15);

    m_scrollArea->setWidget(m_scrollContent);
    m_mainLayout->addWidget(m_scrollArea);
}

void SearchClubs::loadClubsFromDatabase()
{
    // Clear existing club cards first
    QLayoutItem* item;
    while ((item = m_clubsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    QSqlQuery query;
    query.prepare("SELECT club_id, club_name, club_members, leader_id, club_photo FROM clubs_list");

    if (!query.exec()) {
        qDebug() << "Error fetching clubs:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int clubId = query.value(0).toInt();
        QString clubName = query.value(1).toString();

        // Get member count
        QString membersData = query.value(2).toString();
        int memberCount = 0;
        if (!membersData.isEmpty()) {
            QStringList membersList = membersData.split(",");
            memberCount = membersList.size();
        }

        // Get leader name
        int leaderId = query.value(3).toInt();
        QString leaderName = Database::getStudentNameById(leaderId);
        if (leaderName.isEmpty()) {
            leaderName = "ID: " + QString::number(leaderId);
        }

        // Get club photo
        QByteArray photoData = query.value(4).toByteArray();

        // Check if user is already a member or has a pending request
        bool isJoined = m_joinedClubs.contains(clubId);
        bool isPending = m_pendingClubs.contains(clubId);

        // Add club card
        addClubCard(clubId, clubName, memberCount, leaderName, photoData, isJoined, isPending);
    }

    // If no clubs found, display a message
    if (m_clubsLayout->count() == 0) {
        QLabel* noClubsLabel = new QLabel("No clubs available", this);
        noClubsLabel->setAlignment(Qt::AlignCenter);
        m_clubsLayout->addWidget(noClubsLabel);
    }
}

void SearchClubs::addClubCard(int clubId, const QString& clubName, int memberCount,
                              const QString& leaderName, const QByteArray& photoData,
                              bool isJoined, bool isPending)
{
    ClubCard* card = new ClubCard(clubId, clubName, memberCount, leaderName, photoData, isJoined, isPending, this);
    m_clubsLayout->addWidget(card);

    // Connect join button signal
    connect(card, &ClubCard::joinClicked, this, &SearchClubs::handleJoinButtonClicked);

    // Add separator line
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: #E0E0E0;");
    line->setMaximumHeight(1);
    m_clubsLayout->addWidget(line);
}

void SearchClubs::filterClubs(const QString& text)
{
    if (text.isEmpty()) {
        // Show all clubs
        for (int i = 0; i < m_clubsLayout->count(); ++i) {
            QWidget* widget = m_clubsLayout->itemAt(i)->widget();
            widget->setVisible(true);
        }
        return;
    }

    // Filter clubs based on club name or club ID
    for (int i = 0; i < m_clubsLayout->count(); ++i) {
        QWidget* widget = m_clubsLayout->itemAt(i)->widget();
        ClubCard* card = qobject_cast<ClubCard*>(widget);

        if (card) {
            QString clubName = card->getClubName().toLower();
            QString clubId = QString::number(card->getClubId());

            bool matches = clubName.contains(text.toLower()) || clubId.contains(text);
            card->setVisible(matches);
        } else {
            // It's probably a separator line
            QFrame* line = qobject_cast<QFrame*>(widget);
            if (line) {
                // Make line visible only if previous widget is visible
                bool previousVisible = false;
                if (i > 0) {
                    QWidget* prevWidget = m_clubsLayout->itemAt(i-1)->widget();
                    previousVisible = prevWidget->isVisible();
                }
                line->setVisible(previousVisible);
            }
        }
    }
}

QVector<int> SearchClubs::getUserJoinedClubs()
{
    QVector<int> joinedClubs;

    QSqlQuery query;
    query.prepare("SELECT joined_clubs FROM users_list WHERE user_id = :userId");
    query.bindValue(":userId", m_currentUserId);

    if (query.exec() && query.next()) {
        QString joinedClubsStr = query.value(0).toString();
        joinedClubs = Database::deserializeUserIds(joinedClubsStr);
    } else {
        qDebug() << "Error fetching joined clubs:" << query.lastError().text();
    }

    return joinedClubs;
}

QVector<int> SearchClubs::getUserPendingClubs()
{
    QVector<int> pendingClubs;

    QSqlQuery query;
    query.prepare("SELECT pending_clubs FROM users_list WHERE user_id = :userId");
    query.bindValue(":userId", m_currentUserId);

    if (query.exec() && query.next()) {
        QString pendingClubsStr = query.value(0).toString();
        pendingClubs = Database::deserializeUserIds(pendingClubsStr);
    } else {
        qDebug() << "Error fetching pending clubs:" << query.lastError().text();
    }

    return pendingClubs;
}

bool SearchClubs::updateUserPendingClubs(const QVector<int>& pendingClubs)
{
    QString serializedPendingClubs = Database::serializeUserIds(pendingClubs);

    QSqlQuery query;
    query.prepare("UPDATE users_list SET pending_clubs = :pendingClubs WHERE user_id = :userId");
    query.bindValue(":pendingClubs", serializedPendingClubs);
    query.bindValue(":userId", m_currentUserId);

    if (!query.exec()) {
        qDebug() << "Error updating pending clubs:" << query.lastError().text();
        return false;
    }

    return true;
}

void SearchClubs::handleJoinButtonClicked(int clubId, bool isPending)
{
    if (isPending) {
        // User wants to cancel a pending request
        // [existing code for canceling request]
    } else {
        // User wants to join the club
        // Add club to pending list
        if (!m_pendingClubs.contains(clubId)) {
            m_pendingClubs.append(clubId);

            // Update in database
            if (updateUserPendingClubs(m_pendingClubs)) {
                // First, delete any existing notifications for this club and user
                QSqlQuery deleteExistingNotification;
                deleteExistingNotification.prepare("DELETE FROM notification_lists WHERE club_id = :clubId AND user_id = :userId ");
                deleteExistingNotification.bindValue(":clubId", clubId);
                deleteExistingNotification.bindValue(":userId", m_currentUserId);

                if (!deleteExistingNotification.exec()) {
                    qDebug() << "Error deleting existing notification:" << deleteExistingNotification.lastError().text();
                }

                // Now add the new notification
                QSqlQuery addNotification;
                addNotification.prepare("INSERT INTO notification_lists (club_id, is_request, content, user_id, timestamp) "
                                        "VALUES (:clubId, 1, 'join', :userId, :timestamp)");
                addNotification.bindValue(":clubId", clubId);
                addNotification.bindValue(":userId", m_currentUserId);
                addNotification.bindValue(":timestamp", QDateTime::currentSecsSinceEpoch());

                if (!addNotification.exec()) {
                    qDebug() << "Error adding notification:" << addNotification.lastError().text();
                }

                // Refresh club cards to show updated status
                refreshClubCards();
            } else {
                // [existing error handling code]
            }
        }
    }
}

void SearchClubs::refreshClubCards()
{
    // Simply reload clubs from database with updated status
    loadClubsFromDatabase();
}
