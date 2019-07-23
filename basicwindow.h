#ifndef BASICWINDOW_H
#define BASICWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "workwindow.h"
#include "signwindow.h"
#include "appsettings.h"

/*
    this class is main for all widgets

    the main idea is to use QStackedWidgets

    and one by one call them

*/

class BasicWindow : public QMainWindow
{
    Q_OBJECT
public:
      explicit BasicWindow(QWidget *parent = nullptr);

      void alignToCenterScreen();

protected:
   // Events

   // this event we need just only for resend it to the workWindow
    void closeEvent(QCloseEvent *event) override;

private:
    QVBoxLayout *vbox_main;
    QWidget *widget_main;
    SignWindow *signwindow;
    QStackedWidget *stacked_widget;
    AppSettings appsettings;
};

#endif
