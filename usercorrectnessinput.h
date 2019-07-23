#ifndef USERCORRECTNESSINPUT_H
#define USERCORRECTNESSINPUT_H

#include <QObject>
#include <QPushButton>
#include <QString>

#include "database.h"

class UserCorrectnessInput : public QObject
{
    Q_OBJECT
public:
    explicit UserCorrectnessInput(QObject *parent = nullptr);

    static QString getCountBoards(int user_boards_count);
    void inputCheckCorrectLine(QString line_input, QPushButton *button);
    QString checkValidationUsername(QString user_name, DataBase &database);
    QString checkValidationPassword(QString user_password);
    QString checkValidationEmail(QString user_email, DataBase &database);
    bool isEmail(QString user_email);

signals:

public slots:

};

#endif
