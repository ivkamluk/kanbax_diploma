#include "usercorrectnessinput.h"

#include <QRegularExpression>
#include <QRegExp>

UserCorrectnessInput::UserCorrectnessInput(QObject *parent) : QObject(parent)
{}

QString UserCorrectnessInput::getCountBoards(int user_boards_count)
{
    QString total_boards_data;

    if (user_boards_count > 1)
       total_boards_data.append(tr("%1 boards").arg(QString::number(user_boards_count)));
     else
       total_boards_data.append(tr("%1 board").arg(QString::number(user_boards_count)));

  return total_boards_data;
}

void UserCorrectnessInput::inputCheckCorrectLine(QString line_input, QPushButton *button)
{
    QRegularExpression reg("^\\s");
    // we check if line is not empty and it not contins space at the beginning
    if (!line_input.isEmpty() && !line_input.contains(reg))
        button->setEnabled(true);
    else
        button->setEnabled(false);
}

QString UserCorrectnessInput::checkValidationUsername(QString user_name, DataBase &database)
{
    // with this expression don`t allow to use
    // to begin from space and more symbols ( asdasdasd) - false
    // to begin from digital and more symbols (09asdasd) - false
    // a0, kamluk ivan k0 asd - GOOD
    // it don`t starts from space or digital
    QRegularExpression reg_username("^\\s.*|^\\d.*|^[-_.].*");
    QRegularExpression reg_username2("[!#$\\№[\\]%&'()*'\\\\+,/:;<=>?@^`{|}~]");

    if (user_name.isEmpty())
        return tr("Username can`t be empty.");
    else if (user_name == "KanbaX" ||
            user_name == "Admin")
        return tr("You can`t use this keyword.");
    else if (user_name.length()>30)
        return tr("Username must have less than 30 symbols.");
    else if (database.checkMatchUsernameOrEmail
            (user_name, Field::Username))
        return tr("Username already exists.");
    else if (user_name.contains(reg_username))
         return tr("Username must begin from letter");
    else if (user_name.contains(reg_username2))
          return tr("Username can contain symbols _ - . spacebar");
    else
      return "";
}

QString UserCorrectnessInput::checkValidationPassword(QString user_password)
{
    // app is hang out
    // when regax is working
    // regax expression to validate password
    // don`t allow to use space at the start or end
    // password must contain at least one uppercase letter
    // one number and one symbol
    QRegularExpression reg_password("^\\s|\\s$");
    QRegularExpression reg_password2("^.*(?=.*\\d)(?=.*[a-zA-Z]).*$");
    if (user_password.isEmpty())
        return tr("Password can`t be empty.");
    else if (user_password.length()<8)
        return tr("Password must have at least 8 symbols.");
    else if (user_password.length()>32)
        return tr("Password must be smaller than 32 symbols.");
    else if (user_password.contains(reg_password))
        return tr("Password can`t begin or end with space.");
    else if (!user_password.contains(reg_password2))
        return tr("Password must have at least one uppercase letter, "
                  "one digit and one symbol.");
    else
        return "";
}

QString UserCorrectnessInput::checkValidationEmail(QString user_email, DataBase &database)
{
    // app is hang out
    // when regax is working
    if (user_email.isEmpty())
        return tr("Email address can`t be empty.");
    else if (isEmail(user_email)==false)
        return tr("Invalid email address.");
    else if(database.checkMatchUsernameOrEmail
            (user_email, Field::Email)==true)
        return tr("Email address already exists.");
    else
        return "";
}

bool UserCorrectnessInput::isEmail(QString user_email)
{
   //   regax expression to validate email address
      QRegExp reg_email("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
      reg_email.setCaseSensitivity(Qt::CaseInsensitive);
      reg_email.setPatternSyntax(QRegExp::RegExp);

      return user_email.contains(reg_email) ? true : false;
}
