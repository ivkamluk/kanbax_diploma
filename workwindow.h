#ifndef WORKWINDOW_H
#define WORKWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QTimer>
#include <QStackedWidget>
#include <QScrollArea>
#include <QSize>

#include "database.h"
#include "appsettings.h"
#include "recentwidget.h"
#include "customwidget/extendedlabel.h"
#include "customwidget/extendedwidget.h"
#include "boardwidget.h"
#include "htaskwidget.h"
#include "calendarwidget.h"
#include "settingswidget.h"

/*
     this class is main for user work

     with this class user can manage with all his boads
     add task, remove, open calendar and many other...
*/

class WorkWindow : public ExtendedWidget
{
    Q_OBJECT
public:
    explicit WorkWindow(QWidget *parent = nullptr);
    void setUserName(QString username);
    void setDatabase(DataBase& database);

    void loadWindow();

    void setStackWidget(QStackedWidget *stackedwidget);

    void setResizing(QMainWindow *window);

public slots:
    // this functions check if user are still in
    // app, maybe you have been removed this session
    // remotely
    void isAvailableSession();
    // this function allows user to exit from accout
    // and remove his current session
    void closeCurrentSession();
    void setRecentWidget();
    void setListWidgetItem(QListWidgetItem* item);
    void showRecentWidget(); 

    bool eventFilter(QObject *watched, QEvent *event);

 protected:
    // at window close we want to remove user current session
    // it`s very important when user have session_remember=0
    // because at this moment in databse will not removed his session
    void closeEvent(QCloseEvent *event);

private:
    QString  user_name;
    QWidget *top_widget, *main_widget, *widget_icon_editline, *widget_side_panel,
    *widget_connection;
    ExtendedWidget *widget_settings_user;
    QHBoxLayout *hbox_top, *hbox_bottom, *hbox_settings_user, *hbox_icon_editline;
    QVBoxLayout *vbox_main, *vbox_side_panel, *vbox_connection;
    ExtendedLabel *label_icon_search;
    QLabel *label_quit, *label_user, *label_connection, *label_status_connection,
    *label_text_logo;
    QLineEdit *editline_search;
    QListWidget *list_widget_menu, *list_widget_control;
    QTimer *timer;
    //TODO: add here later vector
    // Because i would like to make alias of boards
    QListWidgetItem *list_item_home, *list_item_board, *list_item_htask,
    *list_item_calendar, *list_item_settings;
    RecentWidget *recentwidget;
    BoardWidget *boardwidget;
    HTaskWidget *htaskwidget;
    CalendarWidget *calendarwidget;
    QScrollArea *scrollarea;
    DataBase database;
    QStringList user_data, user_session;
    AppSettings appsettings;
    SettingsWidget *settingswidget;
    QStackedWidget *stacked_widget_window_main, *stacked_widget_current_window;
    QMainWindow *window_main;
};

#endif
