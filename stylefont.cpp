#include "stylefont.h"

/*
    this class provides functionality
    that gives to use custom fonts
*/

//static function that allows to use custom font
QFont StyleFont::loadFontSourceCodeVariable()
{
    QFontDatabase::addApplicationFont(":/fonts/Fonts/SourceCodeVariable.ttf");
    QFont font = QFont("Source Code Variable",1,1);
    return font;
}
