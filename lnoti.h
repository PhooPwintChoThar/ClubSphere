#ifndef LNOTI_H
#define LNOTI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QVector>
#include <QFrame>

class UserRequest {
public:
    QString name;
    int points;

    UserRequest(const QString &n, int p) : name(n), points(p) {}
};

class LNoti : public QWidget
{
    Q_OBJECT

public:
    explicit LNoti(QWidget *parent = nullptr);
    ~LNoti();

signals:
    void navigateBack();

private slots:
    void onBackClicked();
    void onAcceptClicked(int index);
    void onRejectClicked(int index);

private:
    void setupUI();
    void setupNotificationsArea(); // Add this function declaration
    QWidget* createUserRequestWidget(const UserRequest &request, int index);
    QWidget* createHeaderWidget();

    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_scrollContent;
    QVBoxLayout *m_requestsLayout;

    QVector<UserRequest> m_requests;
    QVector<QWidget*> m_requestWidgets;
};

#endif // LNOTI_H
