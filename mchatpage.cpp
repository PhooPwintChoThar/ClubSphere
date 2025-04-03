#include "mchatpage.h"
#include "database.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QTimer>
#include <QDebug>
#include <QTimer>
#include <QResizeEvent>

MBubbleMessage::MBubbleMessage(const QString &text, const QString &sender, const QDateTime &time, MessageType type, QWidget *parent)
    : QWidget(parent), messageType(type)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create message container with horizontal layout for avatar support
    QHBoxLayout *containerLayout = new QHBoxLayout();
    containerLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *bubbleWidget = new QWidget(this);
    QVBoxLayout *bubbleLayout = new QVBoxLayout(bubbleWidget);
    bubbleLayout->setSpacing(5);
    bubbleLayout->setContentsMargins(10, 10, 10, 10);

    // Set bubble style and alignment based on message type
    if (type == Incoming) {
        bubbleWidget->setStyleSheet("background-color: #E8FDD8; border-radius: 10px;");

        // Add avatar for incoming messages
        QLabel *avatarLabel = new QLabel(this);
        avatarLabel->setFixedSize(32, 32);
        avatarLabel->setStyleSheet("background-color: #E0E0E0; border-radius: 16px;");

        // Get the first letter of sender and set it as avatar text if sender is provided
        if (!sender.isEmpty()) {
            QPainter painter(new QPixmap(32, 32));
            QPixmap avatar = createAvatar(sender.left(1), "#E0E0E0");
            avatarLabel->setPixmap(avatar);

            // Add sender name
            senderLabel = new QLabel(sender, this);
            senderLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #000000;");
            bubbleLayout->addWidget(senderLabel);
        }

        containerLayout->addWidget(avatarLabel);
        containerLayout->addWidget(bubbleWidget);
        containerLayout->addStretch(1); // Keep message on the left

    } else {
        bubbleWidget->setStyleSheet("background-color: #E8FDD8; border-radius: 10px;");
        containerLayout->addStretch(1); // Push message to the right
        containerLayout->addWidget(bubbleWidget);
        senderLabel = nullptr;
    }

    // Message text
    messageLabel = new QLabel(text, this);
    messageLabel->setWordWrap(true);
    messageLabel->setStyleSheet("font-size: 14px;");

    // Calculate appropriate width to match the design
    QFont font = messageLabel->font();
    QFontMetrics metrics(font);
    int textWidth = metrics.horizontalAdvance(text);
    int maxWidth = 220;  // Maximum width for the message bubble

    if (textWidth > maxWidth) {
        messageLabel->setFixedWidth(maxWidth);
    }

    bubbleLayout->addWidget(messageLabel);

    // Time label
    timeLabel = new QLabel(time.toString("h:mm AP"), this);
    timeLabel->setStyleSheet("font-size: 12px; color: gray;");
    timeLabel->setAlignment(Qt::AlignRight);
    bubbleLayout->addWidget(timeLabel);

    mainLayout->addLayout(containerLayout);
    setLayout(mainLayout);
}

QPixmap MBubbleMessage::createAvatar(const QString &letter, const QString &color)
{
    QPixmap pixmap(32, 32);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);

    // Draw circle with specified color
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawEllipse(0, 0, 32, 32);

    // Draw text
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(pixmap.rect(), Qt::AlignCenter, letter.toUpper());

    return pixmap;
}



MChatPage::MChatPage(int clubId, int currentUserId, QWidget *parent) : QWidget(parent), m_clubId(clubId), m_currentUserId(currentUserId)
{
    // Get club name from database
    QSqlQuery query;
    query.prepare("SELECT club_name FROM clubs_list WHERE club_id = :clubId");
    query.bindValue(":clubId", clubId);

    if (query.exec() && query.next()) {
        m_clubName = query.value(0).toString();
    } else {
        m_clubName = "Club Chat";
        qDebug() << "Error getting club name:" << query.lastError().text();
    }

    // Initialize avatars colors
    avatarColors = {
        {"You", "#b39ddb"}
    };

    setupUI();
    loadInitialMessages();
}

MChatPage::~MChatPage()
{
    qDebug() << "MChatPage destructor called";
}

void MChatPage::prepareForDestruction()
{
    // Disconnect all signals from this object
    this->disconnect();

    // Also disconnect specific signals as a safety measure
    if (backButton) {
        disconnect(backButton, &QPushButton::clicked, this, &MChatPage::navigateToClub);
    }
    if (sendButton) {
        disconnect(sendButton, &QPushButton::clicked, this, &MChatPage::sendMessage);
    }
    if (messageInput) {
        disconnect(messageInput, &QLineEdit::returnPressed, this, &MChatPage::sendMessage);
    }
}

void MChatPage::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Setup header, messages area, and input area
    setupHeader();
    setupMessagesArea();
    setupInputArea();

    setLayout(mainLayout);

    // Set fixed width to match mobile viewport like in the old version
    setFixedWidth(350);
    setMinimumHeight(650);
    setStyleSheet("background-color: white;");
}




void MChatPage::setupHeader()
{
    headerWidget = new QWidget(this);
    headerWidget->setFixedHeight(60);
    headerWidget->setStyleSheet("background-color: #F2F2F2;");

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);

    // Back button - modified to use resource PNG
    backButton = new QPushButton("", this);
    backButton->setIcon(QIcon(":/images/resources/back.png")); // Use resource PNG
    backButton->setIconSize(QSize(20, 20));
    backButton->setFlat(true);
    backButton->setStyleSheet("QPushButton { border: none; }");
    connect(backButton, &QPushButton::clicked, this, &MChatPage::navigateToClub);

    // Group avatar
    groupAvatarLabel = new QLabel(this);
    groupAvatarLabel->setFixedSize(40, 40);
    groupAvatarLabel->setStyleSheet("background-color: #E0E0E0; border-radius: 20px;");

    // Load club photo if available
    QSqlQuery photoQuery;
    photoQuery.prepare("SELECT club_photo FROM clubs_list WHERE club_id = :clubId");
    photoQuery.bindValue(":clubId", m_clubId);

    if (photoQuery.exec() && photoQuery.next()) {
        QByteArray photoData = photoQuery.value(0).toByteArray();
        if (!photoData.isEmpty()) {
            QPixmap pixmap;
            if (pixmap.loadFromData(photoData)) {
                // Create circular club avatar
                QPixmap clubAvatar = createGroupAvatar(m_clubName.left(1), "#E0E0E0");
                groupAvatarLabel->setPixmap(clubAvatar);
            }
        }
    }

    // Group info (name and status)
    QVBoxLayout *groupInfoLayout = new QVBoxLayout();
    groupInfoLayout->setSpacing(0);

    groupNameLabel = new QLabel(m_clubName, this);  // Use actual club name
    QFont titleFont = groupNameLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    groupNameLabel->setFont(titleFont);
    groupNameLabel->setStyleSheet("color: #404040;");

    statusLabel = new QLabel("Online now", this);
    statusLabel->setStyleSheet("color: #4CAF50; font-size: 12px;");

    groupInfoLayout->addWidget(groupNameLabel);
    groupInfoLayout->addWidget(statusLabel);

    headerLayout->addWidget(backButton);
    headerLayout->addWidget(groupAvatarLabel);
    headerLayout->addLayout(groupInfoLayout, 1);
    headerLayout->addStretch();
    headerLayout->setContentsMargins(10, 5, 10, 5);

    mainLayout->addWidget(headerWidget);
}

void MChatPage::setupMessagesArea()
{
    // Create scroll area for messages
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet("background-color: white;");

    // Create widget to hold all messages
    scrollContent = new QWidget(scrollArea);
    scrollContent->setStyleSheet("background-color: white;");

    messagesLayout = new QVBoxLayout(scrollContent);
    messagesLayout->setContentsMargins(10, 10, 10, 10);
    messagesLayout->setSpacing(15);
    messagesLayout->addStretch(); // Add stretch to keep messages at the top

    scrollContent->setLayout(messagesLayout);
    scrollArea->setWidget(scrollContent);

    mainLayout->addWidget(scrollArea, 1); // Add stretch factor to make scroll area expand
}

void MChatPage::setupInputArea()
{
    inputWidget = new QWidget(this);
    inputWidget->setFixedHeight(60);
    inputWidget->setStyleSheet("background-color: #F2F2F2;");

    QHBoxLayout *inputLayout = new QHBoxLayout(inputWidget);
    inputLayout->setContentsMargins(10, 10, 10, 10);

    // Message input field
    messageInput = new QLineEdit(this);
    messageInput->setPlaceholderText("Type something...");
    messageInput->setStyleSheet(
        "QLineEdit {"
        "   border: none;"
        "   border-radius: 20px;"
        "   padding: 5px 15px;"
        "   font-size: 14px;"
        "   background-color: white;"
        "}"
        );

    // Send button
    sendButton = new QPushButton(this);
    sendButton->setFixedSize(40, 40);
    sendButton->setText("➤");  // Arrow icon as text
    sendButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #E8FDD8;"
        "   border-radius: 20px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        );

    connect(sendButton, &QPushButton::clicked, this, &MChatPage::sendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &MChatPage::sendMessage);

    inputLayout->addWidget(messageInput, 1);
    inputLayout->addWidget(sendButton);

    mainLayout->addWidget(inputWidget, 0); // No stretch factor to keep input fixed
}

void MChatPage::loadInitialMessages()
{
    while (messagesLayout->count() > 0) {
        QLayoutItem* item = messagesLayout->takeAt(0);
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    // Add stretch back
    messagesLayout->addStretch();

    // Query to get messages for this club
    QSqlQuery query;
    query.prepare("SELECT message_content, sender_id, timestamp FROM messages_list "
                  "WHERE club_id = :clubId "
                  "ORDER BY timestamp ASC");
    query.bindValue(":clubId", m_clubId);

    if (!query.exec()) {
        qDebug() << "Error loading messages:" << query.lastError().text();
        // Add fallback message if database query fails
        addMessage("Failed to load messages from database", "System", false);
        return;
    }

    bool hasMessages = false;

    while (query.next()) {
        hasMessages = true;

        QString content = query.value(0).toString();
        int senderId = query.value(1).toInt();
        QDateTime timestamp = QDateTime::fromSecsSinceEpoch(query.value(2).toLongLong());

        bool isCurrentUser = (senderId == m_currentUserId);
        QString senderName = isCurrentUser ? "You" : Database::getUserNameById(senderId);

        // Add message with appropriate sender and timestamp
        addMessage(content, senderName, isCurrentUser, timestamp);
    }

    // Add a welcome message if no messages exist
    if (!hasMessages) {
        addMessage("Welcome to the club chat! Messages sent here will be visible to all club members.", "System", false);
    }

    // Scroll to bottom to show the latest messages
    QTimer::singleShot(100, [this]() {
        if (scrollArea && scrollArea->verticalScrollBar()) {
            scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
        }
    });
}

void MChatPage::sendMessage()
{
    QString text = messageInput->text().trimmed();
    if (!text.isEmpty()) {
        // Save message to database
        if (Database::saveMessage(m_clubId, text, m_currentUserId)) {
            // Add message to UI
            addMessage(text, "You", true);
            messageInput->clear();
        } else {
            // Show error in chat if message couldn't be saved
            addMessage("Failed to send message. Please try again.", "System", false);
        }
    }
}

void MChatPage::addMessage(const QString &text, const QString &sender, bool isOutgoing, const QDateTime &timestamp)
{
    // Remove the stretch if it exists
    if (messagesLayout->count() > 0) {
        QLayoutItem* item = messagesLayout->itemAt(messagesLayout->count() - 1);
        if (item->spacerItem()) {
            messagesLayout->removeItem(item);
            delete item;
        }
    }

    MBubbleMessage *messageBubble = new MBubbleMessage(
        text,
        isOutgoing ? "" : sender,  // Don't show "You" label for outgoing messages
        timestamp,
        isOutgoing ? MBubbleMessage::Outgoing : MBubbleMessage::Incoming,
        this
        );

    messagesLayout->addWidget(messageBubble);
    messagesLayout->addStretch(); // Add stretch to keep messages proper positioning

    // Scroll to bottom to show new message
    QTimer::singleShot(100, [this]() {
        if (scrollArea && scrollArea->verticalScrollBar()) {
            scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
        }
    });
}

QPixmap MChatPage::createGroupAvatar(const QString &name, const QString &color)
{
    QPixmap pixmap(40, 40);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);

    // Draw circle with specified color
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawEllipse(0, 0, 40, 40);

    // Draw text
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(pixmap.rect(), Qt::AlignCenter, name.left(1).toUpper());

    return pixmap;
}
