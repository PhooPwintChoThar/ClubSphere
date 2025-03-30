#include "lhomepage.h"
#include <QIcon>
#include <QPixmap>
#include <QFont>
#include <QSize>

AnnouncementCard::AnnouncementCard(const QString& clubName, const QDateTime& dateTime,
                                   const QString& description, const QString& imagePath,
                                   int goingCount, int notGoingCount, QWidget* parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::StyledPanel);
    setStyleSheet("QFrame { background-color: white; border-radius: 10px; }");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    // Header with profile, club name and date
    QHBoxLayout* headerLayout = new QHBoxLayout();

    m_profileImage = new QLabel();
    QPixmap profilePix(":/resources/profile_placeholder.png"); //?????????????????????????????????????
    if (profilePix.isNull()) {
        profilePix = QPixmap(40, 40);
        profilePix.fill(Qt::lightGray);
    }
    profilePix = profilePix.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_profileImage->setPixmap(profilePix);
    m_profileImage->setFixedSize(40, 40);
    m_profileImage->setStyleSheet("border-radius: 20px; background-color: lightgray;");

    QVBoxLayout* nameTimeLayout = new QVBoxLayout();
    m_clubNameLabel = new QLabel(clubName);
    m_clubNameLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    m_dateTimeLabel = new QLabel(dateTime.toString("MMMM d, h:mm AP"));
    m_dateTimeLabel->setStyleSheet("color: gray; font-size: 12px;");

    nameTimeLayout->addWidget(m_clubNameLabel);
    nameTimeLayout->addWidget(m_dateTimeLabel);
    nameTimeLayout->setSpacing(2);

    headerLayout->addWidget(m_profileImage);
    headerLayout->addLayout(nameTimeLayout);
    headerLayout->addStretch();

    // Description
    m_descriptionLabel = new QLabel(description);
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->setStyleSheet("font-size: 13px; margin: 5px 0px;");

    // Event Image
    m_eventImageLabel = new QLabel();
    QPixmap eventPix(imagePath);
    if (!eventPix.isNull()) {
        m_eventImageLabel->setPixmap(eventPix.scaled(335, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_eventImageLabel->setFixedHeight(280);
        m_eventImageLabel->setAlignment(Qt::AlignCenter);
        m_eventImageLabel->setStyleSheet("background-color: #f5f5f5; border-radius: 5px;");
    }

    // Going / Not Going section
    QHBoxLayout* attendanceLayout = new QHBoxLayout();

    m_goingButton = new QPushButton();
    m_goingButton->setIcon(QIcon(":/resources/check.png")); //????????????????????
    m_goingButton->setText("Going");
    m_goingButton->setStyleSheet("QPushButton { border: 1px solid #ddd; border-radius: 15px; padding: 5px 10px; }");

    m_notGoingButton = new QPushButton();
    m_notGoingButton->setIcon(QIcon(":/resources/close.png")); //????????????????
    m_notGoingButton->setText("Not Going");
    m_notGoingButton->setStyleSheet("QPushButton { border: 1px solid #ddd; border-radius: 15px; padding: 5px 10px; }");

    m_goingCountLabel = new QLabel(QString::number(goingCount));
    m_goingCountLabel->setStyleSheet("font-weight: bold;");

    m_notGoingCountLabel = new QLabel(QString::number(notGoingCount));
    m_notGoingCountLabel->setStyleSheet("font-weight: bold;");

    attendanceLayout->addWidget(m_goingButton);
    attendanceLayout->addWidget(m_goingCountLabel);
    attendanceLayout->addStretch();
    attendanceLayout->addWidget(m_notGoingButton);
    attendanceLayout->addWidget(m_notGoingCountLabel);

    // Add all sections to main layout
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(m_descriptionLabel);
    mainLayout->addWidget(m_eventImageLabel);
    mainLayout->addLayout(attendanceLayout);

    setFixedWidth(338);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
}

LHomepage::LHomepage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    createMockAnnouncements();
}

LHomepage::~LHomepage()
{
}

void LHomepage::setupUI()
{
    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Header section with title and status bar
    QWidget* headerWidget = new QWidget();
    headerWidget->setStyleSheet("background-color: white;");
    QVBoxLayout* headerLayout = new QVBoxLayout(headerWidget);

    // // Status bar with time and icons
    // QHBoxLayout* statusBarLayout = new QHBoxLayout();
    // QLabel* timeLabel = new QLabel("9:41");
    // timeLabel->setStyleSheet("font-weight: bold;");
    // statusBarLayout->addWidget(timeLabel);
    // statusBarLayout->addStretch();

    // // Signal strength, WiFi, and battery icons
    // QLabel* signalIcon = new QLabel();
    // signalIcon->setPixmap(QPixmap(":/resources/signal.png").scaled(20, 20, Qt::KeepAspectRatio));

    // QLabel* wifiIcon = new QLabel();
    // wifiIcon->setPixmap(QPixmap(":/resources/wifi.png").scaled(20, 20, Qt::KeepAspectRatio));

    // QLabel* batteryIcon = new QLabel();
    // batteryIcon->setPixmap(QPixmap(":/resources/battery.png").scaled(25, 20, Qt::KeepAspectRatio));

    // statusBarLayout->addWidget(signalIcon);
    // statusBarLayout->addWidget(wifiIcon);
    // statusBarLayout->addWidget(batteryIcon);

    // Title and header icons
    QHBoxLayout* titleLayout = new QHBoxLayout();
    m_titleLabel = new QLabel("Announcement");
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    titleLayout->addWidget(m_titleLabel);
    titleLayout->addStretch();

    // Header icons (achievement, chat, notification)
    m_headerIconsLayout = new QHBoxLayout();

    QPushButton* achievementButton = new QPushButton();
    achievementButton->setIcon(QIcon(":/resources/medal.png"));
    achievementButton->setIconSize(QSize(30, 30));
    achievementButton->setFixedSize(45, 45);
    achievementButton->setStyleSheet("QPushButton { background-color: #e0e0e0; border-radius: 22px; }");
    connect(achievementButton, &QPushButton::clicked, this, &LHomepage::onAchievementButtonClicked);

    QPushButton* chatButton = new QPushButton();
    chatButton->setIcon(QIcon(":/resources/chat.png"));
    chatButton->setIconSize(QSize(30, 30));
    chatButton->setFixedSize(45, 45);
    chatButton->setStyleSheet("QPushButton { background-color: #e0e0e0; border-radius: 22px; }");
    connect(chatButton, &QPushButton::clicked, this, &LHomepage::onChatButtonClicked);


    QPushButton* notificationButton = new QPushButton();
    notificationButton->setIcon(QIcon(":/resources/noti_logo.png"));
    notificationButton->setIconSize(QSize(30, 30));
    notificationButton->setFixedSize(45, 45);
    notificationButton->setStyleSheet("QPushButton { background-color: #e0e0e0; border-radius: 22px; }");
    connect(notificationButton, &QPushButton::clicked, this, &LHomepage::onNotificationButtonClicked);

    m_headerIconsLayout->addWidget(achievementButton);
    m_headerIconsLayout->addWidget(chatButton);
    m_headerIconsLayout->addWidget(notificationButton);
    m_headerIconsLayout->setSpacing(10);

    titleLayout->addLayout(m_headerIconsLayout);

    // Create new announcement section
    m_createNewLayout = new QHBoxLayout();
    m_createNewButton = new QPushButton();
    m_createNewButton->setIcon(QIcon(":/resources/plus_logo.png")); //??????????????????????
    m_createNewButton->setIconSize(QSize(20, 20));
    m_createNewButton->setFixedSize(45, 45);
    m_createNewButton->setStyleSheet("QPushButton { background-color: #e0e0e0; border-radius: 22px; }");
    connect(m_createNewButton, &QPushButton::clicked, this, &LHomepage::createNewAnnouncement);

    QLabel* createNewLabel = new QLabel("Create new announcement");
    createNewLabel->setStyleSheet("color: #505050; font-size: 14px;");

    m_createNewLayout->addWidget(m_createNewButton);
    m_createNewLayout->addWidget(createNewLabel);
    m_createNewLayout->addStretch();

    // Add sections to header layout
    // headerLayout->addLayout(statusBarLayout);
    headerLayout->addLayout(titleLayout);
    headerLayout->addSpacing(15);
    headerLayout->addLayout(m_createNewLayout);

    // Scroll area for announcements
    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setStyleSheet("background-color: #f0f0f0;");

    m_scrollContent = new QWidget();
    m_scrollContent->setStyleSheet("background-color: #f0f0f0;");

    m_announcementsLayout = new QVBoxLayout(m_scrollContent);
    m_announcementsLayout->setSpacing(15);
    m_announcementsLayout->setContentsMargins(10, 10, 10, 10);

    m_scrollArea->setWidget(m_scrollContent);

    // Add all to main layout
    m_mainLayout->addWidget(headerWidget);
    m_mainLayout->addWidget(m_scrollArea, 1);

    // Set fixed width to match mobile viewport
    setFixedWidth(375);
    setMinimumHeight(800);
}

void LHomepage::createMockAnnouncements()
{
    // First announcement
    addAnnouncementCard(
        "KMITL Science Club",
        QDateTime(QDate(2024, 7, 26), QTime(16, 0)),
        "Join us for an exciting journey into the world of science! Discover fascinating facts, engage in interactive experiments, and explore groundbreaking discoveries that shape our future.",
        ":/resources/science_fair.jpg",
        50,
        12
        );

    // Second announcement
    addAnnouncementCard(
        "KMITL Science Club",
        QDateTime(QDate(2024, 7, 20), QTime(17, 0)),
        "Step into the future of science and technology! Join us for an eye-opening event where we explore cutting-edge innovations, groundbreaking research, and the latest trends shaping our world.",
        ":/resources/science_future.jpg",
        45,
        8
        );

    // Add more mock announcements for scrolling
    for (int i = 0; i < 3; i++) {
        addAnnouncementCard(
            "KMITL Tech Workshop",
            QDateTime(QDate(2024, 8, 5 + i), QTime(13, 0)),
            "Learn practical skills with our hands-on tech workshop series. This session focuses on building your first IoT device. All materials provided!",
            ":/resources/dance.jpg",
            30 + i,
            5 + i
            );
    }
}

void LHomepage::addAnnouncementCard(const QString& clubName, const QDateTime& dateTime,
                                    const QString& description, const QString& imagePath,
                                    int goingCount, int notGoingCount)
{
    AnnouncementCard* card = new AnnouncementCard(clubName, dateTime, description, imagePath,
                                                  goingCount, notGoingCount);

    // Add a separator line except for the first card
    if (m_announcementsLayout->count() > 0) {
        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setStyleSheet("background-color: #d0d0d0;");
        line->setFixedHeight(1);
        m_announcementsLayout->addWidget(line);
    }

    m_announcementsLayout->addWidget(card);
}

void LHomepage::createNewAnnouncement()
{
    // This would connect to functionality to create a new announcement
    // For now it's just a placeholder
}

// Add this new method to LHomepage.cpp
void LHomepage::onChatButtonClicked()
{
    emit showGroupChat();
}

// In lhomepage.cpp - add these methods
void LHomepage::onNotificationButtonClicked()
{
    emit showNotifications();
}

void LHomepage::onAchievementButtonClicked()
{
    emit showLeaderboard();
}
