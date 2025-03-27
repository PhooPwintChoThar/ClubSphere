#ifndef ADMINNOTI_H
#define ADMINNOTI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QScrollArea>

class AdminNoti : public QWidget
{
    Q_OBJECT

public:
    explicit AdminNoti(QWidget *parent = nullptr);
    ~AdminNoti();

signals:
    void navigateToHome();
    void navigateToMembers();
    void navigateToClubs();
    // This class doesn't need to navigate to itself

private slots:
    void onHomeButtonClicked();
    void onProfileButtonClicked();
    void onGroupsButtonClicked();
    void onNotificationsButtonClicked();

private:
    void setupUI();
    void setupNotificationsArea();
    void setupNavigation();
    void createSuspensionRequest(const QString &username, const QString &reportedBy, const QString &reason);

    QFrame* createRoundedFrame();

    // Layout components
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;

    // Notification components
    QScrollArea *notificationsScrollArea;
    QWidget *notificationsContainer;
    QVBoxLayout *notificationsLayout;

    // Navigation components
    QFrame *navigationFrame;
    QPushButton *homeButton;
    QPushButton *profileButton;
    QPushButton *groupsButton;
    QPushButton *notificationsButton;
};

#endif // ADMINNOTI_H
