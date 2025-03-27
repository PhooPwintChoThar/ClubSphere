#ifndef ADMINMEMBER_H
#define ADMINMEMBER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QLineEdit>

class AdminMember : public QWidget
{
    Q_OBJECT

public:
    explicit AdminMember(QWidget *parent = nullptr);
    ~AdminMember();

signals:
    void navigateToHome();
    void navigateToClubs();
    void navigateToNotifications();

private slots:
    void onHomeButtonClicked();
    void onProfileButtonClicked();
    void onGroupsButtonClicked();
    void onNotificationsButtonClicked();

private:
    void setupUI();
    void setupMemberList();
    void toggleUserSuspension(const QString &userId, bool suspend);
    void setupSearchFunctionality();
    void searchMembers(const QString &searchText);
    void setupNavigation();
    void createMemberCard(const QString &name, const QString &id, int points, bool suspended);

    QFrame* createRoundedFrame();

    // Main layout
    QVBoxLayout *mainLayout;

    // Title and search
    QLabel *titleLabel;
    QFrame *searchFrame;
    QLineEdit *searchEdit;

    // Member list
    QLabel *membersLabel;
    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QVBoxLayout *membersLayout;

    // Navigation bar
    QFrame *navigationFrame;
    QPushButton *homeButton;
    QPushButton *profileButton;
    QPushButton *groupsButton;
    QPushButton *notificationsButton;
};

#endif // ADMINMEMBER_H
