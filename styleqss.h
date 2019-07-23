#ifndef STYLEQSS_H
#define STYLEQSS_H
#include <QString>

/*
     this class provides loading from the file
     QSS styles to the specific window
*/

class StyleQSS
{
public:
    static QString loadQSSfromFile(QString filename);
};

#endif
