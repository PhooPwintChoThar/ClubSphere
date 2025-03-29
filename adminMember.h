#ifndef ADMINMEMBER_H
#define ADMINMEMBER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QFrame>

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
    void searchMembers(const QString &searchText);

private:
    // UI components
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QFrame *searchFrame;
    QLineEdit *searchEdit;
    QLabel *membersLabel;
    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QVBoxLayout *membersLayout;
    QFrame *navigationFrame;
    QPushButton *homeButton;
    QPushButton *profileButton;
    QPushButton *groupsButton;
    QPushButton *notificationsButton;

    // Methods
    void setupUI();
    void setupMemberList();
    void setupNavigation();
    void setupSearchFunctionality();
    QFrame* createRoundedFrame();
    void createMemberCard(const QString &name, const QString &id, int points, bool suspended, const QByteArray &profileImageData = QByteArray());
    void toggleUserSuspension(const QString &userId, bool suspend);
};

#endif // ADMINMEMBER_H
