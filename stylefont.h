#ifndef STYLEFONT_H
#define STYLEFONT_H
#include <QFont>
#include <QFontDatabase>

/*
    this class provides functionality
    that gives to use custom fonts
*/

class StyleFont
{
public:
    //static function that allows to use custom font
    static QFont loadFontSourceCodeVariable();
};

#endif
