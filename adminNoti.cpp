#include "adminNoti.h"
#include <QPixmap>
#include <QIcon>
#include <QDebug>
#include <QScrollBar>

AdminNoti::AdminNoti(QWidget *parent) : QWidget(parent)
{
    setupUI();
    setupNotificationsArea();
    setupNavigation();
}

AdminNoti::~AdminNoti()
{
}

void AdminNoti::setupUI()
{
    // Set background color to white and fix window size
    this->setStyleSheet("background-color: white;");
    this->setFixedSize(350, 650);

    // Main layout setup
    mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setContentsMargins(3, 30, 3, 20);  // Remove horizontal margins
    mainLayout->setSpacing(15);

    // Title
    titleLabel = new QLabel("Clubsphere", this);
    titleLabel->setFont(QFont("Arial", 16, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(titleLabel);
}

void AdminNoti::setupNotificationsArea()
{
    // Create the scroll area for notifications
    notificationsScrollArea = new QScrollArea(this);
    notificationsScrollArea->setWidgetResizable(true);
    notificationsScrollArea->setFrameShape(QFrame::NoFrame);
    notificationsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    notificationsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set minimum height to use more vertical space
    notificationsScrollArea->setMinimumHeight(480); // Increase this value to use more space

    // Create container widget for notifications
    notificationsContainer = new QWidget(notificationsScrollArea);
    notificationsContainer->setStyleSheet("background-color: #D9E9D8; border-radius: 12px;");


    // Create layout for notifications
    notificationsLayout = new QVBoxLayout(notificationsContainer);
    notificationsLayout->setContentsMargins(15, 15, 15, 15);
    notificationsLayout->setSpacing(10);  // Reduced spacing between items

    // Add a title to the notifications section
    QLabel *suspensionLabel = new QLabel("Suspension Request", notificationsContainer);
    QFont suspensionFont;
    suspensionFont.setBold(true);
    suspensionFont.setPointSize(14);
    suspensionLabel->setFont(suspensionFont);
    suspensionLabel->setAlignment(Qt::AlignCenter);
    notificationsLayout->addWidget(suspensionLabel);

    // Add suspension requests
    for (int i = 0; i < 15; i++) {
        createSuspensionRequest("Jenny", "John", "Vandalism of club property");

        // Add separator line except after the last item
        if (i < 4) {
            QFrame *line = new QFrame(notificationsContainer);
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            line->setStyleSheet("background-color: #888888;");
            line->setFixedHeight(1);
            notificationsLayout->addWidget(line);
        }
    }

    // Set the container as the widget for the scroll area
    notificationsScrollArea->setWidget(notificationsContainer);

    // Add scroll area to main layout with proper margins
    QHBoxLayout *scrollAreaLayout = new QHBoxLayout();
    scrollAreaLayout->setContentsMargins(10, 0, 10, 0);
    scrollAreaLayout->addWidget(notificationsScrollArea);
    mainLayout->addLayout(scrollAreaLayout);

    // Add spacer to push navigation bar to bottom
    mainLayout->addStretch(1);
}

void AdminNoti::createSuspensionRequest(const QString &username, const QString &reportedBy, const QString &reason)
{
    // User info layout
    QHBoxLayout *requestLayout = new QHBoxLayout();
    requestLayout->setContentsMargins(5, 10, 5, 10);  // Add padding around items

    // User icon
    QLabel *userIcon = new QLabel(this);
    userIcon->setFixedSize(40, 40);  // Slightly larger icon to match design
    userIcon->setStyleSheet("background-color: #FFFFFF; border-radius: 20px;");

    // User details
    QVBoxLayout *userDetailsLayout = new QVBoxLayout();
    userDetailsLayout->setSpacing(3);  // Tighter spacing between text lines

    QLabel *usernameLabel = new QLabel(username, this);
    QFont usernameFont;
    usernameFont.setBold(true);
    usernameFont.setPointSize(12);  // Slightly larger font
    usernameLabel->setFont(usernameFont);

    QLabel *reportLabel = new QLabel(QString("was reported by %1").arg(reportedBy), this);
    reportLabel->setStyleSheet("color: #555555;");
    QFont reportFont;
    reportFont.setPointSize(10);
    reportLabel->setFont(reportFont);

    QLabel *reasonLabel = new QLabel(QString("Reason: %1").arg(reason), this);
    reasonLabel->setStyleSheet("color: #333333;");
    QFont reasonFont;
    reasonFont.setPointSize(10);
    reasonLabel->setFont(reasonFont);

    userDetailsLayout->addWidget(usernameLabel);
    userDetailsLayout->addWidget(reportLabel);
    userDetailsLayout->addWidget(reasonLabel);

    // Action buttons
    QVBoxLayout *actionButtonsLayout = new QVBoxLayout();
    actionButtonsLayout->setSpacing(6);  // Space between buttons

    // Approve button (checkmark)
    QPushButton *approveButton = new QPushButton(this);
    approveButton->setFixedSize(32, 32);  // Slightly larger buttons
    approveButton->setIcon(QIcon(":/images/resources/checkmark.png"));
    approveButton->setIconSize(QSize(18, 18));
    approveButton->setStyleSheet("QPushButton { background-color: white; border-radius: 16px; border: 1px solid #CCCCCC; }");

    // Reject button (X)
    QPushButton *rejectButton = new QPushButton(this);
    rejectButton->setFixedSize(32, 32);  // Match approve button size
    rejectButton->setIcon(QIcon(":/images/resources/x_mark.png"));
    rejectButton->setIconSize(QSize(18, 18));
    rejectButton->setStyleSheet("QPushButton { background-color: white; border-radius: 16px; border: 1px solid #CCCCCC; }");

    // Fallback if images are not found
    if (approveButton->icon().isNull()) {
        approveButton->setText("✓");
    }
    if (rejectButton->icon().isNull()) {
        rejectButton->setText("✗");
    }

    actionButtonsLayout->addWidget(approveButton);
    actionButtonsLayout->addWidget(rejectButton);

    // Add all elements to the request layout
    requestLayout->addWidget(userIcon);
    requestLayout->addLayout(userDetailsLayout, 1);
    requestLayout->addLayout(actionButtonsLayout);
    requestLayout->setSpacing(10);  // Spacing between icon, text, and buttons

    // Add the request to the notifications layout
    notificationsLayout->addLayout(requestLayout);
}

void AdminNoti::setupNavigation()
{
    // Bottom navigation bar
    navigationFrame = new QFrame(this);
    navigationFrame->setFrameShape(QFrame::NoFrame);
    navigationFrame->setLineWidth(0);
    navigationFrame->setStyleSheet("QFrame { background-color: white; }");


    QHBoxLayout *navLayout = new QHBoxLayout(navigationFrame);
    navLayout->setContentsMargins(15, 10, 15, 10);
    navLayout->setSpacing(53);  // Keep the spacing consistent with adminHome

    // Create navigation icons
    homeButton = new QPushButton("", this);
    homeButton->setIcon(QIcon(":/images/resources/home_logo.png"));
    homeButton->setIconSize(QSize(20, 20));
    homeButton->setStyleSheet("QPushButton { border: none; }");

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
    // Match the style used in adminHome.cpp exactly
    notificationsButton->setStyleSheet("QPushButton { border: none; background-color: #D9E9D8; border-radius: 10px; padding: 5px; }");

    navLayout->addStretch(1);
    navLayout->addWidget(homeButton);
    navLayout->addWidget(profileButton);
    navLayout->addWidget(groupsButton);
    navLayout->addWidget(notificationsButton);
    navLayout->addStretch(1);

    mainLayout->addWidget(navigationFrame);

    // Connect navigation buttons
    connect(homeButton, &QPushButton::clicked, this, &AdminNoti::onHomeButtonClicked);
    connect(profileButton, &QPushButton::clicked, this, &AdminNoti::onProfileButtonClicked);
    connect(groupsButton, &QPushButton::clicked, this, &AdminNoti::onGroupsButtonClicked);
    connect(notificationsButton, &QPushButton::clicked, this, &AdminNoti::onNotificationsButtonClicked);
}
void AdminNoti::onHomeButtonClicked()
{
    qDebug() << "Home button clicked in AdminNoti";
    emit navigateToHome();
}

void AdminNoti::onProfileButtonClicked()
{
    qDebug() << "Profile button clicked in AdminNoti";
    emit navigateToMembers();
}

void AdminNoti::onGroupsButtonClicked()
{
    qDebug() << "Groups button clicked in AdminNoti";
    emit navigateToClubs();
}

void AdminNoti::onNotificationsButtonClicked()
{
    qDebug() << "Notifications button clicked in AdminNoti - already on this page";
    // No navigation needed as we're already on this page
}

QFrame* AdminNoti::createRoundedFrame()
{
    QFrame* frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setLineWidth(0);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    return frame;
}
