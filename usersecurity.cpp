#include "usersecurity.h"

#include <random>
#include <QCryptographicHash>
#include <QByteArray>
#include <chrono>
#include <QDateTime>

/*
     this class provides security to the
     user account
*/

UserSecurity::UserSecurity() : salt_iteration(50)
{}

QString UserSecurity::generateSalt()
{
    QString symbols=" !#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHI"
                    "JKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    QString user_salt;

    // generate pse-random number
    unsigned seed = static_cast<int>
            (std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 mt_rng(seed);
    std::uniform_int_distribution<std::mt19937::result_type> rand_numbers(0,symbols.size());

    // with specific number get one character from "symbols" range
    for (unsigned short i=0; i<salt_iteration; i++)
        user_salt.append(symbols[rand_numbers(mt_rng)]);

    return user_salt;
}

QString UserSecurity::generateHashPassword(QString data)
{
    // for generating hash using SHA-2(SHA-512)
    QByteArray byt_array;
    byt_array = data.toUtf8();
    return data = QCryptographicHash::hash(byt_array,QCryptographicHash::Sha512).toHex();
}

QString UserSecurity::generateToken()
{
    // for token we get : salt + salt + currentDateTime + &
    QString token = generateSalt() + generateSalt() +
            QDateTime::currentDateTime().toString() + '&';
    token = generateHashPassword(token);
    return token;
}

//function that makes very simple "ecnryption"
// we do this just for hiding data that user can see
// user can very simply decrypt it, but we do this not for secure
// our data already have been hashed, so we do this just for first user look
QString UserSecurity::simpleEncryptingData(QString data, unsigned short value)
{
    std::string some_data = data.toUtf8().constData();

    for (auto i=0; i<some_data.size(); i++)
            some_data.at(i) += value;

    return QString::fromUtf8(some_data.c_str());
}

QString UserSecurity::simpleDecryptingData(QString data, unsigned short value)
{
    std::string some_data = data.toUtf8().constData();

    for (auto i=0; i<some_data.size(); i++)
            some_data.at(i) -= value;

    return QString::fromUtf8(some_data.c_str());
}
