#ifndef USERSECURITY_H
#define USERSECURITY_H

#include <QString>

/*
     this class provides security to the
     user account
*/

class UserSecurity
{
public:
    UserSecurity();
    QString generateSalt();
    QString generateHashPassword(QString data);
    QString generateToken();

    //function that makes very simple "ecnryption"
    // we do this just for hiding data that user can see
    // user can very simply decrypt it, but we do this not for secure
    // our data already have been hashed, so we do this just for first user look
    QString simpleEncryptingData(QString data, unsigned short value);
    QString simpleDecryptingData(QString data, unsigned short value);

private:
    unsigned short salt_iteration;
};

#endif
