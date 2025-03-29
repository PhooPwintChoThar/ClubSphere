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
#include <QVector>
#include <QRandomGenerator>


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


        QSqlQuery query;

        // Users table with profile_photo as BLOB and new suspended column
        if (!query.exec("CREATE TABLE IF NOT EXISTS users_list("
                        "user_id INTEGER PRIMARY KEY NOT NULL,"
                        "password INTEGER NOT NULL,"
                        "points INTEGER NOT NULL,"
                        "profile_photo BLOB,"
                        "suspended INTEGER DEFAULT 0,"  // 0 = not suspended, 1 = suspended
                        "name TEXT"  // Added name column
                        ");")) {
            qDebug() << "Error creating users table:" << query.lastError();
            return false;
        }

        // Club leaders table
        if (!query.exec("CREATE TABLE IF NOT EXISTS clubleaders_list("
                        "leader_id INTEGER PRIMARY KEY NOT NULL,"
                        "assigned_club_id INTEGER NOT NULL"
                        ");")) {
            qDebug() << "Error creating clubleaders table:" << query.lastError();
            return false;
        }


        if (!query.exec("CREATE TABLE IF NOT EXISTS clubs_list ("
                                   "club_id INTEGER PRIMARY KEY, "
                                   "club_name TEXT NOT NULL, "
                                   "club_photo BLOB, "
                                   "club_members TEXT, "  // Will store serialized vector of user_ids
                                   "leader_id INTEGER"
                                   ")")) {
            qDebug() << "Error creating clubs_list table:" << query.lastError();
            return false;
        }


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



// Load default club photo as QByteArray
static QByteArray loadDefaultClubPhoto() {
    // Load default club photo from resources
    QImage defaultImage(":/images/resources/club_logo.png");

    if (defaultImage.isNull()) {
        qDebug() << "Failed to load default club image";
        return QByteArray();
    }

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);

    // Save image as PNG
    defaultImage.save(&buffer, "PNG");
    qDebug() << "Successfully loaded default club image";

    return imageData;
}

// Generate a unique club ID
static int generateUniqueClubId() {
    QSqlQuery query;
    int newId;
    bool idExists = true;

    // Keep trying until we find an ID that doesn't exist
    while (idExists) {
        // Generate a random ID between 1000 and 9999
        newId = QRandomGenerator::global()->bounded(1000, 10000);

        // Check if this ID already exists
        query.prepare("SELECT COUNT(*) FROM clubs_list WHERE club_id = :id");
        query.bindValue(":id", newId);

        if (query.exec() && query.next()) {
            idExists = query.value(0).toInt() > 0;
        } else {
            qDebug() << "Error checking club ID:" << query.lastError().text();
            // If there's an error, break the loop and return a "safe" default
            return QRandomGenerator::global()->bounded(1000, 10000);
        }
    }

    return newId;
}

// Check if user is already a leader of a club
static bool isUserAlreadyLeader(int userId, QString& existingClubName) {
    QSqlQuery query;
    query.prepare("SELECT club_name FROM clubs_list WHERE leader_id = :leaderId");
    query.bindValue(":leaderId", userId);

    if (query.exec() && query.next()) {
        existingClubName = query.value(0).toString();
        return true;
    }

    return false;
}

// Convert vector of user IDs to a string for storage
static QString serializeUserIds(const QVector<int>& userIds) {
    QString result;
    for (int i = 0; i < userIds.size(); ++i) {
        if (i > 0) result += ",";
        result += QString::number(userIds[i]);
    }
    return result;
}

// Convert stored string back to vector of user IDs
static QVector<int> deserializeUserIds(const QString& serialized) {
    QVector<int> result;
    if (serialized.isEmpty()) {
        return result;
    }

    QStringList parts = serialized.split(",");
    for (const QString& part : parts) {
        bool ok;
        int id = part.toInt(&ok);
        if (ok) {
            result.append(id);
        }
    }
    return result;
}

};

#endif // DATABASE_H
