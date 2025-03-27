#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class RegisterPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr);

signals:
    void navigateToHome(); // Signal to navigate back to home page
    void openadminHome();

private slots:
    void onRegisterButtonClicked();
    void goBackToHome(); // Slot to handle back button click

public slots:
    void showRegisterPage();

private:
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *logoLabel;
    QLabel *descriptionLabel;
    QLabel *termsCheckBoxLabel;
    QLineEdit *studentIdEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;
    QCheckBox *termsCheckBox;
    QPushButton *signUpButton;
    QPushButton *backButton;
    QVBoxLayout *mainLayout;
};

#endif // REGISTERPAGE_H
