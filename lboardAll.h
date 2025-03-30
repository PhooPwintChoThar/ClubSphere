#ifndef LBOARDALL_H
#define LBOARDALL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>

class LeaderboardAll : public QWidget
{
    Q_OBJECT

public:
    explicit LeaderboardAll(QWidget *parent = nullptr);
    ~LeaderboardAll();

signals:
    void navigateBack();

private slots:
    void onBackButtonClicked();
    void onAllTabClicked();
    void onMembersTabClicked();
    void onClubsTabClicked();

private:
    void setupUI();
    void setupTabBar();
    void setupTopRankersSection();
    void setupRankingListSection();
    void updateTabButtonStyles();
    QLabel* createMedalLabel(const QString &color);
    QWidget* createTopRankerWidget(const QString &medalType, const QString &name, int score);
    QWidget* createRankListItemWidget(const QString &name, int score);
    QWidget* createTopUserWidget(const QString &medalType, const QString &name);
    QWidget* createTopClubWidget(const QString &medalType, const QString &name);

    // Member data
    struct MemberData {
        QString name;
        int score;
    };

    // Club data
    struct ClubData {
        QString name;
        int score;
    };

    void loadMemberData();
    void loadClubData();
    void updateMembersView();
    void updateClubsView();
    void updateAllView();
    void createSectionHeader(QVBoxLayout *layout, const QString &title);

    QList<MemberData> memberList;
    QList<ClubData> clubList;

    // State tracking
    enum TabState {
        ALL_TAB,
        MEMBERS_TAB,
        CLUBS_TAB
    };
    TabState currentTab;

    // Main layout and components
    QVBoxLayout *mainLayout;

    // Header components
    QWidget *headerWidget;
    QHBoxLayout *headerLayout;
    QPushButton *backButton;
    QLabel *titleLabel;

    // Tab components
    QWidget *tabWidget;
    QHBoxLayout *tabLayout;
    QPushButton *allTabButton;
    QPushButton *membersTabButton;
    QPushButton *clubsTabButton;

    // Top Rankers section
    QWidget *topRankersWidget;
    QHBoxLayout *topRankersLayout;

    // Rankings list section (scrollable)
    QScrollArea *rankingsScrollArea;
    QWidget *rankingsListWidget;
    QVBoxLayout *rankingsListLayout;
};

#endif // LBOARDALL_H
