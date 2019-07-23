#include "basicwindow.h"
#include "appsettings.h"

#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // we need this for QSettings
    // in settings: MAC -> ~Library\Preferences -> plist
    // Windows: HKEY_CURRENT_USER
    QCoreApplication::setOrganizationName("kanbax");
    QCoreApplication::setApplicationName("KanbaX");

    BasicWindow basicwindow;
    AppSettings appsettings;
    QTranslator translator;

        //now let`s set our window to the saved positions
    if(!appsettings.retrieveWindowPosition().isNull())
        basicwindow.move(appsettings.retrieveWindowPosition());
    else
        basicwindow.alignToCenterScreen();

    if (appsettings.getUserLanguage() == 2) // Ukrainian
        translator.load(":/translation/Translation/diploma_ua.qm");
    else if (appsettings.getUserLanguage() == 1) // Russian
        translator.load(":/translation/Translation/diploma_ru.qm");
    else // English
        translator.load(":/translation/Translation/diploma_en.qm");

    qApp->installTranslator(&translator);
    basicwindow.show();

    return a.exec();
}
