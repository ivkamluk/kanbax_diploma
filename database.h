#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QStringList>
#include <QVector>
#include <QPair>
#include <QVariantList>

/*
    this class provides all operations
    that should be done with database

    this app is using MySQL database
    that gives opportunity to make all online
*/

// with enum we can set what operations
// we would like to do in select
// when user Sign in, we don`t know
// what he was select : username or email
// so this, enum helps to get know what he was typed
enum class Field { Username = 0, Email };

class DataBase
{
public:
    DataBase();
    bool isConnected();
    void createConnection();
    void closeConnection();

    // INSERT VALUES

    void writeUserDataTUser(QString user_name, QString user_email,
                       QString user_password, QString user_salt, QString user_country);
    void writeUserDataTSession(QString user_id, QString location_id, QString device_id,
                               QString session_token, bool session_remember,
                               bool session_active);
    void writeUserDataTLocation(QString user_id);
    void writeUserDataTDevice(QString user_id);
    void writeUserDataTBoard(QString user_id, QString board_name, QString board_date_creation,
                             QString board_date_modification, bool board_notification_sound);

    void writeUserDataTTask(QString user_id, QString board_id, QString task_name, QString
                            task_description, unsigned short task_column);

    //SELECT VALUES

    // here @value@ can be user_name or user_email
    // @field@ -> detects if it`s Username or Email

    QStringList getUserDataTUser(QString value, Field field);
    QStringList getUserDataTUser(QString user_id);
    QStringList getUserDataTSession(QString value, Field field );

    // MAYBE HERE WE NEED TO HAVE VECTOR OR LIST OF QSTRINLIST
    QStringList getUserDataTLocation(QString user_id);
    // MAYBE HERE WE NEED TO HAVE VECTOR OR LIST OF QSTRINLIST

    QStringList getUserDataTDevice(QString user_id);
    QVector<QStringList> getUserOtherSessions(QString user_id, QString session_id);
    QStringList getUserCurrentSession(QString session_id);
    QStringList getUserHashAndSaltTUser(QString value, Field field);


    QVector<QStringList> getUserDataTBoardFCreated(QString user_id);
    QVector<QStringList> getUserDataTBoardFModified(QString user_id);
    QVector<QStringList> getUserDataTBoardFSound(QString user_id);
    QVector<QStringList> getUserDataTBoardFNoSound(QString user_id);
    QVector<QStringList> getUserDataTBoard(QString user_id,
                                           unsigned short boards_limit);
    QVector<QStringList> getUserDataTTaskToDo(QString user_id, QString board_id,
                                      unsigned short task_column);
    QVector<QStringList> getUserDataTTaskDoing(QString user_id, QString board_id);
    QVector<QVariantList> getUserDataTTaskDone(QString user_id, QString board_id,
                                      unsigned short task_column);
    // this function gives last added user board
    QString getUserCreatedBoardId(QString user_id);
    // function that gives last modified user board
    QString getUserModifiedBoardId(QString user_id);

    QStringList getUserLastAddedTask(QString user_id, QString board_id);

    QPair<QString, bool> getUserBoardNameNotification(QString user_id, QString board_id);

    bool getUserBoardAvailableTasks(QString user_id, QString board_id);

    QPair<QString,QString> getUserUpdatedTaskDoing(QString user_id, QString board_id, QString task_id);

    //UPDATE VALUES

    void updateUserName(QString user_id, QString user_name);
    void updateUserEmail(QString user_id, QString user_email);
    void updateUserPassword(QString user_id, QString user_password);
    void updateUserSalt(QString user_id, QString user_salt);
    void updateUserCountry(QString user_id, QString user_country);

    void updateSession(QString user_id, QString session_token);

    void updateBoardName(QString user_id, QString board_id, QString board_name);
    void updateBoardDateModification(QString user_id, QString board_id);
    void updateBoardNotificationSound(QString user_id, QString board_id,
                                      bool board_notification_sound);

    void updateTaskName(QString user_id, QString task_id, QString task_name);
    void updateTaskDescription(QString user_id, QString task_id, QString task_description);
    void updateTaskDatetime(QString user_id, QString board_id);
    void updateTaskColumnDone(QString user_id, QString board_id, QString task_id, unsigned short task_column, bool task_over_byuser);
    void updateTaskColumnDoing(QString user_id, QString board_id, QString task_id, QString task_start_time, QString task_end_time, unsigned short task_column);


    // ANOTHER DATABASE OPERATIONS

    // removes all available sessions not including current
    void removeOtherAvailableSessions(QString user_id, QString user_token);
    void removeCurrentSession(QString user_id, QString user_token);
    void removeAccount(QString user_id);

   // void removeUserTask(QString board_id);
   // void removeBoardAllTasks(QString )

    void removeUserBoard(QString board_id);
    void removeBoardAllTasks(QString user_id, QString board_id);
    void removeOneTask(QString user_id, QString task_id);

    // with this function we get to know
    // if user_name and user_token  on device and DB are the same
    // if not -> will be launched MainWindow (Sign in/up)
    // if all is good -> WorkWindow
    bool isRememberedRestore(QString user_name, QString user_token);
    bool checkMatchUsernameOrEmail(QString value, Field field);

    // this function give us to know if user session is stll available
    // we need to check if we have this session in database
    bool checkUserSession(QString user_id, QString user_token);

private:
    QSqlDatabase db;
};

#endif
