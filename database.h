#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QBuffer>
#include <QImage>

class Database {
public:
    // Initialize database and create tables
    static bool initialize() {
        // Remove any existing connections
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

        // Create a new database connection
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("club_sphere.db");

        if (!db.open()) {
            qDebug() << "Error: connection with database failed";
            return false;
        }

        // Verify total users after opening
        QSqlQuery countQuery;
        if (countQuery.exec("SELECT COUNT(*) FROM users_list")) {
            if (countQuery.next()) {
                qDebug() << "Total users in database during initialization:" << countQuery.value(0).toInt();
            }
        } else {
            qDebug() << "Error counting users:" << countQuery.lastError().text();
        }

        // Rest of your existing initialization code...
        return true;
    }

    // Load default profile photo as QByteArray
    static QByteArray loadDefaultProfilePhoto() {
        // Load default profile photo from resources
        QImage defaultImage(":/images/resources/user_profile.jpg");

        if (defaultImage.isNull()) {
            qDebug() << "Failed to load default profile image";
            return QByteArray();
        }

        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);

        // Save image as PNG
        defaultImage.save(&buffer, "PNG");
        qDebug() << "Succeed to load default profile image";

        return imageData;
    }

    // Optional: Method to add a sample user
    static bool addSampleUser(const QString& name, int points, bool suspended = false) {
        QSqlQuery query;
        query.prepare("INSERT INTO users_list (password, points, profile_photo, suspended, name) "
                      "VALUES (:password, :points, :photo, :suspended, :name)");
        query.bindValue(":password", 1234);  // Default password
        query.bindValue(":points", points);
        query.bindValue(":photo", loadDefaultProfilePhoto());
        query.bindValue(":suspended", suspended ? 1 : 0);
        query.bindValue(":name", name);

        if (!query.exec()) {
            qDebug() << "Error adding sample user:" << query.lastError();
            return false;
        }
        return true;
    }
};

#endif // DATABASE_H
