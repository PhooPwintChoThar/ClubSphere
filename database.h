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
#include<QDateTime>


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

        if (!query.exec("CREATE TABLE IF NOT EXISTS users_list("
                        "user_id INTEGER PRIMARY KEY NOT NULL,"
                        "password INTEGER NOT NULL,"
                        "points INTEGER NOT NULL,"
                        "profile_photo BLOB,"
                        "name TEXT,"  // Added name column
                        "joined_clubs TEXT,"  // Stores serialized club IDs that user has joined
                        "pending_clubs TEXT,"  // Stores serialized club IDs that user has requested but not approved
                        "going_events TEXT"  // Stores serialized event IDs that user is going to
                        ");")) {
            qDebug() << "Error creating users table:" << query.lastError();
            return false;
        }

        query.prepare("SELECT COUNT(*) FROM users_list WHERE user_id = 67001922");
        if (query.exec() && query.next() && query.value(0).toInt() == 0) {
            // User doesn't exist, create default user
            QByteArray defaultProfilePhoto = loadDefaultProfilePhoto();

            query.prepare("INSERT INTO users_list (user_id, password, points, profile_photo, name) "
                          "VALUES (67001922, 123456, 0, :profile_photo, 'Admin')");
            query.bindValue(":profile_photo", defaultProfilePhoto);

            if (!query.exec()) {
                qDebug() << "Error creating default user:" << query.lastError();
                return false;
            }
            qDebug() << "Created default user with ID 67001922";
        }

        if (!query.exec("CREATE TABLE IF NOT EXISTS clubs_list ("
                        "club_id INTEGER PRIMARY KEY, "
                        "club_name TEXT NOT NULL, "
                        "club_photo BLOB, "
                        "club_members TEXT, "  // Will store serialized vector of user_ids
                        "leader_id INTEGER, "
                        "event_ids TEXT"  // Will store serialized vector of event_ids
                        ")")) {
            qDebug() << "Error creating clubs_list table:" << query.lastError();
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


        if (!query.exec("CREATE TABLE IF NOT EXISTS events_list ("
                        "event_id INTEGER PRIMARY KEY, "
                        "club_id INTEGER NOT NULL, "
                        "event_content TEXT, "
                        "event_photo BLOB, "
                        "event_code TEXT, "
                        "event_going_count INTEGER DEFAULT 0, "
                        "created_timestamp INTEGER NOT NULL"
                        ")")) {
            qDebug() << "Error creating events_list table:" << query.lastError();
            return false;
        }

        // Add the messages_list table
        if (!query.exec("CREATE TABLE IF NOT EXISTS messages_list ("
                        "message_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "club_id INTEGER NOT NULL, "
                        "message_content TEXT NOT NULL, "
                        "sender_id INTEGER NOT NULL, "
                        "timestamp INTEGER NOT NULL, "
                        "FOREIGN KEY (club_id) REFERENCES clubs_list(club_id), "
                        "FOREIGN KEY (sender_id) REFERENCES users_list(user_id)"
                        ")")) {
            qDebug() << "Error creating messages_list table:" << query.lastError();
            return false;
        }


        if (!query.exec("CREATE TABLE IF NOT EXISTS notification_lists ("
                        "club_id INTEGER NOT NULL, "
                        "is_request INTEGER NOT NULL, "  // 0 = notification, 1 = request
                        "content TEXT NOT NULL, "
                        "user_id INTEGER NOT NULL, "
                        "timestamp INTEGER NOT NULL, "
                        "FOREIGN KEY (club_id) REFERENCES clubs_list(club_id), "
                        "FOREIGN KEY (user_id) REFERENCES users_list(user_id)"
                        ")")) {
            qDebug() << "Error creating notification_lists table:" << query.lastError();
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


    static int generateUniqueEventId() {
        QSqlQuery query;
        int newId;
        bool idExists = true;

        // Keep trying until we find an ID that doesn't exist
        while (idExists) {
            // Generate a random ID between 10000 and 99999
            newId = QRandomGenerator::global()->bounded(10000, 100000);

            // Check if this ID already exists
            query.prepare("SELECT COUNT(*) FROM events_list WHERE event_id = :id");
            query.bindValue(":id", newId);

            if (query.exec() && query.next()) {
                idExists = query.value(0).toInt() > 0;
            } else {
                qDebug() << "Error checking event ID:" << query.lastError().text();
                // If there's an error, break the loop and return a "safe" default
                return QRandomGenerator::global()->bounded(10000, 100000);
            }
        }

        return newId;
    }

    // Helper function to save a message to the database
    static bool saveMessage(int clubId, const QString& content, int senderId) {
        QSqlQuery query;
        query.prepare("INSERT INTO messages_list (club_id, message_content, sender_id, timestamp) "
                      "VALUES (:club_id, :content, :sender_id, :timestamp)");
        query.bindValue(":club_id", clubId);
        query.bindValue(":content", content);
        query.bindValue(":sender_id", senderId);
        query.bindValue(":timestamp", QDateTime::currentSecsSinceEpoch());

        if (!query.exec()) {
            qDebug() << "Error saving message:" << query.lastError().text();
            return false;
        }

        return true;
    }

    // Helper function to get user name by ID
    static QString getUserNameById(int userId) {
        QSqlQuery query;
        query.prepare("SELECT name FROM users_list WHERE user_id = :id");
        query.bindValue(":id", userId);

        if (query.exec() && query.next()) {
            return query.value(0).toString();
        }

        return QString("Unknown User");
    }

    static bool initializeStudentNames() {
        QSqlQuery query;

        // Create the se_names_list table if it doesn't exist
        if (!query.exec("CREATE TABLE IF NOT EXISTS se_names_list("
                        "student_id INTEGER PRIMARY KEY NOT NULL,"
                        "name TEXT NOT NULL"
                        ");")) {
            qDebug() << "Error creating se_names_list table:" << query.lastError();
            return false;
        }

        // Sample data - replace with your actual student data
        QVector<QPair<int, QString>> studentData = {
                                                    {67011073, "Adisorn Numpradit"}, {67011078, "Bhawat Kolkitchaiwan"},
                                                    {67011091, "Chananyu Chinnawuth"}, {67011093, "Chavit Saritdeechakul"}, {67011096, "Chayut Panangkasiri"}, {67011100, "Chirawad Koollachote"}, {67011110, "Ekboonya Srisook"},
                                                    {67011118, "Hsu Myat Shwe Sin"}, {67011117, "Khanin Chuanchaisit"}, {67011152, "Koses Suvarnasuddhi"}, {67011153, "Kyi Thant Sin"}, {67011177, "Napatrawee Chieowwitt"},
                                                    {67011112, "Nuttamon Ketkaeo"}, {67011236, "Pannawhiz Pipatmunkong"}, {67011258, "Paphavee Yanmook"}, {67011273, "Payut Kapasuwan"}, {67011287, "Ramida Laphashopkin"},
                                                    {67011297, "Sarun Rattanapan"}, {67011300, "Singhayapol Kliengma"}, {67011302, "Sirapot Satarntraipope"}, {67011318, "Supichaya Ratanaopas"}, {67011322, "Suwitchaya Chintawan"},
                                                    {67011335, "Thanaphat Chongkananu"}, {67011352, "Theepakorn Phayonrat"}, {67011362, "Theepakon Khwanna"}, {67011372, "Thunthanut Teemaethaw"}, {67011371, "Virithpol Thara"},
                                                    {67011596, "Han Ni Aung"}, {67011614, "Korapat Tripatarasit"}, {67011615, "La Min Maung"}, {67011619, "May Nyein Chan"}, {67011627, "Nopparath Nonraksanuk"}, {67011629, "Norrapat Nimdit"},
                                                    {67011638, "Pasu"}, {67011653, "Phone Myat Pyae Sone"}, {67011659, "Saw Zi Dunn"}, {67011685, "Bhanuwat Swadsidsri"}, {67011725, "San Aung"}, {67011731, "Thiri Thaw"},
                                                    {67011755, "Phoo Pwint Cho Thar"},  {66010599, "Arhway Larhuna"}, {66010968, "Bowornthat Chiangthong"}, {66010998, "Cusson Laohapatanawong"},
                                                    {66010991, "Diyaan Pulikkal"}, {66011008, "Jirawatt Chimanee"}, {66011014, "Kant Isaranuchheep"}, {66011036, "Kongfah Sangchaisirisak"}, {66011702, "Naphat Umpa"}, {66011800, "Natavee Pecharat"},
                                                    {66011801, "Natchapon Sukthep"}, {66011805, "Nay Chi Shunn Lei"}, {66011807, "Nutthhapat Chaloemlarpsombut"}, {66011107, "Panchaya Wejchapinant"}, {66011131, "Pannatat Sribusarakkham"},
                                                    {66011174, "Pannawat Yorkhant"}, {66011123, "Parin Vessakosol"}, {66011147, "Phatdanai Khemanukul"}, {66011148, "Phathompol Siripichaiprom"},
                                                    {66011149, "Phatthadon Sornplang"}, {66011167, "Piraya Noorit"}, {66011177, "Rachata Phondi"}, {66011193, "Rachatawan Sudjarid"}, {66011203, "Sai Marn Pha"},
                                                    {66011211, "Sarita Manopatana"}, {66011215, "Satikit Tapbumrong"}, {66011217, "Shisa Klaysuban"}, {66011231, "Sorasich Lertwerasirikul"}, {66011244, "Syril Tuladhar"},
                                                    {66011245, "Tada Siangchin"}, {66011249, "Tanakrit Doltanakarn"}, {66011276, "Thunyaphon Chumkasian"}, {66011277, "Thuwanon Siddhichai"}, {66011288, "Vichaya Roongsiripornphol"},
                                                    {66011301, "Xanin Sae Ma"}, {66011525, "Audthanee Supeeramongkolkul"}, {66011533, "Eaint Kay Khaing Kyaw"}, {66011534, "Ei Myat Nwe"}, {66011564, "Panisara Yimcharoen"},
                                                    {66011580, "Phurirat Punmerod"}, {66011582, "Pongchanan Sriwanna"}, {66011606, "Thura Aung"}
        };

        // Begin transaction for faster insertion of multiple rows
        QSqlDatabase::database().transaction();

        bool success = true;

        // For each student in our sample data
        for (const auto &student : studentData) {
            // First check if this student ID already exists
            query.prepare("SELECT COUNT(*) FROM se_names_list WHERE student_id = :id");
            query.bindValue(":id", student.first);

            if (query.exec() && query.next() && query.value(0).toInt() == 0) {
                // Student ID doesn't exist, so insert it
                query.prepare("INSERT INTO se_names_list (student_id, name) VALUES (:id, :name)");
                query.bindValue(":id", student.first);
                query.bindValue(":name", student.second);

                if (!query.exec()) {
                    qDebug() << "Error inserting student data:" << query.lastError();
                    success = false;
                    break;
                }
            } else {
                // Student ID already exists, log it but continue with others
                qDebug() << "Student ID" << student.first << "already exists, skipping";
            }
        }

        if (success) {
            QSqlDatabase::database().commit();
            qDebug() << "Successfully populated se_names_list with initial data";
        } else {
            QSqlDatabase::database().rollback();
            qDebug() << "Failed to populate se_names_list, transaction rolled back";
        }

        return success;
    }


    // Add this method to fetch a student name from se_names_list
    static QString getStudentNameById(int studentId) {
        QSqlQuery query;
        query.prepare("SELECT name FROM se_names_list WHERE student_id = :id");
        query.bindValue(":id", studentId);

        if (query.exec() && query.next()) {
            return query.value(0).toString();
        }

        return QString(); // Return empty string if name not found

    }



};



#endif // DATABASE_H
