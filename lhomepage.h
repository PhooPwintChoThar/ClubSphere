#ifndef LHOMEPAGE_H
#define LHOMEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QFrame>
#include <QDateTime>

class AnnouncementCard : public QFrame
{
    Q_OBJECT

public:
    AnnouncementCard(const QString& clubName, const QDateTime& dateTime,
                     const QString& description, const QString& imagePath,
                     int goingCount, int notGoingCount, QWidget* parent = nullptr);

private:
    QLabel* m_profileImage;
    QLabel* m_clubNameLabel;
    QLabel* m_dateTimeLabel;
    QLabel* m_descriptionLabel;
    QLabel* m_eventImageLabel;
    QPushButton* m_goingButton;
    QPushButton* m_notGoingButton;
    QLabel* m_goingCountLabel;
    QLabel* m_notGoingCountLabel;
};

class LHomepage : public QWidget
{
    Q_OBJECT

public:
    explicit LHomepage(QWidget *parent = nullptr);
    ~LHomepage();

signals:
    void showGroupChat(); // New signal to show group chat
    void showNotifications(); // New signal
    void showLeaderboard();

private slots:
    void createNewAnnouncement();
    void onChatButtonClicked(); // New slot to handle chat button click
    void onNotificationButtonClicked(); // New slot
    void onAchievementButtonClicked();

private:
    void setupUI();
    void createMockAnnouncements();
    void addAnnouncementCard(const QString& clubName, const QDateTime& dateTime,
                             const QString& description, const QString& imagePath,
                             int goingCount, int notGoingCount);

    // UI Components
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_headerIconsLayout;
    QHBoxLayout* m_createNewLayout;
    QLabel* m_titleLabel;
    QPushButton* m_createNewButton;
    QScrollArea* m_scrollArea;
    QWidget* m_scrollContent;
    QVBoxLayout* m_announcementsLayout;

    // Added button reference for the chat button
    QPushButton* m_chatButton;
};

#endif // LHOMEPAGE_H
