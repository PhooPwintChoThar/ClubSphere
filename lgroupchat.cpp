// lgroupchat.cpp
#include "lgroupchat.h"
#include <QPainter>
#include <QDebug>
#include <QScrollBar>
#include <QFont>
#include <QFontMetrics>
#include <QTimer>

BubbleMessage::BubbleMessage(const QString &text, const QString &sender, const QDateTime &time, MessageType type, QWidget *parent)
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

QPixmap BubbleMessage::createAvatar(const QString &letter, const QString &color)
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

LGroupChat::LGroupChat(QWidget *parent) : QWidget(parent)
{
    // Initialize participants and avatars
    participants = {"John", "Alice", "Amina"};
    avatarColors = {
        {"John", "#c5e1a5"},
        {"Alice", "#ffcc80"},
        {"Amina", "#90caf9"},
        {"You", "#b39ddb"}
    };

    setupUI();
    loadInitialMessages();
}

LGroupChat::~LGroupChat()
{
    // Clean up explicitly to prevent dangling pointers
    qDebug() << "LGroupChat destructor called";
}

void LGroupChat::prepareForDestruction()
{
    // Disconnect all signals from this object
    this->disconnect();

    // Also disconnect specific signals as a safety measure
    if (backButton) {
        disconnect(backButton, &QPushButton::clicked, this, &LGroupChat::goBack);
    }
    if (sendButton) {
        disconnect(sendButton, &QPushButton::clicked, this, &LGroupChat::sendMessage);
    }
    if (messageInput) {
        disconnect(messageInput, &QLineEdit::returnPressed, this, &LGroupChat::sendMessage);
    }
}

void LGroupChat::setupUI()
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
    setFixedWidth(375);
    setMinimumHeight(800);
    setStyleSheet("background-color: white;");
}

void LGroupChat::setupHeader()
{
    headerWidget = new QWidget(this);
    headerWidget->setFixedHeight(60);
    headerWidget->setStyleSheet("background-color: #F2F2F2;");

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);

    // Back button - modified to use resource PNG
    backButton = new QPushButton("", this);
    backButton->setIcon(QIcon(":/resources/back.png")); // Use resource PNG
    backButton->setIconSize(QSize(20, 20));
    backButton->setFlat(true);
    backButton->setStyleSheet("QPushButton { border: none; }");
    connect(backButton, &QPushButton::clicked, this, &LGroupChat::goBack);

    // Group avatar
    groupAvatarLabel = new QLabel(this);
    groupAvatarLabel->setFixedSize(40, 40);
    groupAvatarLabel->setStyleSheet("background-color: #E0E0E0; border-radius: 20px;");

    // Group info (name and status)
    QVBoxLayout *groupInfoLayout = new QVBoxLayout();
    groupInfoLayout->setSpacing(0);

    groupNameLabel = new QLabel("KMITL science club", this);
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

void LGroupChat::setupMessagesArea()
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
    messagesLayout->addStretch(); // Add stretch to keep messages at the top, as in old version

    scrollContent->setLayout(messagesLayout);
    scrollArea->setWidget(scrollContent);

    mainLayout->addWidget(scrollArea, 1);
}

void LGroupChat::setupInputArea()
{
    inputWidget = new QWidget(this);
    inputWidget->setFixedHeight(60);
    inputWidget->setStyleSheet("background-color: #F2F2F2;");

    QHBoxLayout *inputLayout = new QHBoxLayout(inputWidget);
    inputLayout->setContentsMargins(10, 10, 10, 10);

    // Message input field - matching old style
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

    // Send button - matching old style
    sendButton = new QPushButton(this);
    sendButton->setFixedSize(40, 40);
    sendButton->setText("➤");  // Arrow icon as text (you can replace with QIcon if you have the resource)
    sendButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #E8FDD8;"
        "   border-radius: 20px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "}"
        );

    connect(sendButton, &QPushButton::clicked, this, &LGroupChat::sendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &LGroupChat::sendMessage);

    inputLayout->addWidget(messageInput, 1);
    inputLayout->addWidget(sendButton);

    mainLayout->addWidget(inputWidget);
}

void LGroupChat::loadInitialMessages()
{
    // Add messages shown in the screenshot - exactly as in the old version
    addMessage("Hey! How's it going? I was wondering what time are we going to leave for today club meeting?", "John", false);
    addMessage("Honestly I don't think I'll be able to make it :( Something came up at my end. I'm really sorry!", "You", true);
    addMessage("Oh, okay. How about others?", "John", false);
    addMessage("Maybe around 6:30pm?", "Alice", false);
    addMessage("Talked to Amina and she's going and says 8 pm is better", "You", true);
    addMessage("Okay great! Going to miss you. Could you send us her number?", "Alice", false);
    addMessage("Yeah sure!", "You", true);

    // Scroll to bottom to show the latest messages
    QTimer::singleShot(100, [this]() {
        if (scrollArea && scrollArea->verticalScrollBar()) {
            scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
        }
    });
}

void LGroupChat::sendMessage()
{
    QString text = messageInput->text().trimmed();
    if (!text.isEmpty()) {
        addMessage(text, "You", true);
        messageInput->clear();
    }
}

void LGroupChat::addMessage(const QString &text, const QString &sender, bool isOutgoing)
{
    // Remove the stretch if it exists (just like in the old implementation)
    if (messagesLayout->count() > 0) {
        QLayoutItem* item = messagesLayout->itemAt(messagesLayout->count() - 1);
        if (item->spacerItem()) {
            messagesLayout->removeItem(item);
            delete item;
        }
    }

    BubbleMessage *messageBubble = new BubbleMessage(
        text,
        isOutgoing ? "" : sender,  // Don't show "You" label for outgoing messages
        QDateTime::currentDateTime(),
        isOutgoing ? BubbleMessage::Outgoing : BubbleMessage::Incoming,
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

void LGroupChat::goBack()
{
    emit navigateToHomePage();
}

QPixmap LGroupChat::createAvatar(const QString &name, const QString &color)
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
