// mprofilepage.h
#ifndef MPROFILEPAGE_H
#define MPROFILEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QTimer>
#include <QScrollArea>

class MProfilePage : public QWidget
{
    Q_OBJECT

public:
    explicit MProfilePage(QWidget *parent = nullptr);
    ~MProfilePage();

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

private:
    void setupUI();

    // UI Components
    QVBoxLayout* m_mainLayout;
    QLabel* m_titleLabel;

    // Bottom navigation
    QWidget* m_bottomNavBar;
    QHBoxLayout* m_bottomNavLayout;
};

#endif // MPROFILEPAGE_H
