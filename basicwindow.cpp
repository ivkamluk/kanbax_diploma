#include "basicwindow.h"
#include "styleqss.h"
#include "stylefont.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>

#define APP_MIN_WIDTH 450
#define APP_MIN_HEIGHT 610

#define APP_MAX_WIDTH 530
#define APP_MAX_HEIGHT 730


/*
    this class is main for all widgets

    the main idea is to use QStackedWidgets

    and one by one call them

*/

// delete later
#include <QDebug>

BasicWindow::BasicWindow(QWidget *parent) : QMainWindow(parent)
{
    setMinimumSize(APP_MIN_WIDTH, APP_MIN_HEIGHT);
    resize(APP_MAX_WIDTH, APP_MAX_HEIGHT);

    widget_main = new QWidget(this);
    widget_main->setObjectName("widgetMain");

    vbox_main = new QVBoxLayout;
    signwindow = new SignWindow;

    // for all our widgets we have qstack
    // by this we can load them one by one
    stacked_widget = new QStackedWidget;

    stacked_widget->addWidget(signwindow);

    // current widget is Sign in/up

    stacked_widget->setCurrentWidget(signwindow); // launch with signwindow

    widget_main->setLayout(vbox_main);

    vbox_main->addWidget(stacked_widget);
    // with margin we remove border
    vbox_main->setMargin(0);

    setCentralWidget(widget_main);

    alignToCenterScreen();

    // we do this, to make opportunity to change current widget to
    // workwindow
         signwindow->setStackedWidget(stacked_widget);
         signwindow->setResizing(this);

    if (appsettings.getThemeColor() == 1) // Dark
            setStyleSheet(StyleQSS::loadQSSfromFile
                          (":/styles/QSS/MAC_mainwindow_dark.qss"));
    else
        setStyleSheet(StyleQSS::loadQSSfromFile
                      (":/styles/QSS/MAC_mainwindow_light.qss"));
}

 // this function helps to set widget to the center of our screen
void BasicWindow::alignToCenterScreen()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                size(), qApp->desktop()->availableGeometry()));
}

//Events

 //this event we need just only for resend it to the signwindow->workwindow
void BasicWindow::closeEvent(QCloseEvent *event)
{
    QApplication::sendEvent(signwindow, event);
}

