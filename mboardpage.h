// mboardpage.h
#ifndef MBOARDPAGE_H
#define MBOARDPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QButtonGroup>
#include <QRadioButton>

class MBoardPage : public QWidget
{
    Q_OBJECT

public:
    explicit MBoardPage(QWidget *parent = nullptr);
    ~MBoardPage();

signals:
    void navigateToHome();
    void navigateToClub();
    void navigateToGoing();
    void navigateToProfile();
    void navigateToBoard();

private slots:
    void homeClicked();
    void clubClicked();
    void eventClicked();
    void profileClicked();
    void boardClicked();
    void onMembersTabClicked();
    void onClubsTabClicked();

private:
    void setupUI();
    void setupTopLeaders();
    void setupListItems();
    void clearLeaderboard();
    void showMembersLeaderboard();
    void showClubsLeaderboard();
    QWidget* createLeaderItem(const QString& name, int points, bool isTopThree = false, int rank = 0);

    // UI Components
    QVBoxLayout* m_mainLayout;
    QLabel* m_titleLabel;
    QScrollArea* m_scrollArea;
    QWidget* m_scrollContent;
    QVBoxLayout* m_contentLayout;
    QWidget* m_topThreeWidget;
    QVBoxLayout* m_leaderListLayout;

    // Tab switching
    QButtonGroup* m_tabGroup;
    QRadioButton* m_membersTab;
    QRadioButton* m_clubsTab;

    // Bottom navigation
    QWidget* m_bottomNavBar;
    QHBoxLayout* m_bottomNavLayout;

    // Current view state
    bool m_showingMembers;
};

#endif // MBOARDPAGE_H
