#include "meventpage.h"
#include <QIcon>
#include <QFont>
#include <QPixmap>
#include <QFrame>
#include<database.h>
#include<QPainter>
#include<QMessageBox>


// MEventCard implementation for MHomepage - Without delete button and join code
MEventCard::MEventCard(int eventId, const QString& clubName, const QDateTime& dateTime,
                       const QString& description, const QByteArray& imageData,
                       int goingCount, int userId, bool isAlreadyGoing, QWidget* parent)
    : QFrame(parent), m_eventId(eventId), m_userId(userId), m_isGoing(isAlreadyGoing)
{
    setFrameStyle(QFrame::StyledPanel);
    setStyleSheet("QFrame { background-color: white; border-radius: 10px; }");
    setFixedWidth(338);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    // Debug the dateTime value
    qDebug() << "DateTime for card:" << dateTime.toString("MMMM d, h:mm AP");

    // Layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    // 1. HEADER LAYOUT
    QHBoxLayout* headerLayout = new QHBoxLayout();

    // Profile image - Make it square
    m_profileImage = new QLabel(this);
    m_profileImage->setFixedSize(40, 40);
    m_profileImage->setStyleSheet("border: 1px solid lightgray; background-color: lightgray;");

    // Load club photo from database based on club name
    QSqlQuery query;
    query.prepare("SELECT club_photo FROM clubs_list WHERE club_name = :clubName");
    query.bindValue(":clubName", clubName);

    if (query.exec() && query.next()) {
        QByteArray clubPhotoData = query.value(0).toByteArray();
        if (!clubPhotoData.isEmpty()) {
            QPixmap clubPixmap;
            clubPixmap.loadFromData(clubPhotoData);
            if (!clubPixmap.isNull()) {
                // Create square profile image
                QPixmap squarePixmap = clubPixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                // Center the image in the square frame
                QPixmap centeredPixmap(40, 40);
                centeredPixmap.fill(Qt::lightGray);

                QPainter painter(&centeredPixmap);
                // Calculate position to center the image
                int x = (40 - squarePixmap.width()) / 2;
                int y = (40 - squarePixmap.height()) / 2;
                painter.drawPixmap(x, y, squarePixmap);

                m_profileImage->setPixmap(centeredPixmap);
            } else {
                // Default profile image if loading fails
                QPixmap profilePix(40, 40);
                profilePix.fill(Qt::lightGray);
                m_profileImage->setPixmap(profilePix);
            }
        } else {
            // Default profile image if no photo data
            QPixmap profilePix(40, 40);
            profilePix.fill(Qt::lightGray);
            m_profileImage->setPixmap(profilePix);
        }
    } else {
        // Default profile image if query fails
        QPixmap profilePix(40, 40);
        profilePix.fill(Qt::lightGray);
        m_profileImage->setPixmap(profilePix);
    }

    // Club name and date
    QVBoxLayout* headerInfoLayout = new QVBoxLayout();
    m_clubNameLabel = new QLabel(clubName, this);
    m_clubNameLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    // Modified timestamp label with improved visibility
    m_dateTimeLabel = new QLabel(dateTime.toString("MMMM d, h:mm AP"), this);
    m_dateTimeLabel->setStyleSheet("color: #505050; font-size: 12px;");

    headerInfoLayout->addWidget(m_clubNameLabel);
    headerInfoLayout->addWidget(m_dateTimeLabel);
    headerInfoLayout->setSpacing(2);

    headerLayout->addWidget(m_profileImage);
    headerLayout->addLayout(headerInfoLayout, 1);
    headerLayout->addStretch();

    // 2. CONTENT (DESCRIPTION)
    m_descriptionLabel = new QLabel(description, this);
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->setStyleSheet("font-size: 13px; margin: 5px 0px;");

    // 3. EVENT IMAGE (only created if there's image data)
    m_eventImageLabel = nullptr; // Initialize to nullptr
    if (!imageData.isEmpty()) {
        QPixmap pixmap;
        if (pixmap.loadFromData(imageData)) {
            m_eventImageLabel = new QLabel(this);
            m_eventImageLabel->setFixedHeight(280);
            m_eventImageLabel->setAlignment(Qt::AlignCenter);
            m_eventImageLabel->setStyleSheet("background-color: #f5f5f5; border-radius: 5px;");
            m_eventImageLabel->setPixmap(pixmap.scaled(335, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }

    // 4. FOOTER LAYOUT
    QHBoxLayout* footerLayout = new QHBoxLayout();

    // Going button
    m_goingButton = new QPushButton(m_isGoing ? "Going ✓" : "Going", this);
    m_goingButton->setStyleSheet(m_isGoing ?
                                     "QPushButton { border: 1px solid #ddd; border-radius: 15px; padding: 5px 10px; background-color: #e1f5fe; }" :
                                     "QPushButton { border: 1px solid #ddd; border-radius: 15px; padding: 5px 10px; }");

    // Going count
    m_goingCountLabel = new QLabel(QString::number(goingCount), this);
    m_goingCountLabel->setStyleSheet("font-weight: bold;");

    footerLayout->addWidget(m_goingButton);
    footerLayout->addWidget(m_goingCountLabel);
    footerLayout->addStretch();

    // Add everything to main layout in the correct order:
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(m_descriptionLabel);
    if (m_eventImageLabel) {
        mainLayout->addWidget(m_eventImageLabel);
    }
    mainLayout->addLayout(footerLayout);

    // Connect going button
    connect(m_goingButton, &QPushButton::clicked, this, &MEventCard::onGoingClicked);
}

void MEventCard::onGoingClicked()
{
    // Toggle the going status
    m_isGoing = !m_isGoing;

    // If now going, add event to user's going events
    QSqlQuery query;

    // First, get current going_events for the user
    query.prepare("SELECT going_events FROM users_list WHERE user_id = :userId");
    query.bindValue(":userId", m_userId);

    if (query.exec() && query.next()) {
        QString goingEvents = query.value(0).toString();
        QVector<int> goingEventsVector = Database::deserializeUserIds(goingEvents);

        if (m_isGoing) {
            // Add event to going events if not already there
            if (!goingEventsVector.contains(m_eventId)) {
                goingEventsVector.append(m_eventId);

                // Update going_events for user
                QString serializedGoingEvents = Database::serializeUserIds(goingEventsVector);
                query.prepare("UPDATE users_list SET going_events = :goingEvents WHERE user_id = :userId");
                query.bindValue(":goingEvents", serializedGoingEvents);
                query.bindValue(":userId", m_userId);

                if (!query.exec()) {
                    qDebug() << "Error updating user's going events:" << query.lastError().text();
                    return;
                }

                // Increment going count for event
                query.prepare("UPDATE events_list SET event_going_count = event_going_count + 1 WHERE event_id = :eventId");
                query.bindValue(":eventId", m_eventId);

                if (!query.exec()) {
                    qDebug() << "Error updating event going count:" << query.lastError().text();
                    return;
                }

                // Update button and label
                m_goingButton->setText("Going ✓");
                m_goingButton->setStyleSheet("QPushButton { border: 1px solid #ddd; border-radius: 15px; padding: 5px 10px; background-color: #e1f5fe; }");
                int goingCount = m_goingCountLabel->text().toInt() + 1;
                m_goingCountLabel->setText(QString::number(goingCount));

                QMessageBox::information(this, "Success", "You're now going to this event!");
            }
        } else {
            // Remove event from going events
            if (goingEventsVector.contains(m_eventId)) {
                goingEventsVector.removeAll(m_eventId);

                // Update going_events for user
                QString serializedGoingEvents = Database::serializeUserIds(goingEventsVector);
                query.prepare("UPDATE users_list SET going_events = :goingEvents WHERE user_id = :userId");
                query.bindValue(":goingEvents", serializedGoingEvents);
                query.bindValue(":userId", m_userId);

                if (!query.exec()) {
                    qDebug() << "Error updating user's going events:" << query.lastError().text();
                    return;
                }

                // Decrement going count for event
                query.prepare("UPDATE events_list SET event_going_count = GREATEST(event_going_count - 1, 0) WHERE event_id = :eventId");
                query.bindValue(":eventId", m_eventId);

                if (!query.exec()) {
                    qDebug() << "Error updating event going count:" << query.lastError().text();
                    return;
                }

                // Update button and label
                m_goingButton->setText("Going");
                m_goingButton->setStyleSheet("QPushButton { border: 1px solid #ddd; border-radius: 15px; padding: 5px 10px; }");
                int goingCount = m_goingCountLabel->text().toInt() - 1;
                if (goingCount < 0) goingCount = 0;
                m_goingCountLabel->setText(QString::number(goingCount));

                QMessageBox::information(this, "Updated", "You're no longer going to this event.");
            }
        }
    }
}


MEventPage::MEventPage(int clubId,int userId, QWidget *parent)
    : QWidget(parent), m_clubId(clubId), user_id(userId)
{
    setupUI();
    MEventPage::loadClubEvents();
}


MEventPage::~MEventPage()
{
}

void MEventPage::setupUI()
{
    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Header with back button
    QWidget* headerWidget = new QWidget();
    headerWidget->setFixedHeight(50);
    headerWidget->setStyleSheet("background-color: white;");

    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(12, 4, 12, 4);

    // Create back button with image instead of text
    QPushButton* backBtn = new QPushButton();

    // Load back arrow icon from resources
    QIcon backIcon(":/images/resources/back.png");

    // Check if icon loaded successfully
    if (backIcon.isNull()) {
        // Fallback to a secondary path
        backIcon = QIcon(":/icons/navigation/back");

        // If still null, create a warning in console
        if (backIcon.isNull()) {
            qWarning("Back button icon not found in resources!");
        }
    }

    // Set the icon to the button
    backBtn->setIcon(backIcon);
    backBtn->setIconSize(QSize(18, 18));

    // Style the button to be circular
    backBtn->setStyleSheet("QPushButton {"
                           "background-color: #f0f0f0;"
                           "border: none;"
                           "border-radius: 15px;"
                           "padding: 5px;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: #e0e0e0;"
                           "}");
    backBtn->setFixedSize(30, 30);

    QLabel* appTitle = new QLabel("Clubsphere");
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    appTitle->setFont(titleFont);

    headerLayout->addWidget(backBtn);
    headerLayout->addWidget(appTitle, 1, Qt::AlignCenter);
    headerLayout->addSpacing(backBtn->width()); // Balance the header

    // Title section with Event label
    QWidget* titleWidget = new QWidget();
    titleWidget->setFixedHeight(50);
    titleWidget->setStyleSheet("background-color: white;");

    QHBoxLayout* titleLayout = new QHBoxLayout(titleWidget);
    titleLayout->setContentsMargins(12, 4, 12, 4);

    m_titleLabel = new QLabel("Science Club's announcement");
    QFont eventsFont;
    eventsFont.setPointSize(18);
    eventsFont.setBold(true);
    m_titleLabel->setFont(eventsFont);

    titleLayout->addWidget(m_titleLabel);
    titleLayout->addStretch(1);

    // Line separator
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Plain);
    line->setFixedHeight(1);
    line->setStyleSheet("background-color: #e0e0e0;");

    // Scroll area for events
    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setStyleSheet("background-color: white;");

    m_scrollContent = new QWidget();
    m_scrollContent->setStyleSheet("background-color: white;");

    m_eventsLayout = new QVBoxLayout(m_scrollContent);
    m_eventsLayout->setContentsMargins(10, 10, 10, 10);
    m_eventsLayout->setSpacing(10);

    m_scrollArea->setWidget(m_scrollContent);

    // Connect back button signal
    connect(backBtn, &QPushButton::clicked, this, &MEventPage::navigateToClub);

    // Add all to main layout
    m_mainLayout->addWidget(headerWidget);
    m_mainLayout->addWidget(titleWidget);
    m_mainLayout->addWidget(line);
    m_mainLayout->addWidget(m_scrollArea, 1);

    // Set fixed width to match window size
    setFixedWidth(350);

    // Set background color
    setStyleSheet("background-color: white;");
}


void MEventPage::loadClubEvents()
{
    // Clear existing events first if any
    QLayoutItem* item;
    while ((item = m_eventsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    // Load events for this club from the database
    QSqlQuery query;
    query.prepare("SELECT event_id, event_content, event_photo, created_timestamp, event_going_count "
                  "FROM events_list "
                  "WHERE club_id = :clubId "
                  "ORDER BY created_timestamp DESC");
    query.bindValue(":clubId", m_clubId);

    // Get club name for the title
    QSqlQuery clubQuery;
    clubQuery.prepare("SELECT club_name FROM clubs_list WHERE club_id = :clubId");
    clubQuery.bindValue(":clubId", m_clubId);

    if (clubQuery.exec() && clubQuery.next()) {
        QString clubName = clubQuery.value(0).toString();
        m_titleLabel->setText(clubName + "'s events");
    }

    if (query.exec()) {
        bool hasEvents = false;
        int userId = user_id; // Assume this method exists or add it

        while (query.next()) {
            hasEvents = true;

            int eventId = query.value(0).toInt();
            QString content = query.value(1).toString();
            QByteArray imageData = query.value(2).toByteArray();
            QDateTime timestamp = QDateTime::fromSecsSinceEpoch(query.value(3).toLongLong());
            int goingCount = query.value(4).toInt();

            // Check if user is already going to this event
            bool isAlreadyGoing = false;
            QSqlQuery userQuery;
            userQuery.prepare("SELECT going_events FROM users_list WHERE user_id = :userId");
            userQuery.bindValue(":userId", userId);

            if (userQuery.exec() && userQuery.next()) {
                QString goingEvents = userQuery.value(0).toString();
                QVector<int> goingEventsVector = Database::deserializeUserIds(goingEvents);
                isAlreadyGoing = goingEventsVector.contains(eventId);
            }

            // Get club name for this event
            QSqlQuery clubNameQuery;
            clubNameQuery.prepare("SELECT club_name FROM clubs_list WHERE club_id = :clubId");
            clubNameQuery.bindValue(":clubId", m_clubId);
            QString clubName = "Unknown Club";

            if (clubNameQuery.exec() && clubNameQuery.next()) {
                clubName = clubNameQuery.value(0).toString();
            }

            // Add separator except for the first event
            if (m_eventsLayout->count() > 0) {
                QFrame* line = new QFrame();
                line->setFrameShape(QFrame::HLine);
                line->setFrameShadow(QFrame::Plain);
                line->setFixedHeight(1);
                line->setStyleSheet("background-color: #e0e0e0;");
                m_eventsLayout->addWidget(line);
            }

            // Create MEventCard with the loaded data
            MEventCard* card = new MEventCard(eventId, clubName, timestamp, content,
                                              imageData, goingCount, userId, isAlreadyGoing, this);
            m_eventsLayout->addWidget(card);
        }

        if (!hasEvents) {
            QLabel* noEventsLabel = new QLabel("No events found for this club", this);
            noEventsLabel->setAlignment(Qt::AlignCenter);
            noEventsLabel->setStyleSheet("font-size: 14px; color: #505050; margin: 20px;");
            m_eventsLayout->addWidget(noEventsLabel);
        }
    } else {
        qDebug() << "Error loading events:" << query.lastError().text();
    }
}

