#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QString>
#include <QSettings>
#include <QStringList>
#include <QPoint>

#include "usersecurity.h"

/*
     this class helps to provide
     saving user data such as: user_name, user_token, window_resize...
*/

class AppSettings
{
public:
    AppSettings();

    // this function helps to set value for user_name and user_token
    // in settings: MAC -> ~Library\Preferences -> plist
    // Windows: HKEY_CURRENT_USER
    void saveUserSignName(QString user);
    void saveUserSignToken(QString token);
    void saveWindowPosition(QPoint position);
    void saveUserLanguage(unsigned int language);
    void saveOptionRandomColorTodo(bool random_color_todo);
    void saveNotificationSide(unsigned int notification_side);
    void saveRecentBoardsCount(unsigned int boards_count);
    void saveThemeColor(unsigned int theme);

    //function that get`s from specific file list of countries
    QStringList readCountriesFromFile(QString filename);

    // function returns user_name and user_token
    // that saved on device
    QStringList retrieveUserSignData();
    unsigned int getUserLanguage();
    bool getOptionRandomColorToDo();
    unsigned int getNotificationSide();
    unsigned int getRecentBoardsCount();
    unsigned int getThemeColor();

    QPoint retrieveWindowPosition();

private:
     unsigned short encrypting_value_user, encrypting_value_token;
     QSettings settings;
     UserSecurity security;
};

#endif
