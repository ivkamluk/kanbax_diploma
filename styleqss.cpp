#include "styleqss.h"
#include <QFile>
#include <QTextStream>

/*
     this class provides loading from the file
     QSS styles to the specific window
*/

QString StyleQSS::loadQSSfromFile(QString filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    return styleSheet;
}
