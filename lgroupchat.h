// lgroupchat.h
#ifndef LGROUPCHAT_H
#define LGROUPCHAT_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDateTime>
#include <QMap>
#include <QTimer>

class BubbleMessage : public QWidget
{
    Q_OBJECT
public:
    enum MessageType { Incoming, Outgoing };

    BubbleMessage(const QString &text, const QString &sender, const QDateTime &time, MessageType type, QWidget *parent = nullptr);

private:
    QLabel *messageLabel;
    QLabel *timeLabel;
    QLabel *senderLabel;
    MessageType messageType;

    QPixmap createAvatar(const QString &letter, const QString &color);
};

class LGroupChat : public QWidget
{
    Q_OBJECT

public:
    explicit LGroupChat(QWidget *parent = nullptr);
    ~LGroupChat() override;
    void prepareForDestruction();

signals:
    void navigateToHomePage();

private slots:
    void sendMessage();
    void addMessage(const QString &text, const QString &sender, bool isOutgoing);
    void goBack();

private:
    // UI Components
    QWidget *headerWidget;
    QLabel *groupNameLabel;
    QLabel *statusLabel;
    QPushButton *backButton;
    QLabel *groupAvatarLabel;

    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QVBoxLayout *messagesLayout;

    QWidget *inputWidget;
    QLineEdit *messageInput;
    QPushButton *sendButton;

    QVBoxLayout *mainLayout;

    // Data
    QMap<QString, QString> avatarColors;
    QStringList participants;

    void setupUI();
    void setupHeader();
    void setupMessagesArea();
    void setupInputArea();
    void loadInitialMessages();
    QPixmap createAvatar(const QString &name, const QString &color);
};

#endif // LGROUPCHAT_H
