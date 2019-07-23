#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T21:47:57
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Diploma_App
TEMPLATE = app

CONFIG +=app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    database.cpp \
    styleqss.cpp \
    recentwidget.cpp \
    stylefont.cpp \
    usersecurity.cpp \
    workwindow.cpp \
    networkdata.cpp \
    appsettings.cpp \
    signwindow.cpp \
    basicwindow.cpp \
    databaseerror.cpp \
    boardwidget.cpp \
    htaskwidget.cpp \
    calendarwidget.cpp \
    createboard.cpp \
    confirmdeleteboard.cpp \
    allboards.cpp \
    globalpositioning.cpp \
    customwidget/animateddialog.cpp \
    customwidget/extendedabel.cpp \
    customwidget/extendedwidget.cpp \
    createtask.cpp \
    taskwidget.cpp \
    tasktodowidget.cpp \
    taskdonewidget.cpp \
    taskdoingwidget.cpp \
    usercorrectnessinput.cpp \
    notificationtaskdone.cpp \
    customwidget/extendedcombobox.cpp \
    boardsettings.cpp \
    customwidget/switchbutton.cpp \
    randomcolor.cpp \
    translatetime.cpp \
    selecttasktime.cpp \
    datetimewidget.cpp \
    settingswidget.cpp \
    viewtask.cpp

HEADERS += \
    database.h \
    styleqss.h \
    recentwidget.h \
    stylefont.h \
    usersecurity.h \
    workwindow.h \
    networkdata.h \
    appsettings.h \
    signwindow.h \
    basicwindow.h \
    databaseerror.h \
    boardwidget.h \
    htaskwidget.h \
    calendarwidget.h \
    createboard.h \
    confirmdeleteboard.h \
    allboards.h \
    globalpositioning.h \
    customwidget/animateddialog.h \
    customwidget/extendedlabel.h \
    customwidget/extendedwidget.h \
    createtask.h \
    taskwidget.h \
    tasktodowidget.h \
    taskdonewidget.h \
    taskdoingwidget.h \
    usercorrectnessinput.h \
    notificationtaskdone.h \
    customwidget/extendedcombobox.h \
    boardsettings.h \
    customwidget/switchbutton.h \
    randomcolor.h \
    translatetime.h \
    selecttasktime.h \
    datetimewidget.h \
    settingswidget.h \
    viewtask.h

TRANSLATIONS += Translation/diploma_ru.ts Translation/diploma_en.ts Translation/diploma_ua.ts

CODECFORSRC     = UTF-8

RESOURCES += \
    resource.qrc

ICON = Diploma_App.icns
