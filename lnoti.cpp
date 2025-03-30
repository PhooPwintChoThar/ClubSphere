#include "lnoti.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QFrame>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

// Helper function to create a circular user avatar
QPixmap createCircularAvatar(int size, const QColor &color = Qt::white) {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(color));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, size, size);

    return pixmap;
}

LNoti::LNoti(QWidget *parent)
    : QWidget(parent)
{
    // Sample data
    m_requests.append(UserRequest("Jenny", 250));
    m_requests.append(UserRequest("John", 50));
    m_requests.append(UserRequest("Alice", 300));
    m_requests.append(UserRequest("Mary", 120));
    m_requests.append(UserRequest("Michael", 300));

    // Add more sample data to demonstrate scrolling
    m_requests.append(UserRequest("Robert", 175));
    m_requests.append(UserRequest("Sarah", 225));
    m_requests.append(UserRequest("David", 190));

    setupUI();
}

LNoti::~LNoti()
{
}

void LNoti::setupUI()
{
    // Set background color to white and fix window size
    this->setStyleSheet("background-color: white;");
    this->setFixedSize(350, 650);

    // Main layout setup
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setAlignment(Qt::AlignTop);
    m_mainLayout->setContentsMargins(3, 30, 3, 20);
    m_mainLayout->setSpacing(15);

    // Create a combined header widget with back button and title
    QWidget* headerWidget = new QWidget(this);
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(5, 5, 5, 5);

    // Back button using resource PNG - defined as local variable
    QPushButton* backButton = new QPushButton("", this);
    backButton->setIcon(QIcon(":/resources/back.png"));
    backButton->setIconSize(QSize(20, 20));
    backButton->setFlat(true);
    backButton->setStyleSheet("QPushButton { border: none; }");
    connect(backButton, &QPushButton::clicked, this, &LNoti::onBackClicked);

    // Title
    QLabel* titleLabel = new QLabel("Clubsphere", this);
    titleLabel->setFont(QFont("Arial", 16, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);

    // Add widgets to header layout
    headerLayout->addWidget(backButton, 0);
    headerLayout->addWidget(titleLabel, 1);

    // Add header to main layout
    m_mainLayout->addWidget(headerWidget);

    // Create notifications area
    setupNotificationsArea();
}
void LNoti::setupNotificationsArea()
{
    // Create the scroll area for notifications (matching adminNoti)
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setMinimumHeight(480); // Same as adminNoti

    // Create container widget for notifications (matching adminNoti)
    m_scrollContent = new QWidget(m_scrollArea);
    m_scrollContent->setStyleSheet("background-color: #D9E9D8; border-radius: 12px;");

    // Create layout for notifications (matching adminNoti)
    m_requestsLayout = new QVBoxLayout(m_scrollContent);
    m_requestsLayout->setContentsMargins(15, 15, 15, 15);
    m_requestsLayout->setSpacing(10);

    // Add a title to the notifications section (matching adminNoti)
    QLabel *requestsLabel = new QLabel("Requests to join clubs", m_scrollContent);
    QFont requestsFont;
    requestsFont.setBold(true);
    requestsFont.setPointSize(14);
    requestsLabel->setFont(requestsFont);
    requestsLabel->setAlignment(Qt::AlignCenter);
    m_requestsLayout->addWidget(requestsLabel);

    // Add user request widgets
    for (int i = 0; i < m_requests.size(); ++i) {
        QWidget *requestWidget = createUserRequestWidget(m_requests[i], i);
        m_requestWidgets.append(requestWidget);
        m_requestsLayout->addWidget(requestWidget);

        // Add separator except after the last item (matching adminNoti)
        if (i < m_requests.size() - 1) {
            QFrame *line = new QFrame(m_scrollContent);
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            line->setStyleSheet("background-color: #888888;");
            line->setFixedHeight(1);
            m_requestsLayout->addWidget(line);
        }
    }

    // Set the container as the widget for the scroll area
    m_scrollArea->setWidget(m_scrollContent);

    // Add scroll area to main layout with proper margins (matching adminNoti)
    QHBoxLayout *scrollAreaLayout = new QHBoxLayout();
    scrollAreaLayout->setContentsMargins(10, 0, 10, 0);
    scrollAreaLayout->addWidget(m_scrollArea);
    m_mainLayout->addLayout(scrollAreaLayout);
}

QWidget* LNoti::createHeaderWidget()
{
    QWidget *headerWidget = new QWidget();
    headerWidget->setFixedHeight(50);
    headerWidget->setStyleSheet("background-color: white;");

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(10, 0, 10, 0);

    // Use resource PNG for back button
    QPushButton *backButton = new QPushButton("", this);
    backButton->setIcon(QIcon(":/resources/back.png"));
    backButton->setIconSize(QSize(20, 20));
    backButton->setFlat(true);
    backButton->setStyleSheet("QPushButton { border: none; }");
    connect(backButton, &QPushButton::clicked, this, &LNoti::onBackClicked);

    // Empty widget for spacing
    QWidget* spacer = new QWidget();
    spacer->setFixedWidth(30);

    headerLayout->addWidget(backButton);
    headerLayout->addStretch(1);
    headerLayout->addWidget(spacer);

    return headerWidget;
}

QWidget* LNoti::createUserRequestWidget(const UserRequest &request, int index)
{
    // User info layout (matching adminNoti style)
    QHBoxLayout *requestLayout = new QHBoxLayout();
    requestLayout->setContentsMargins(5, 10, 5, 10);  // Add padding around items

    // User icon (matching adminNoti style)
    QLabel *userIcon = new QLabel();
    userIcon->setFixedSize(40, 40);  // Match adminNoti size
    userIcon->setStyleSheet("background-color: #FFFFFF; border-radius: 20px;");
    userIcon->setPixmap(createCircularAvatar(40, Qt::white));

    // User details (matching adminNoti layout)
    QVBoxLayout *userDetailsLayout = new QVBoxLayout();
    userDetailsLayout->setSpacing(3);  // Tighter spacing between text lines

    QLabel *usernameLabel = new QLabel(request.name);
    QFont usernameFont;
    usernameFont.setBold(true);
    usernameFont.setPointSize(12);  // Match adminNoti
    usernameLabel->setFont(usernameFont);

    QLabel *pointsLabel = new QLabel(QString("points: %1").arg(request.points));
    pointsLabel->setStyleSheet("color: #555555;");
    QFont pointsFont;
    pointsFont.setPointSize(10);  // Match adminNoti
    pointsLabel->setFont(pointsFont);

    userDetailsLayout->addWidget(usernameLabel);
    userDetailsLayout->addWidget(pointsLabel);

    // Action buttons (matching adminNoti style)
    QVBoxLayout *actionButtonsLayout = new QVBoxLayout();
    actionButtonsLayout->setSpacing(6);  // Match adminNoti spacing

    // Approve button (checkmark) - match adminNoti style
    QPushButton *approveButton = new QPushButton();
    approveButton->setFixedSize(32, 32);
    approveButton->setIcon(QIcon(":/images/resources/checkmark.png"));
    approveButton->setIconSize(QSize(18, 18));
    approveButton->setStyleSheet("QPushButton { background-color: white; border-radius: 16px; border: 1px solid #CCCCCC; }");

    // Fallback if image not found
    if (approveButton->icon().isNull()) {
        approveButton->setText("✓");
    }

    connect(approveButton, &QPushButton::clicked, [this, index]() {
        onAcceptClicked(index);
    });

    // Reject button (X) - match adminNoti style
    QPushButton *rejectButton = new QPushButton();
    rejectButton->setFixedSize(32, 32);
    rejectButton->setIcon(QIcon(":/images/resources/x_mark.png"));
    rejectButton->setIconSize(QSize(18, 18));
    rejectButton->setStyleSheet("QPushButton { background-color: white; border-radius: 16px; border: 1px solid #CCCCCC; }");

    // Fallback if image not found
    if (rejectButton->icon().isNull()) {
        rejectButton->setText("✗");
    }

    connect(rejectButton, &QPushButton::clicked, [this, index]() {
        onRejectClicked(index);
    });

    actionButtonsLayout->addWidget(approveButton);
    actionButtonsLayout->addWidget(rejectButton);

    // Add all elements to the request layout
    requestLayout->addWidget(userIcon);
    requestLayout->addLayout(userDetailsLayout, 1);
    requestLayout->addLayout(actionButtonsLayout);
    requestLayout->setSpacing(10);  // Spacing between icon, text, and buttons

    // Create a widget to contain this layout
    QWidget *widget = new QWidget();
    widget->setLayout(requestLayout);

    return widget;
}

void LNoti::onBackClicked()
{
    emit navigateBack();
}

void LNoti::onAcceptClicked(int index)
{
    if (index >= 0 && index < m_requestWidgets.size()) {
        qDebug() << "Accepted request from:" << m_requests[index].name;
        // Here you would handle the acceptance logic

        // Remove the widget from layout
        if (m_requestWidgets[index]) {
            // Get the separator following this widget (if not the last one)
            QLayoutItem* separator = nullptr;
            if (index < m_requestWidgets.size() - 1) {
                separator = m_requestsLayout->itemAt(m_requestsLayout->indexOf(m_requestWidgets[index]) + 1);
            }

            // Remove and delete the widget
            m_requestsLayout->removeWidget(m_requestWidgets[index]);
            delete m_requestWidgets[index];
            m_requestWidgets[index] = nullptr;

            // Remove and delete the separator if it exists
            if (separator) {
                QWidget* sepWidget = separator->widget();
                m_requestsLayout->removeItem(separator);
                delete sepWidget;
            }

            // Remove from the data model
            m_requests.remove(index);
            m_requestWidgets.remove(index);
        }
    }
}

void LNoti::onRejectClicked(int index)
{
    if (index >= 0 && index < m_requestWidgets.size()) {
        qDebug() << "Rejected request from:" << m_requests[index].name;
        // Here you would handle the rejection logic

        // Remove the widget from layout
        if (m_requestWidgets[index]) {
            // Get the separator following this widget (if not the last one)
            QLayoutItem* separator = nullptr;
            if (index < m_requestWidgets.size() - 1) {
                separator = m_requestsLayout->itemAt(m_requestsLayout->indexOf(m_requestWidgets[index]) + 1);
            }

            // Remove and delete the widget
            m_requestsLayout->removeWidget(m_requestWidgets[index]);
            delete m_requestWidgets[index];
            m_requestWidgets[index] = nullptr;

            // Remove and delete the separator if it exists
            if (separator) {
                QWidget* sepWidget = separator->widget();
                m_requestsLayout->removeItem(separator);
                delete sepWidget;
            }

            // Remove from the data model
            m_requests.remove(index);
            m_requestWidgets.remove(index);
        }
    }
}
