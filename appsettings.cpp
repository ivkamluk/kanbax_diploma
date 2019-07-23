#include "appsettings.h"

#include <QSettings>
#include <QByteArray>
#include <QFile>

//delete later
#include <QDebug>

// this values is just random, so enjoy to set another
AppSettings::AppSettings() : encrypting_value_user(255),
                             encrypting_value_token(17)
{}

// this function helps to set value for user_name and user_token
// in settings: MAC -> ~Library\Preferences -> plist
// Windows: HKEY_CURRENT_USER
void AppSettings::saveUserSignName(QString user)
{
    // let`s hide data
    // we do this just only for hiding data from first look
    // this is not important to use hard ecnryption or hashing
    // because token are already hashed
    settings.setValue("user", security.simpleEncryptingData
                                     (user, encrypting_value_user));
}

void AppSettings::saveUserSignToken(QString token)
{
    // let`s hide data
    // we do this just only for hiding data from first look
    // this is not important to use hard ecnryption or hashing
    // because token are already hashed
    settings.setValue("token", security.simpleEncryptingData
                                    (token, encrypting_value_token));
}

void AppSettings::saveWindowPosition(QPoint position)
{
    settings.setValue("positionW", position);
}

void AppSettings::saveUserLanguage(unsigned int language)
{
    settings.setValue("language", language);
}

void AppSettings::saveOptionRandomColorTodo(bool random_color_todo)
{
    settings.setValue("rnd-color-todo", random_color_todo);
}

void AppSettings::saveNotificationSide(unsigned int notification_side)
{
    settings.setValue("notification-side", notification_side);
}

void AppSettings::saveRecentBoardsCount(unsigned int boards_count)
{
    settings.setValue("boards-count", boards_count);
}

void AppSettings::saveThemeColor(unsigned int theme)
{
    settings.setValue("theme-color", theme);
}

// function returns user_name and user_token
// that saved on device
QStringList AppSettings::retrieveUserSignData()
{
    QStringList user_sign;
    user_sign << security.simpleDecryptingData(
                     settings.value("user").toString(), encrypting_value_user);

    user_sign << security.simpleDecryptingData(
                     settings.value("token").toString(), encrypting_value_token);

    return user_sign;
}

QPoint AppSettings::retrieveWindowPosition()
{
    return settings.value("positionW").toPoint();
}

unsigned int AppSettings::getUserLanguage()
{
    return settings.value("language").toUInt();
}

bool AppSettings::getOptionRandomColorToDo()
{
    return settings.value("rnd-color-todo").toBool();
}

unsigned int AppSettings::getNotificationSide()
{
    return settings.value("notification-side").toUInt();
}

unsigned int AppSettings::getRecentBoardsCount()
{
    return settings.value("boards-count").toUInt();
}

unsigned int AppSettings::getThemeColor()
{
    return settings.value("theme-color").toUInt();
}

//function that get`s from specific file list of countries
// this parameter is depends from language that user works

//By this function we will read 3 different files
//(list_countries_en.txt/*_ru.txt/*_ua.txt)
QStringList AppSettings::readCountriesFromFile(QString namefile)
{
    QStringList stringlist_temp;
    QFile file(namefile);
    file.open(QFile::ReadOnly);

    QTextStream in(&file);
    QString string_temp;

    while(!in.atEnd()) {
        string_temp = in.readLine();
        stringlist_temp.push_back(string_temp);
    }
    file.close();

    return stringlist_temp;
}
