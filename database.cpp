#include "database.h"
#include "networkdata.h"

#include <QSqlQuery>
#include <QDateTime>
#include <QSysInfo>
#include <QSqlRecord>

/*
    this class provides all operations
    that should be done with database

    this app is using MySQL database
    that gives opportunity to make all online
*/

//delete later
#include <QDebug>

DataBase::DataBase()
{}

bool DataBase::isConnected()
{
    return db.open() ? true : false;
}

void DataBase::createConnection()
{
  // don`t forget to put here your data
    db = QSqlDatabase::addDatabase("?");
    db.setHostName("?");
    db.setDatabaseName("?");
    db.setUserName("?");
    db.setPassword("?");
}

void DataBase::closeConnection()
{
   db.close();
}

// INSERT VALUES

void DataBase::writeUserDataTUser(QString user_name, QString user_email,
                              QString user_password, QString user_salt, QString user_country)
{
    QSqlQuery query;
    query.prepare("Insert Into User (User.user_name, User.user_email,"
                  "User.user_password, User.user_salt, User.user_country) Values"
                  "(:value_name, :value_email, :value_password,"
                  ":value_salt, :value_country)");
    query.bindValue(":value_name", user_name);
    query.bindValue(":value_email", user_email);
    query.bindValue(":value_password", user_password);
    query.bindValue(":value_salt", user_salt);
    query.bindValue(":value_country", user_country);
    query.exec();
}

void DataBase::writeUserDataTSession(QString user_id, QString location_id, QString device_id,
                                    QString session_token, bool session_remember,
                                    bool session_active)
{
    QSqlQuery query;
    query.prepare("Insert Into Session (Session.user_id_FK, Session.location_id_FK,"
                  "Session.device_id_FK, Session.session_date, Session.session_token, "
                  "Session.session_remember, Session.session_active) Values (:value_user_id, "
                  ":value_location_id, :value_device_id, :value_date,  :value_token, "
                  ":value_remember, :value_active)");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_location_id", location_id);
    query.bindValue(":value_device_id", device_id);
    query.bindValue(":value_date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":value_token", session_token);
    query.bindValue(":value_remember", session_remember);
    query.bindValue(":value_active", session_active);
    query.exec();
}

void DataBase::writeUserDataTLocation(QString user_id)
{
    QSqlQuery query;
    NetworkData network;
    query.prepare("Insert Into Location (Location.user_id_FK, Location.location_ip,"
                  "Location.location_city, Location.location_country) Values"
                  "(:value_user_id, :value_location_ip, :value_location_city,"
                  ":value_location_country)");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_location_ip", network.getUserIp());
    query.bindValue(":value_location_city", network.getUserCity());
    query.bindValue(":value_location_country", network.getUserCountry());
    query.exec();
}

void DataBase::writeUserDataTDevice(QString user_id)
{
    QSqlQuery query;
    query.prepare("Insert Into Device (Device.user_id_FK, Device.device_name,"
                  " Device.device_OS) Values (:value_user_id, :value_device_name, "
                  ":value_device_OS)");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_device_name", QSysInfo::machineHostName());
    query.bindValue(":value_device_OS", QSysInfo::prettyProductName());
    query.exec();
}

void DataBase::writeUserDataTBoard(QString user_id, QString board_name,
                                   QString board_date_creation, QString board_date_modification,
                                   bool board_notification_sound)
{
    QSqlQuery query;
    query.prepare("Insert Into Board (Board.user_id_FK, Board.board_name, "
                  "Board.board_date_creation, Board.board_date_modification, "
                  "Board.board_notification_sound) Values (:value_user_id, :value_board_name,"
                  ":value_board_date_creation, :value_board_date_modification,"
                  ":value_board_notification_sound)");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_board_name", board_name);
    query.bindValue(":value_board_date_creation", board_date_creation);
    query.bindValue(":value_board_date_modification", board_date_modification);
    query.bindValue(":value_board_notification_sound", board_notification_sound);
    query.exec();
}

void DataBase::writeUserDataTTask(QString user_id, QString board_id, QString task_name, QString task_description, unsigned short task_column)
{
    QSqlQuery query;
    query.prepare("Insert Into Task (Task.user_id_FK, Task.board_id_FK, Task.task_name, Task.task_description, Task.task_column) Values (:value_user_id, :value_board_id, "
                  ":value_task_name, :value_task_description, :value_task_column)");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_board_id", board_id);
    query.bindValue(":value_task_name", task_name);
    query.bindValue(":value_task_description", task_description);
    query.bindValue(":value_task_column", task_column);
    query.exec();
}

  //SELECT VALUES
// better to get data in QStringList, because we get all the data and we don`t need
// everytime get acess to our database and get from there data
// we trying to get once most important and do something with them
// it`s will help optimize speed of app
// with no often request to our database and queries

QStringList DataBase::getUserDataTUser(QString value, Field field)
{
    // find user_data (user_id, user_name, user_email, user_country)
    // password and salt (we don`t need) and query for this must be minimized
    QSqlQuery query;
    switch(field)
    {
        case (Field::Username):
        query.prepare("Select User.user_id, User.user_name, User.user_email,"
                      " User.user_country From User Where BINARY user_name = :value");
        break;
        case (Field::Email):
        query.prepare("Select User.user_id, User.user_name, User.user_email, "
                      "User.user_country From User Where BINARY user_email = :value");
        break;
    };
    query.bindValue(":value", value);
    query.exec();
    QStringList result;
    while(query.next())
    {
         // 0 -user_id  //  1 - user_name // 2 - user_email // 3 - user_country
        for (unsigned short i=0; i<query.record().count(); i++)
            result << query.value(i).toString();
    }
    return result;
}

QStringList DataBase::getUserDataTUser(QString user_id)
{
    // find user_data (user_id, user_name, user_email, user_country)
    // password and salt (we don`t need) and query for this must be minimized
    QSqlQuery query;
    query.prepare("Select User.user_id, User.user_name, User.user_email,"
                  " User.user_country From User Where BINARY user_id = :value_user_id");
    query.bindValue(":value_user_id", user_id);
    query.exec();
    QStringList result;
    while(query.next())
    {
         // 0 -user_id  //  1 - user_name // 2 - user_email // 3 - user_country
        for (unsigned short i=0; i<query.record().count(); i++)
            result << query.value(i).toString();
    }
    return result;
}

QStringList DataBase::getUserDataTSession(QString value, Field field)
{
    QSqlQuery query;
    switch(field)
    {
        case (Field::Username):
        query.prepare("Select Session.session_id, Session.user_id_FK, "
                    "Session.location_id_FK, Session.device_id_FK,  Session.session_date,"
      " Session.session_token, Session.session_remember, Session.session_active "
      "From Session, User, Device WHERE Session.user_id_FK = User.user_id AND "
      "User.user_name = :value AND Device.device_id = Session.device_id_FK AND "
      "Device.device_name = :value_device_name AND Device.device_OS = :value_device_OS "
      "ORDER BY Session.session_date DESC LIMIT 1");
        break;
        case (Field::Email):
        query.prepare("Select Session.session_id, Session.user_id_FK, "
                      "Session.location_id_FK, Session.device_id_FK,  Session.session_date,"
        " Session.session_token, Session.session_remember, Session.session_active "
        "From Session, User, Device WHERE Session.user_id_FK = User.user_id AND "
        "User.user_email = :value AND Device.device_id = Session.device_id_FK AND "
        "Device.device_name = :value_device_name AND Device.device_OS = :value_device_OS "
        "ORDER BY Session.session_date DESC LIMIT 1");
        break;
    };
    query.bindValue(":value", value);
    query.bindValue(":value_device_name", QSysInfo::machineHostName());
    query.bindValue(":value_device_OS", QSysInfo::prettyProductName());
    query.exec();
    QStringList result;
    while(query.next())
    {
         // 0 - session_id, // 1 - user_id // 2- location_id // 3 - device_id
        // 4 - session_date // 5 - session_token // 6 - session_remember // 7 - session_active
        for (unsigned short i=0; i<query.record().count(); i++)
            result<< query.value(i).toString();
    }
    return result;
}

 // MAYBE HERE WE NEED TO HAVE VECTOR OR LIST OF QSTRINLIST

QStringList DataBase::getUserDataTLocation(QString user_id)
{
    // find user_location (user_id, location_ip, location_city, location_country)
    QSqlQuery query;
    query.prepare("Select Location.location_id, Location.location_ip, Location.location_city,"
                  " Location.location_country From Location INNER JOIN User ON "
                  "Location.user_id_FK = User.user_id WHERE User.user_id = :value_user_id "
                  "ORDER BY Location.location_id DESC LIMIT 1");
    query.bindValue(":value_user_id", user_id);
    query.exec();
    QStringList result;
    while(query.next())
    {
         // 0 - location_id  //  1 - location_ip // 2 - location_city // 3 - location_country
        for (unsigned short i=0; i<query.record().count();i++)
            result <<query.value(i).toString();
    }
    return result;
}
 // MAYBE HERE WE NEED TO HAVE VECTOR OR LIST OF QSTRINLIST

QStringList DataBase::getUserDataTDevice(QString user_id)
{
    // find user_device (user_id, device_name, device_OS)
    QSqlQuery query;
    query.prepare("Select Device.device_id, Device.device_name, Device.device_OS"
                  " From Device INNER JOIN User ON Device.user_id_FK = User.user_id"
                  " WHERE User.user_id = :value_user_id ORDER BY Device.device_id "
                  "DESC LIMIT 1");
    query.bindValue(":value_user_id", user_id);
    query.exec();
    QStringList result;
    while(query.next())
    {
         // 0 - device_id  //  1 - device_name // 2 - device_OS
        for (unsigned short i=0; i<query.record().count();i++)
            result <<query.value(i).toString();
    }
    return result;
}

 // other sessions -> that means another app working on another device
QVector<QStringList> DataBase::getUserOtherSessions(QString user_id, QString session_id)
{
    QSqlQuery query;
    query.prepare("Select Location.location_ip, Location.location_city, "
                  "Location.location_country, Device.device_OS, Device.device_name, "
                  "Session.session_date From Location, Device, Session Where "
                  "Location.location_id = Session.location_id_FK AND "
                  "Device.device_id = Session.device_id_FK "
                  "AND Session.user_id_FK = :value_user_id AND Session.session_active ='1' AND"
                  " Session.session_id NOT IN (:value_session_id)"
                  "ORDER BY Session.session_date DESC");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_session_id", session_id);
    query.exec();
    QVector<QStringList> result;
    unsigned long long i=0;
    // now we allocate memory for this boards
    result.resize(query.size());
    // here we get data, just like from 2D QVector
    while(query.next())
    {
        // 0 - location_ip // 1 - location_city // 2 - location_country // 3- device_OS
       // 4 - device_name // 5 - session_date
        for (unsigned long long j=0; j<query.record().count(); j++)
          result[i].insert(j, query.value(j).toString()); // with at-> not working
        i++;
    }

    return result;
}

QStringList DataBase::getUserCurrentSession(QString session_id)
{
    // find user_device (user_id, device_name, device_OS)
    QSqlQuery query;
    query.prepare("Select Location.location_ip, Location.location_city, "
                  "Location.location_country, Device.device_OS, Device.device_name, "
                  " Session.session_date From Location, Device, "
                  "Session Where Location.location_id = "
                  "Session.location_id_FK AND Device.device_id = Session.device_id_FK "
                  "AND Session.session_id = :value_session_id");
    query.bindValue(":value_session_id", session_id);
    query.exec();
    QStringList result;
    while(query.next())
    {
        // 0 - location_ip // 1 - location_city // 2 - location_country // 3- device_OS
       // 4 - device_name // 5 - session_date
        for (unsigned short i=0; i<query.record().count();i++)
            result <<query.value(i).toString();
    }
    return result;
}

// we need to get user hashed password
// because only with that we can compare
// if password that user input in sign in
// is equal to that we have in DB
QStringList DataBase::getUserHashAndSaltTUser(QString value, Field field)
{
    QSqlQuery query;
    switch(field)
    {
        case (Field::Email):
        query.prepare("Select User.user_password, User.user_salt From User Where"
                      " BINARY User.user_email = :value");
        break;
        case (Field::Username):
        query.prepare("Select User.user_password, User.user_salt From User Where"
                      " BINARY User.user_name = :value");
        break;
    };
    query.bindValue(":value", value);
    query.exec();
    QStringList user_hash_salt;
    while(query.next())
    {          // 0 - user_hash  // 1 - user_salt
        user_hash_salt <<  query.value(0).toString();
        user_hash_salt << query.value(1).toString();
    }
    return user_hash_salt;
}


QVector<QStringList> DataBase::getUserDataTBoardFCreated(QString user_id)
{
    QVector<QStringList> user_boards;
    QSqlQuery query;
    query.prepare("Select Board.* from Board, User Where Board.user_id_FK = User.user_id AND"
                  " user_id_FK = :value_user_id ORDER BY Board.board_date_creation DESC");
    query.bindValue(":value_user_id", user_id);
    query.exec();

    unsigned long long i=0;
    // now we allocate memory for this boards
    user_boards.resize(query.size());
    // here we get data, just like from 2D QVector
    while(query.next())
    {
        // 0 - board_id // 1 - user_id // 2 - board_name // 3 - board_date_creation
        // 4 - board_date_modification // 5 - board_notification_sound
        for (unsigned long long j=0; j<query.record().count(); j++)
          user_boards[i].insert(j, query.value(j).toString()); // with at-> not working
        i++;
    }
    return user_boards;
}

QVector<QStringList> DataBase::getUserDataTBoardFModified(QString user_id)
{
    QVector<QStringList> user_boards;
    QSqlQuery query;
    query.prepare("Select Board.* from Board, User Where Board.user_id_FK = User.user_id AND"
                  " user_id_FK = :value_user_id ORDER BY Board.board_date_modification DESC");
    query.bindValue(":value_user_id", user_id);
    query.exec();

    unsigned long long i=0;
    // now we allocate memory for this boards
    user_boards.resize(query.size());
    // here we get data, just like from 2D QVector
    while(query.next())
    {
        // 0 - board_id // 1 - user_id // 2 - board_name // 3 - board_date_creation
        // 4 - board_date_modification // 5 - board_notification_sound
        for (unsigned long long j=0; j<query.record().count(); j++)
          user_boards[i].insert(j, query.value(j).toString()); // with at-> not working
        i++;
    }
    return user_boards;
}

QVector<QStringList> DataBase::getUserDataTBoardFSound(QString user_id)
{
    QVector<QStringList> user_boards;
    QSqlQuery query;
    query.prepare("Select Board.* from Board, User Where Board.user_id_FK = User.user_id AND"
                  " user_id_FK = :value_user_id AND board_notification_sound = '1' ORDER BY"
                  " Board.board_date_creation DESC");
    query.bindValue(":value_user_id", user_id);
    query.exec();

    unsigned long long i=0;
    // now we allocate memory for this boards
    user_boards.resize(query.size());
    // here we get data, just like from 2D QVector
    while(query.next())
    {
        // 0 - board_id // 1 - user_id // 2 - board_name // 3 - board_date_creation
        // 4 - board_date_modification // 5 - board_notification_sound
        for (unsigned long long j=0; j<query.record().count(); j++)
          user_boards[i].insert(j, query.value(j).toString()); // with at-> not working
        i++;
    }
    return user_boards;
}

QVector<QStringList> DataBase::getUserDataTBoardFNoSound(QString user_id)
{
    QVector<QStringList> user_boards;
    QSqlQuery query;
    query.prepare("Select Board.* from Board, User Where Board.user_id_FK = User.user_id AND"
                  " user_id_FK = :value_user_id AND board_notification_sound = '0' ORDER BY"
                  " Board.board_date_creation DESC");
    query.bindValue(":value_user_id", user_id);
    query.exec();

    unsigned long long i=0;
    // now we allocate memory for this boards
    user_boards.resize(query.size());
    // here we get data, just like from 2D QVector
    while(query.next())
    {
        // 0 - board_id // 1 - user_id // 2 - board_name // 3 - board_date_creation
        // 4 - board_date_modification // 5 - board_notification_sound
        for (unsigned long long j=0; j<query.record().count(); j++)
          user_boards[i].insert(j, query.value(j).toString()); // with at-> not working
        i++;
    }
    return user_boards;
}

QVector<QStringList> DataBase::getUserDataTBoard(QString user_id, unsigned short boards_limit)
{
    QVector<QStringList> user_boards;
    QSqlQuery query;
    query.prepare("Select Board.* from Board, User Where Board.user_id_FK = User.user_id AND"
                  " user_id_FK = :value_user_id ORDER BY Board.board_date_modification DESC ");
    query.bindValue(":value_user_id", user_id);
    query.exec();

    // we doing this, because if we have set in boards limits
    // more than we have boards, then we will get number of boards
    // from which, we have created
    // at result we get number from query.size()
    if (query.size() < boards_limit)
        boards_limit = query.size();

   // now we allocate memory for this boards
    user_boards.resize(boards_limit);
    // here we get data, just like from 2D QVector
    unsigned short i=0;
            while(query.next())
            {
                if (i == boards_limit) break;
                // 0 - board_id // 1 - user_id // 2 - board_name // 3 - board_date_creation
                // 4 - board_date_modification // 5 - board_notification_sound
                    for (unsigned short j=0; j<query.record().count(); j++)
                       user_boards[i].append(query.value(j).toString()); // with at-> not working
                i++;
            }
    return user_boards;
}

QVector<QStringList> DataBase::getUserDataTTaskToDo(QString user_id, QString board_id,
     unsigned short task_column)
{
    QVector<QStringList> user_tasks;
    QSqlQuery query;
    query.prepare("Select Task.task_id, Task.task_name, Task.task_description"
                  " From Task Where Task.user_id_FK =:value_user_id AND"
                  " Task.board_id_FK =:value_board_id AND Task.task_column =:value_task_column "
                  "ORDER BY Task.task_id DESC");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_board_id", board_id);
    query.bindValue(":value_task_column", task_column);
    query.exec();

    unsigned long long i=0;
    // now we allocate memory for this tasks
    user_tasks.resize(query.size());
    // here we get data, just like from 2D QVector
    while(query.next())
    {
           // 0 - task_id // 1 - task_name // 2 - task_description
        for (unsigned long long j=0; j<query.record().count(); j++)
          user_tasks[i].insert(j, query.value(j).toString()); // with at-> not working
        i++;
    }
    return user_tasks;
}

QVector<QStringList> DataBase::getUserDataTTaskDoing(QString user_id, QString board_id)
{
    QVector<QStringList> user_tasks;
    QSqlQuery query;
    query.prepare("Select Task.task_id, Task.task_name, Task.task_description,"
                  " Task.task_start_time, Task.task_end_time From Task Where Task.user_id_FK =:value_user_id AND Task.board_id_FK =:value_board_id AND Task.task_column = 2 ORDER BY Task.task_end_time ASC");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_board_id", board_id);
    query.exec();

    unsigned long long i=0;
    // now we allocate memory for this tasks
    user_tasks.resize(query.size());
    // here we get data, just like from 2D QVector
    while(query.next())
    {
           // 0 - task_id // 1 - task_name // 2 - task_description //  3 - task_start_time
        // 4 - task_end_time
        for (unsigned long long j=0; j<query.record().count(); j++)
          user_tasks[i].insert(j, query.value(j).toString()); // with at-> not working
        i++;
    }
    return user_tasks;
}

QVector<QVariantList> DataBase::getUserDataTTaskDone(QString user_id, QString board_id,
     unsigned short task_column)
{
    QVector<QVariantList> user_tasks;
    QSqlQuery query;
    query.prepare("Select Task.task_id, Task.task_name, Task.task_description, "
                  "Task.task_over_byuser From Task Where Task.user_id_FK =:value_user_id AND"
                  " Task.board_id_FK =:value_board_id AND Task.task_column =:value_task_column ");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_board_id", board_id);
    query.bindValue(":value_task_column", task_column);
    query.exec();

    unsigned long long i=0;
    // now we allocate memory for this tasks
    user_tasks.resize(query.size());
    // here we get data, just like from 2D QVector
    while(query.next())
    {
           // 0 - task_id // 1 - task_name // 2 - task_description // 3 - task_over_byuser
        for (unsigned long long j=0; j<query.record().count(); j++)
        {
          if (j == 3) // task_over_byuser -> must be bool
              user_tasks[i].insert(j, query.value(j).toBool());
          else
              user_tasks[i].insert(j, query.value(j).toString()); // with at-> not working
        }
        i++;
    }
    return user_tasks;
}

// this function gives last added user board
QString DataBase::getUserCreatedBoardId(QString user_id)
{
    QSqlQuery query;
    query.prepare("Select Board.board_id From Board, User Where Board.user_id_FK = User.user_id"
                  " AND User.user_id = :value_user_id ORDER BY Board.board_date_creation DESC LIMIT 1");
    query.bindValue(":value_user_id", user_id);
    query.exec();
    QString board_id;
    while(query.next())
            board_id = query.value(0).toString();
    return board_id;
}

// function that gives last modified user board
QString DataBase::getUserModifiedBoardId(QString user_id)
{
    QSqlQuery query;
    query.prepare("Select Board.board_id From Board, User Where Board.user_id_FK = User.user_id And"
                  " User.user_id = :value_user_id ORDER BY Board.board_date_modification DESC LIMIT 1");
    query.bindValue(":value_user_id", user_id);
    query.exec();
    QString board_id;
    while(query.next())
            board_id = query.value(0).toString();
    return board_id;
}

QStringList DataBase::getUserLastAddedTask(QString user_id, QString board_id)
{
    QSqlQuery query;
    query.prepare("Select Task.task_id, Task.task_name, Task.task_description"
                  " FROM Task Where Task.user_id_FK =:value_user_id"
                  " AND Task.board_id_FK =:value_board_id AND Task.task_column = '1' "
                  "ORDER BY Task.task_id DESC LIMIT 1");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_board_id", board_id);
    query.exec();
    QStringList task;
    while(query.next())       // 0 - task_id  1 - task_name // 2 - task_description
            task << query.value(0).toString() << query.value(1).toString() <<
                    query.value(2).toString();
    return task;
}

QPair<QString, bool> DataBase::getUserBoardNameNotification(QString user_id, QString board_id)
{
    QSqlQuery query;
    query.prepare("Select Board.board_name, Board.board_notification_sound From Board"
                  " Where Board.board_id =:value_board_id AND Board.user_id_FK =:value_user_id");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_board_id", board_id);
    query.exec();

    QPair<QString, bool> user_board;
    while(query.next())
    {
            user_board.first =  query.value(0).toString();
            user_board.second = query.value(1).toBool();
    }
    return user_board;
}

bool DataBase::getUserBoardAvailableTasks(QString user_id, QString board_id)
{
    QSqlQuery query;
    query.prepare("Select Task.task_id From Task Where"
                  " Task.user_id_FK =:value_user_id AND Task.board_id_FK =:value_board_id LIMIT 1");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_board_id", board_id);
    query.exec();

    bool tasks_available = false;
    while(query.next())
            tasks_available =  query.value(0).toBool();
    return tasks_available;
}

QPair<QString, QString> DataBase::getUserUpdatedTaskDoing(QString user_id, QString board_id, QString task_id)
{
    QSqlQuery query;
    query.prepare("Select Task.task_start_time, Task.task_end_time From Task "
                  "Where Task.user_id_FK=:value_user_id AND Task.board_id_FK =:value_board_id "
                  "AND Task.task_id=:value_task_id");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_board_id", board_id);
    query.bindValue(":value_task_id", task_id);
    query.exec();

   QPair<QString, QString> updated_task;
    while(query.next())
    {
            updated_task.first =  query.value(0).toString();
            updated_task.second = query.value(1).toString();
    }
    return updated_task;
}

//UPDATE VALUES

void DataBase::updateUserName(QString user_id, QString user_name)
{
    QSqlQuery query;
    query.prepare("Update User Set User.user_name = :value_user_name WHERE "
                  "User.user_id = :value_user_id");
    query.bindValue(":value_user_name", user_name);
    query.bindValue(":value_user_id", user_id);
    query.exec();
}

void DataBase::updateUserEmail(QString user_id, QString user_email)
{
    QSqlQuery query;
    query.prepare("Update User Set User.user_email = :value_user_email WHERE "
                  "User.user_id = :value_user_id");
    query.bindValue(":value_user_email", user_email);
    query.bindValue(":value_user_id", user_id);
    query.exec();
}

void DataBase::updateUserPassword(QString user_id, QString user_password)
{
    QSqlQuery query;
    query.prepare("Update User Set User.user_password = :value_user_password WHERE "
                  "User.user_id = :value_user_id");
    query.bindValue(":value_user_password", user_password);
    query.bindValue(":value_user_id", user_id);
    query.exec();
}

void DataBase::updateUserSalt(QString user_id, QString user_salt)
{
    QSqlQuery query;
    query.prepare("Update User Set User.user_salt = :value_user_salt WHERE "
                  "User.user_id = :value_user_id");
    query.bindValue(":value_user_salt", user_salt);
    query.bindValue(":value_user_id", user_id);
    query.exec();
}

void DataBase::updateUserCountry(QString user_id, QString user_country)
{
    QSqlQuery query;
    query.prepare("Update User Set User.user_country = :value_user_country WHERE "
                  "User.user_id = :value_user_id");
    query.bindValue(":value_user_country", user_country);
    query.bindValue(":value_user_id", user_id);
    query.exec();
}

void DataBase::updateSession(QString user_id, QString session_token)
{
    // update all values in : location, device, session
    // location updates: location_ip, location_city, location_country
    // device updates: device_name, device_OS
    // session updates: session_time
    QSqlQuery query;
    NetworkData network;
    query.prepare("Update Location, Device, Session Set Location.location_ip = :value_location_ip, "
                  "Location.location_city = :value_location_city, Location.location_country = "
                  ":value_location_country, Device.device_name = :value_device_name, "
                  "Device.device_OS = :value_device_OS, Session.session_date = :value_session_date"
                  " WHERE Session.location_id_FK = Location.location_id AND Session.device_id_FK ="
                  " Device.device_id AND Session.user_id_FK = :value_user_id AND Session.session_token ="
                  " :value_session_token");
    query.bindValue(":value_location_ip", network.getUserIp());
    query.bindValue(":value_location_city", network.getUserCity());
    query.bindValue(":value_location_country", network.getUserCountry());
    query.bindValue(":value_device_name", QSysInfo::machineHostName());
    query.bindValue(":value_device_OS", QSysInfo::prettyProductName());
    query.bindValue(":value_session_date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":value_device_OS", QSysInfo::prettyProductName());
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_session_token", session_token);
    query.exec();
}

void DataBase::updateBoardDateModification(QString user_id, QString board_id)
{
    QSqlQuery query;
    query.prepare("Update Board Set board_date_modification =:value_datetime Where Board.board_id =:value_board_id"
                  " AND Board.user_id_FK =:value_user_id");
    query.bindValue(":value_datetime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_board_id", board_id);
    query.exec();
}

void DataBase::updateBoardName(QString user_id, QString board_id, QString board_name)
{
    QSqlQuery query;
    query.prepare("Update Board Set Board.board_name =:value_board_name Where Board.board_id =:value_board_id AND"
                  " Board.user_id_FK =:value_user_id");
    query.bindValue(":value_board_name", board_name);
    query.bindValue(":value_board_id", board_id);
    query.bindValue(":value_user_id", user_id);
    query.exec();
}

void DataBase::updateBoardNotificationSound(QString user_id, QString board_id,
       bool board_notification_sound)
{
    QSqlQuery query;
    query.prepare("Update Board Set Board.board_notification_sound = :value_board_notification Where Board.board_id"
                  " =:value_board_id AND Board.user_id_FK =:value_user_id");
    query.bindValue(":value_board_notification", board_notification_sound);
    query.bindValue(":value_board_id", board_id);
    query.bindValue(":value_user_id", user_id);
    query.exec();
}

void DataBase::updateTaskName(QString user_id, QString task_id, QString task_name)
{
    QSqlQuery query;
    query.prepare("Update Task Set Task.task_name = :value_task_name Where Task.task_id =:value_task_id AND"
                  " Task.user_id_FK =:value_user_id");
    query.bindValue(":value_task_name", task_name);
    query.bindValue(":value_task_id", task_id);
    query.bindValue(":value_user_id", user_id);
    query.exec();
}

void DataBase::updateTaskDescription(QString user_id, QString task_id, QString task_description)
{
    QSqlQuery query;
    query.prepare("Update Task Set Task.task_description = :value_task_description Where Task.task_id"
                  " =:value_task_id AND Task.user_id_FK =:value_user_id");
    query.bindValue(":value_task_description", task_description);
    query.bindValue(":value_task_id", task_id);
    query.bindValue(":value_user_id", user_id);
    query.exec();
}

void DataBase::updateTaskDatetime(QString user_id, QString board_id)
{
    QSqlQuery query;
    query.prepare("Update Task Set Task.task_time =:value_task_time Where Task.board_id_FK =:value_board_id"
                  " AND Task.user_id_FK =:value_user_id");
    query.bindValue(":value_task_time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":value_board_id", board_id);
    query.bindValue(":value_user_id", user_id);
    query.exec();
}

void DataBase::updateTaskColumnDone(QString user_id, QString board_id, QString task_id, unsigned short task_column,
                                bool task_over_byuser)
{
    QSqlQuery query;
    query.prepare("Update Task Set Task.task_column =:value_task_column, Task.task_over_byuser =:value_task_over "
                  "Where Task.board_id_FK =:value_board_id AND Task.user_id_FK =:value_user_id AND Task.task_id =:value_task_id");
    query.bindValue(":value_task_column", task_column);
    query.bindValue(":value_task_over", task_over_byuser);
    query.bindValue(":value_board_id", board_id);
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_task_id", task_id);
    query.exec();
}

void DataBase::updateTaskColumnDoing(QString user_id, QString board_id, QString task_id, QString task_start_time,
                                     QString task_end_time, unsigned short task_column)
{
    QSqlQuery query;
    query.prepare("Update Task Set Task.task_column =:value_task_column, Task.task_start_time =:value_start_time, "
                  "Task.task_end_time=:value_end_time Where Task.board_id_FK =:value_board_id AND Task.user_id_FK =:value_user_id "
                  "AND Task.task_id =:value_task_id");
    query.bindValue(":value_task_column", task_column);
    query.bindValue(":value_board_id", board_id);
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_task_id", task_id);
    query.bindValue(":value_start_time", task_start_time);
    query.bindValue(":value_end_time", task_end_time);
    query.exec();
}

  // ANOTHER DATABASE OPERATIONS

 // removes all available sessions not including current
void DataBase::removeOtherAvailableSessions(QString user_id, QString user_token)
{
    QSqlQuery query;
    query.prepare("Delete From Location, Session, Device USING Location INNER JOIN Session "
                  "INNER JOIN Device WHERE Location.location_id = Session.location_id_FK AND "
                  "Device.device_id = Session.device_id_FK AND Session.session_active = '1' "
                  "AND Session.user_id_FK = :value_user_id AND Session.session_token"
                  " NOT IN (:value_user_token)");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_user_token", user_token);
    query.exec();
}

void DataBase::removeCurrentSession(QString user_id, QString user_token)
{
    QSqlQuery query;
    query.prepare("Delete From Location, Session, Device USING Location INNER JOIN Session "
                  "INNER JOIN Device WHERE Location.location_id = Session.location_id_FK AND "
                  "Device.device_id = Session.device_id_FK AND Session.session_active = '1' "
                  "AND Session.user_id_FK = :value_user_id AND "
                  "Session.session_token = :value_user_token");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_user_token", user_token);
    query.exec();
}

void DataBase::removeAccount(QString user_id)
{
    QSqlQuery query;
    query.prepare("Delete From Device WHERE Device.user_id_FK = :value_user_id");
    query.bindValue(":value_user_id", user_id);
    query.exec();

    query.prepare("Delete From Board WHERE Board.user_id_FK = :value_user_id");
    query.bindValue(":value_user_id", user_id);
    query.exec();

    query.prepare("Delete From Location WHERE Location.user_id_FK= :value_user_id");
    query.bindValue(":value_user_id", user_id);
    query.exec();

    query.prepare("Delete From Session WHERE Session.user_id_FK = :value_user_id");
    query.bindValue(":value_user_id", user_id);
    query.exec();

    query.prepare("Delete From Task WHERE Task.user_id_FK = :value_user_id");
    query.bindValue(":value_user_id", user_id);
    query.exec();

    query.prepare("Delete From User WHERE User.user_id = :value_user_id");
    query.bindValue(":value_user_id", user_id);
    query.exec();
}

void DataBase::removeUserBoard(QString board_id)
{
    QSqlQuery query;
    query.prepare("Delete From Board Where Board.board_id = :value_board_id");
    query.bindValue(":value_board_id", board_id);
    query.exec();
}

void DataBase::removeBoardAllTasks(QString user_id, QString board_id)
{
    QSqlQuery query;
    query.prepare("Delete From Task Where Task.user_id_FK =:value_user_id"
                  " AND Task.board_id_FK =:value_board_id");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_board_id", board_id);
    query.exec();
}

void DataBase::removeOneTask(QString user_id, QString task_id)
{
    QSqlQuery query;
    query.prepare("Delete From Task Where Task.user_id_FK = :value_user_id"
                  " AND Task.task_id = :value_task_id");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_task_id", task_id);
    query.exec();
}

// with this function we get to know
// if user_name and user_token  on device and DB are the same
// if not -> will be launched MainWindow (Sign in/up)
// if all is good -> WorkWindow
bool DataBase::isRememberedRestore(QString user_name, QString user_token)
{
    QSqlQuery query;
    query.prepare("Select Session.session_remember From User, Session, Device Where BINARY"
                  " User.user_name = :value_name AND Session.session_token = :value_token AND"
                  " Device.device_name = :value_device_name AND "
                  "Device.device_OS = :value_device_OS ORDER BY Session.session_id DESC LIMIT 1");
    query.bindValue(":value_name", user_name);
    query.bindValue(":value_token", user_token);
    query.bindValue(":value_device_name", QSysInfo::machineHostName());
    query.bindValue(":value_device_OS", QSysInfo::prettyProductName());
    query.exec();
    // for default we will have 0
    // if query will be empty
    // than return false
    unsigned short remember=0; // we have only 0 or 1
     while(query.next())
        remember = query.value(0).toInt();

    return remember == 0 ? false : true;
}

bool DataBase::checkMatchUsernameOrEmail(QString value, Field field)
{
    QSqlQuery query;
    switch(field)
    {
        case (Field::Username):
        query.prepare("Select User.user_name From User Where BINARY User.user_name = :value");
        break;
        case (Field::Email):
        query.prepare("Select User.user_email From User Where BINARY User.user_email = :value");
        break;
    };
    query.bindValue(":value", value);
    query.exec();
    return query.size()>0?true:false;
}

// this function give us to know if user session is stll available
// we need to check if we have this session in database
bool DataBase::checkUserSession(QString user_id, QString user_token)
{
    QSqlQuery query;
    query.prepare("Select Session.session_id From Session WHERE"
                  " Session.user_id_FK = :value_user_id AND Session.session_token = "
                  ":value_user_token");
    query.bindValue(":value_user_id", user_id);
    query.bindValue(":value_user_token", user_token);
    query.exec();

    return query.size() > 0 ? true : false;
}

