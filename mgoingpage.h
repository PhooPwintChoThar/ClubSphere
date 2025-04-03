// mgoingpage.h
#ifndef MGOINGPAGE_H
#define MGOINGPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QPainter>
#include <QLineEdit>

class MGoingPage : public QWidget
{
    Q_OBJECT
public:
    explicit MGoingPage(QWidget *parent = nullptr);
    ~MGoingPage();

signals:
    void navigateToHome();
    void navigateToClub();
    void navigateToGoing();
    void navigateToProfile();

private slots:
    void homeClicked();
    void clubClicked();
    void eventClicked();
    void profileClicked();
    void showJoinedView();
    void showPendingView();

private:
    enum class JoinStatus {
        Button,
        Pending,
        Cancel
    };

    void setupUI();
    QWidget* createClubItem(const QString& name, const QString& rank, const QString& members, JoinStatus status);
    QWidget* createClubEventItem(const QString& name, const QString& rank, const QString& members);
    QWidget* createSeparator();

    // UI Components
    QVBoxLayout* m_mainLayout;
    QLabel* m_titleLabel;
    QPushButton* m_joinedButton;
    QPushButton* m_pendingButton;
    QStackedWidget* m_stackedWidget;

    // Bottom navigation
    QWidget* m_bottomNavBar;
    QHBoxLayout* m_bottomNavLayout;
};

#endif // MGOINGPAGE_H
