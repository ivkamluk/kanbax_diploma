#ifndef SIGNWINDOW_H
#define SIGNWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QStringList>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QWidget>
#include <QMainWindow>
#include <QScrollArea>

#include "workwindow.h"
#include "database.h"
#include "usersecurity.h"
#include "appsettings.h"
#include "customwidget/extendedlabel.h"
#include "globalpositioning.h"
#include "usercorrectnessinput.h"

/*
    this class is main for user

    only with that window user can sign in/up

    also here we have checking to restore user work

*/

// create enum for better reading
// when username is not match or email exists
// we will have status for this operations

// Invalid=-1 Valid=0
enum class Status { Invalid=-1, Valid };

class SignWindow : public ExtendedWidget
{
    Q_OBJECT
public:
    explicit SignWindow(QWidget *parent = nullptr);

    // this function allows user to start work
    // because each time app is creating session for work
    void createSession(QString username, QString token, bool remember);

    //function to set stackwidget for leter call workwindow
    void setStackedWidget(QStackedWidget *stackedwidget);

    //function that helps to remove all user inputs before
    // we need this when we log out from WorkWindow
    // because if we back we have that values
    // so we need to remove them
    void removeUserInputs();

    // function that gives opportunity to make resize our window
    // in this class
    void setResizing(QMainWindow *mainwindow);

    // this function wiil help to make translation for app
    void loadWidgetsText();

public slots:
    void preLoad();
    void userSignIn();
    void userSignUp();
//    void userSignFacebook();
//    void userSignGoogle();
//    void userForgotPassword();
//    void sendUserNewPassword();
//    void backToSign();
    void isDataBaseConnected();

protected:
    //this function event helps us to make decision
    // what widget to call at moment @show@
    void showEvent(QShowEvent *event) override;
    // this event we need only for resending it to workwindow
    void closeEvent(QCloseEvent *event) override;

private:
    bool load_fail, block_window_moving;
    QVBoxLayout *vbox_layout_tab_sign_in, *vbox_layout_tab_sign_up, *vbox_main,
    *vbox_sign;
    QHBoxLayout *hbox_facebook_google;
    QTabWidget *tab_sign;
    ExtendedComboBox *combobox_country;
    QStringList stringlist_countries;
    QString placeholder_text;
    QMovie *movie_logo;
    QScrollArea *scrollarea;
    QTimer *timer_database_connection;
    QLineEdit *editline_username_sign_in, *editline_password_sign_in;
    QLineEdit *editline_username_sign_up, *editline_password_sign_up,
    *editline_email_sign_up, *editline_forgot_password;
    QPushButton *button_sign_in, *button_sign_up, *button_facebook, *button_google,
    *button_reset_password;
    QCheckBox *checkbox_remember;
    ExtendedLabel *button_forgot_password, *button_back_to_sign;
    WorkWindow *workwindow;
    DataBase database;
    UserSecurity security;
    AppSettings appsettings;
    QLabel *label_error_username_sign_up, *label_error_email_sign_up,
    *label_error_password_sign_up, *label_error_country_sign_up,
    *label_sign_logo, *label_or, *label_forgot_password, *label_error_sign_in;
    // we need widget to set layout
    QWidget *widget_main, *widget_sign, *tab_sign_in, *tab_sign_up;
    QStackedWidget *stacked_widget;
    QMainWindow *window_main;
    bool sign_allowed;
    UserCorrectnessInput *correctinput;
};

#endif
