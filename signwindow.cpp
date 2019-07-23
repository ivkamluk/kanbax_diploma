#include "signwindow.h"
#include "styleqss.h"
#include "stylefont.h"
#include "databaseerror.h"
#include "customwidget/extendedcombobox.h"

#include <QTextStream>
#include <QFile>
#include <QStandardItemModel>
#include <QRegExp>
#include <QTimer>
#include <QDesktopWidget>
#include <QRect>
#include <QStackedWidget>
#include <QApplication>


#define APP_MIN_WIDTH  400
#define APP_MIN_HEIGHT 400

#define APP_MAX_WIDTH 1175
#define APP_MAX_HEIGHT 710


/*
    this class is main for user

    only with that window user can sign in/up

    also here we have checking to restore user work

*/

// later delete this libraries
#include <QDebug>

//constructor
SignWindow::SignWindow(QWidget *parent) : ExtendedWidget(parent)
{
    timer_database_connection = new QTimer(this);

    // vbox_main - center layout for all window
    vbox_main = new QVBoxLayout(this);
    vbox_sign = new QVBoxLayout(this);
    widget_main = new QWidget(this);
    // i need here widget_sign
    // because later i hide tab_widget
    // and other buttons
    // without it, to made that we have
    // would be harder
    widget_sign = new QWidget(this);
    widget_sign->setObjectName("widgetSign");

    // here we have movie, because it will makes window more responsible
    // it will say to user that there is loading
    // to make windows more flexibles
    movie_logo = new QMovie(":/image/Image/logo_animate.gif", QByteArray(), this);

    label_sign_logo = new QLabel(this);
    label_sign_logo->setObjectName("labelSignLogo");

    label_sign_logo->setMovie(movie_logo);
    movie_logo->setScaledSize(QSize(320,300));
    movie_logo->start();

    vbox_main->addSpacing(50);

    vbox_main->addWidget(label_sign_logo);
    vbox_main->setAlignment(label_sign_logo, Qt::AlignCenter);
    vbox_main->addSpacing(10);

    // let`s add scrollarea if screen will be very small to contain
    // default size
    scrollarea = new QScrollArea(this);
    scrollarea->setObjectName("scrollArea");
    // first we need to hide it
    // because we call it only if user have not been @Remembered@
    // on system
    scrollarea->setVisible(false);
    scrollarea->setWidget(widget_sign);
    // this opportunity gives us to resize widgets inside scrollarea
    scrollarea->setWidgetResizable(true);
    // at the moment loading, let`s hide it
    scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    widget_sign->setLayout(vbox_sign);

    tab_sign = new QTabWidget;
    tab_sign_in = new QWidget;
    tab_sign_up = new QWidget;

    tab_sign->tabBar()->setObjectName("tabSignBar");

    vbox_sign->addWidget(tab_sign);

    widget_main->setLayout(vbox_main);

    tab_sign->addTab(tab_sign_in, "");
    tab_sign->addTab(tab_sign_up, "");

    vbox_layout_tab_sign_in = new QVBoxLayout;

    editline_username_sign_in = new QLineEdit;
    editline_password_sign_in = new QLineEdit;
    button_sign_in = new QPushButton;
    checkbox_remember = new QCheckBox;
    label_error_sign_in = new QLabel(this);

    // this lines remove blue borders on Mac when mouse are
    //pressed on QLineEdit
    editline_username_sign_in->setAttribute(Qt::WA_MacShowFocusRect,0);
    editline_password_sign_in->setAttribute(Qt::WA_MacShowFocusRect,0);

//    // Make this field looks like password (*******)
    editline_password_sign_in->setEchoMode(QLineEdit::Password);

//    // space beetween widgets in tab
   // vbox_layout_tab_sign_in->setSpacing(15);

    button_sign_in->setCursor(Qt::PointingHandCursor);
    button_sign_in->setDefault(true);

    label_error_sign_in->setFixedHeight(25);
    label_error_sign_in->setAlignment(Qt::AlignCenter);

    vbox_layout_tab_sign_in->addWidget(label_error_sign_in);
    vbox_layout_tab_sign_in->addWidget(editline_username_sign_in,
                                       Qt::AlignHCenter);
    vbox_layout_tab_sign_in->addStretch(1);
    vbox_layout_tab_sign_in->addWidget(editline_password_sign_in,
                                       Qt::AlignHCenter);
    vbox_layout_tab_sign_in->addStretch(1);
    vbox_layout_tab_sign_in->addWidget(button_sign_in,
                                       Qt::AlignHCenter);
    vbox_layout_tab_sign_in->addSpacing(20);
    vbox_layout_tab_sign_in->addWidget(checkbox_remember);
    vbox_layout_tab_sign_in->addStretch(1);

    // set object name for widgets(fields)

    editline_username_sign_in->setObjectName("editlineUsernameSignIn");
    editline_password_sign_in->setObjectName("editlinePasswordSignIn");
    checkbox_remember->setObjectName("checkboxRemember");
    button_sign_in->setObjectName("buttonSignIn");
    label_error_sign_in->setObjectName("labelErrorSignIn");

/////////////

//    //

//        button_forgot_password = new ExtendedLabel(this);
//        button_forgot_password->setObjectName("buttonForgotPassword");
//        button_forgot_password->setText(tr("Forgot your password?"));
//        button_forgot_password->setFixedHeight(25);
//        button_forgot_password->setCursor(Qt::PointingHandCursor);

//        // add it to the tab widget
//        vbox_layout_tab_sign_in->addWidget(button_forgot_password);
//        vbox_layout_tab_sign_in->setAlignment(button_forgot_password, Qt::AlignJustify);

//    //

/////////////
 // initialize widgets (fields) for Registration (Sign up)
    vbox_layout_tab_sign_up = new QVBoxLayout;

    editline_username_sign_up = new QLineEdit(this);
    editline_email_sign_up = new QLineEdit(this);
    editline_password_sign_up = new QLineEdit(this);
    button_sign_up = new QPushButton(this);
    combobox_country = new ExtendedComboBox(this);
    label_error_username_sign_up = new QLabel(this);
    label_error_email_sign_up = new QLabel(this);
    label_error_password_sign_up = new QLabel(this);
    label_error_country_sign_up = new QLabel(this);

    // this lines remove blue borders on Mac when mouse are
    //pressed on QLineEdit
    editline_username_sign_up->setAttribute(Qt::WA_MacShowFocusRect,0);
    editline_email_sign_up->setAttribute(Qt::WA_MacShowFocusRect,0);
    editline_password_sign_up->setAttribute(Qt::WA_MacShowFocusRect,0);

     // Make this field looks like password (*******)
    editline_password_sign_up->setEchoMode(QLineEdit::Password);

    //vbox_layout_tab_sign_up->setSpacing(20);

    button_sign_up->setCursor(Qt::PointingHandCursor);
    button_sign_up->setDefault(true);

 //   vbox_layout_tab_sign_up->setSpacing(2);
    vbox_layout_tab_sign_up->addWidget(label_error_username_sign_up);
    vbox_layout_tab_sign_up->addWidget(editline_username_sign_up,
                                       Qt::AlignHCenter);
    vbox_layout_tab_sign_up->addWidget(label_error_email_sign_up);
    vbox_layout_tab_sign_up->addWidget(editline_email_sign_up,
                                       Qt::AlignHCenter);
    vbox_layout_tab_sign_up->addWidget(label_error_password_sign_up);
    vbox_layout_tab_sign_up->addWidget(editline_password_sign_up,
                                       Qt::AlignHCenter);
  //  vbox_layout_tab_sign_up->addSpacing(5);
    vbox_layout_tab_sign_up->addSpacing(20);
    vbox_layout_tab_sign_up->addWidget(combobox_country, Qt::AlignHCenter);
   // vbox_layout_tab_sign_up->addSpacing(5);
    vbox_layout_tab_sign_up->addSpacing(30);
    vbox_layout_tab_sign_up->addWidget(button_sign_up, Qt::AlignHCenter);
    vbox_layout_tab_sign_up->addStretch(1);

    // set object name for widgets(fields)

    editline_username_sign_up->setObjectName("editlineUsernameSignUp");
    editline_email_sign_up->setObjectName("editlineEmailSignUp");
    editline_password_sign_up->setObjectName("editlinePasswordSignUp");
    button_sign_up->setObjectName("buttonSignUp");
    combobox_country->setObjectName("comboboxCountry");
    label_error_username_sign_up->setObjectName("labelErrorUsernameSignUp");
    label_error_email_sign_up->setObjectName("labelErrorEmailSignUp");
    label_error_password_sign_up->setObjectName("labelErrorPasswordSignUp");
    label_error_country_sign_up->setObjectName("labelErrorCountrySignUp");

    editline_username_sign_in->setFont(StyleFont::loadFontSourceCodeVariable());
    editline_password_sign_in->setFont(StyleFont::loadFontSourceCodeVariable());
    editline_username_sign_up->setFont(StyleFont::loadFontSourceCodeVariable());
    editline_password_sign_up->setFont(StyleFont::loadFontSourceCodeVariable());
    editline_email_sign_up->setFont(StyleFont::loadFontSourceCodeVariable());

///////////

//     label_or = new QLabel(this);

//     label_or->setObjectName("labelOr");
//     label_or->setText(tr("or"));
//     label_or->setAlignment(Qt::AlignHCenter);

//     button_facebook = new QPushButton(this);
//     button_google = new QPushButton(this);
//     button_facebook->setObjectName("buttonFacebook");
//     button_google->setObjectName("buttonGoogle");
//     button_facebook->setCursor(Qt::PointingHandCursor);
//     button_google->setCursor(Qt::PointingHandCursor);

//     button_facebook->setText("Facebook");
//     button_google->setText("Google");
//     button_facebook->setIcon(QIcon(":/image/Image/facebook_icon.png"));
//     button_google->setIcon(QIcon(":/image/Image/google_icon.png"));

//     hbox_facebook_google = new QHBoxLayout;

//     vbox_sign->setSpacing(15);
//     vbox_sign->addWidget(label_or);
//     vbox_sign->addLayout(hbox_facebook_google);

//     hbox_facebook_google->addWidget(button_facebook);
//     hbox_facebook_google->addWidget(button_google);

     sign_allowed = false;

     correctinput = new UserCorrectnessInput(this);

    combobox_country->setComboBoxPlaceHolderText(combobox_country, "");

 if (appsettings.getUserLanguage() == 2) // Ukrainian
     combobox_country->addItems(appsettings.readCountriesFromFile(
                                    ":/files/File/list_countries_ua.txt"));
 else if (appsettings.getUserLanguage() == 1) // Russian
     combobox_country->addItems(appsettings.readCountriesFromFile(
                                    ":/files/File/list_countries_ru.txt"));
 else
     combobox_country->addItems(appsettings.readCountriesFromFile(
                                    ":/files/File/list_countries_en.txt"));

 if (appsettings.getThemeColor() == 1) // Dark
    setStyleSheet(StyleQSS::loadQSSfromFile
                  (":/styles/QSS/MAC_signwindow_dark.qss"));
 else
     setStyleSheet(StyleQSS::loadQSSfromFile // Light
                   (":/styles/QSS/MAC_signwindow_light.qss"));

//        // every 5 seconds, we will check if we have connection to database
//    timer_database_connection->start(5000);

    // connection to buttons
    // make a signal to our clicked buttons
    connect(button_sign_in,SIGNAL(clicked(bool)),this,SLOT(userSignIn()));
    connect(button_sign_up,SIGNAL(clicked(bool)),this,SLOT(userSignUp()));
//    connect(button_facebook,SIGNAL(clicked(bool)),this,SLOT(userSignFacebook()));
//    connect(button_google,SIGNAL(clicked(bool)),this,SLOT(userSignGoogle()));
//    connect(button_forgot_password, SIGNAL(clicked()), this,
//            SLOT(userForgotPassword()));
  //  connect(timer_database_connection, SIGNAL(timeout()), this, SLOT(isDataBaseConnected()));

    tab_sign_in->setLayout(vbox_layout_tab_sign_in);
    tab_sign_up->setLayout(vbox_layout_tab_sign_up);

    setLayout(vbox_main);
}

// this function allows user to start work
// because each time app is creating session for work
void SignWindow::createSession(QString user_id, QString token, bool remember)
{
    if (database.isConnected())
    {
       database.writeUserDataTLocation(user_id);
       database.writeUserDataTDevice(user_id);

       //let`s find last our inserted item
        QStringList user_location = database.getUserDataTLocation(user_id);
        QStringList user_device = database.getUserDataTDevice(user_id);
                                               // 0 - location_id  0 - device_id
        database.writeUserDataTSession(user_id, user_location.at(0), user_device.at(0),
                                       token, remember, true);
    }
    else
        isDataBaseConnected(); // call dialogBox for showing error
}

void SignWindow::setStackedWidget(QStackedWidget *stackedwidget)
{
    stacked_widget = stackedwidget;
}


//function that helps to remove all user inputs before
// we need this when we log out from WorkWindow
// because if we back we have that values
// so we need to remove them
void SignWindow::removeUserInputs()
{
    //restore here values to empty
    // because we using stacked_widget and if we quit from WorkWindow
    // we we will have our input values : Username/Password
    // so remove them
    QString error_border_stylessheet_restore =
            "border-bottom: 1px solid white";

    // Sign in clear
    label_error_sign_in->setText("");
    editline_username_sign_in->setStyleSheet(error_border_stylessheet_restore);
    editline_password_sign_in->setStyleSheet(error_border_stylessheet_restore);
    editline_username_sign_in->setText("");
    editline_password_sign_in->setText("");
    checkbox_remember->setChecked(false);

    // Sign up clear
    label_error_username_sign_up->setText("");
    editline_username_sign_up->setStyleSheet(error_border_stylessheet_restore);
    editline_username_sign_up->setText("");
    label_error_email_sign_up->setText("");
    editline_email_sign_up->setStyleSheet(error_border_stylessheet_restore);
    editline_email_sign_up->setText("");
    label_error_password_sign_up->setText("");
    editline_password_sign_up->setStyleSheet(error_border_stylessheet_restore);
    editline_password_sign_up->setText("");
    label_error_country_sign_up->setText("");
    combobox_country->setStyleSheet("QComboBox#comboboxCountry::item "
                                    "{ color:white; }");
    combobox_country->setCurrentIndex(0);
}

// function that gives opportunity to make resize our window
// in this class
void SignWindow::setResizing(QMainWindow *mainwindow)
{
    window_main = mainwindow;
}

// this function wiil help to make translation for app
void SignWindow::loadWidgetsText()
{
        tab_sign->setTabText(0, tr("Sign in"));
        tab_sign->setTabText(1, tr("Sign up"));

    // Sign in
    editline_username_sign_in->setPlaceholderText(tr("Username or email"));
    editline_password_sign_in->setPlaceholderText(tr("Password"));
    editline_username_sign_in->setPlaceholderText(tr("Username or email"));
    editline_password_sign_in->setPlaceholderText(tr("Password"));
    checkbox_remember->setText(tr("Remember me"));
    button_sign_in->setText(tr("Login"));

    // Sign up
    editline_username_sign_up->setPlaceholderText(tr("Username"));
    editline_email_sign_up->setPlaceholderText(tr("Email"));
    editline_password_sign_up->setPlaceholderText(tr("Password"));
    button_sign_up->setText(tr("Create new account"));
    placeholder_text = tr("Please select a country");
    combobox_country->setItemText(0, placeholder_text);
}

//this function allows us to check
// if user must sign in/up or he can work from now
// if he set session_remember=1
void SignWindow::preLoad()
{
    // this variable shows, that loading sign window is ok or failed
    load_fail = false;
   // we need to have this checking, because
    // if we get back from WorkWindow, then
    // we don`t need to create connection
    // if it`s our first time launching then create it
    if (!database.isConnected())
    // creating connection to the database
         database.createConnection();

    if (database.isConnected())
    {
            // now let`s take user data from plist or HKEY_CURRENT_USER
            // that we have saved before
        QStringList user_sign = appsettings.retrieveUserSignData();
           // 0 - user_name                  1 - token
        if (user_sign.at(0) != "" && user_sign.at(1) != "")
        {
        // okay, now, let`s compare if we have remembered this session
                                            // 0 - user_name         1 - token
            if (database.isRememberedRestore(user_sign.at(0), user_sign.at(1)) ==true)
            {                               // 0 -user_name
                  workwindow = new WorkWindow(this);
                  workwindow->setUserName(user_sign.at(0));
                  workwindow->setDatabase(database);
                  workwindow->setStackWidget(stacked_widget);
                  workwindow->setResizing(window_main);
                  workwindow->loadWindow();
                  stacked_widget->addWidget(workwindow);
                  stacked_widget->setCurrentWidget(workwindow);
                  // let`s make window bigger to resize for our widgets
                  window_main->resize(APP_MAX_WIDTH,APP_MAX_HEIGHT);
                  window_main->setMinimumSize(APP_MIN_WIDTH,APP_MIN_HEIGHT);
                  removeUserInputs();
                  sign_allowed = true;
            }
        }
        movie_logo->stop();
        label_sign_logo->setPixmap(QPixmap(":/image/Image/logo_text.png"));
        vbox_main->addWidget(scrollarea);
        scrollarea->setVisible(true);
        scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
       load_fail = true; // if we have`nt load correct our widgets

    loadWidgetsText();
}

// function to make login
void SignWindow::userSignIn()
{
    QString error_border_stylessheet_restore =
            "border-bottom: 1px solid white";
    label_error_sign_in->setText("");
    editline_username_sign_in->setStyleSheet(error_border_stylessheet_restore);
    editline_password_sign_in->setStyleSheet(error_border_stylessheet_restore);
   if(database.isConnected())
   {
       Status status = Status::Valid;
       Field field;
       QString error_sign_text;
       QString error_border_stylessheet = "border-bottom: 1px solid red";

       // first we check if inserted value of username is -> user_name or email
       bool data_email = correctinput->isEmail(editline_username_sign_in->text());

       if (data_email==true)
       {
           if (database.checkMatchUsernameOrEmail
                   (editline_username_sign_in->text(),Field::Email)==false)
           {
               error_sign_text.append(tr("Invalid email or "));
               status=Status::Invalid;
           }
           field = Field::Email;
       }
       else
       {
           if (database.checkMatchUsernameOrEmail
                   (editline_username_sign_in->text(),Field::Username)==false)
           {
               error_sign_text.append(tr("Invalid username or "));
               status=Status::Invalid;
           }
           field = Field::Username;
       }

         if (status==Status::Valid)
         {
             QStringList user_hash_salt;
             if (field == Field::Email)
               user_hash_salt  = database.getUserHashAndSaltTUser
                         (editline_username_sign_in->text(), Field::Email);
             else
               user_hash_salt = database.getUserHashAndSaltTUser
                         (editline_username_sign_in->text(), Field::Username);

             QString user_password;
                                // 1 - user_salt
                            // hashing -> user_salt + entered_password + &
             user_password = user_hash_salt.at(1) + editline_password_sign_in->text() + "&";
             user_password = security.generateHashPassword(user_password);
                                     // 0 - user_hash
             if (user_password == user_hash_salt.at(0))
             {
                 QStringList user_data =
                         database.getUserDataTUser(editline_username_sign_in->text(), field);
                 QString token = security.generateToken();
                 // now check if we need to remember user
                 // if it`s checked
                 // than we need to save file on computer
                    if (checkbox_remember->isChecked())
                    {
                        // let`s create a session and remember user on device
                                     // 0 - user_id
                        createSession(user_data.at(0), token, true);
                                              // 1 - user_name
                        appsettings.saveUserSignName(user_data.at(1));
                        appsettings.saveUserSignToken(token);
                    }
                    else
                    {
                        // create a session and write to not remember user
                        createSession(user_data.at(0), token, false);
                        // if user doesn`t want to have been logged at device
                        appsettings.saveUserSignName("");
                        appsettings.saveUserSignToken("");
                    }
                 workwindow = new WorkWindow(this);
                 workwindow->setUserName(user_data.at(1));
                 workwindow->setDatabase(database);
                 workwindow->setStackWidget(stacked_widget);
                 workwindow->setResizing(window_main);
                 workwindow->loadWindow();
                 // let`s make window bigger to resize for our widgets
                 window_main->resize(APP_MAX_WIDTH,APP_MAX_HEIGHT);
                 window_main->setMinimumSize(APP_MIN_WIDTH,APP_MIN_HEIGHT);
                 //now we trying to set workWindow to the center of our screen
                 window_main->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                             window_main->size(), qApp->desktop()->availableGeometry()));
                 stacked_widget->addWidget(workwindow);
                 stacked_widget->setCurrentWidget(workwindow);
                 removeUserInputs();
                 sign_allowed = true;
             }
             else
             {
                 if (field == Field::Email)
                     error_sign_text = tr("Invalid email or password.");
                 else
                     error_sign_text = tr("Invalid username or password.");

                 label_error_sign_in->setText(error_sign_text);
                 editline_username_sign_in->setStyleSheet(error_border_stylessheet);
                 editline_password_sign_in->setStyleSheet(error_border_stylessheet);
             }

         }
        else
         {
             error_sign_text.append(tr("password."));

             label_error_sign_in->setText(error_sign_text);
             editline_username_sign_in->setStyleSheet(error_border_stylessheet);
             editline_password_sign_in->setStyleSheet(error_border_stylessheet);
         }
   }
   else
       isDataBaseConnected(); // call dialogBox for showing error
}

// function to create new accout
void SignWindow::userSignUp()
{
  // restore all to default position
    QString error_border_stylessheet_restore =
            "border-bottom: 1px solid white";
    label_error_username_sign_up->setText("");
    editline_username_sign_up->setStyleSheet(error_border_stylessheet_restore);
    label_error_email_sign_up->setText("");
    editline_email_sign_up->setStyleSheet(error_border_stylessheet_restore);
    label_error_password_sign_up->setText("");
    editline_password_sign_up->setStyleSheet(error_border_stylessheet_restore);
    label_error_country_sign_up->setText("");
    combobox_country->setStyleSheet("color:white;");

   if(database.isConnected())
   {
       Status status=Status::Valid;
       QString error_border_stylessheet = "border-bottom: 1px solid red";

       // check username validation
       QString error_text = correctinput->checkValidationUsername
               (editline_username_sign_up->text(),database);
        if(!error_text.isEmpty())
        {
            label_error_username_sign_up->setText(error_text);
            editline_username_sign_up->setStyleSheet(error_border_stylessheet);
            status=Status::Invalid;
        }

        // check email validation
        error_text = correctinput->checkValidationEmail
                (editline_email_sign_up->text(),database);
       if (!error_text.isEmpty())
       {
           label_error_email_sign_up->setText(error_text);
           editline_email_sign_up->setStyleSheet(error_border_stylessheet);
           status=Status::Invalid;
       }

       // check password validation
        error_text = correctinput->checkValidationPassword
                (editline_password_sign_up->text());
      if (!error_text.isEmpty())
      {
          label_error_password_sign_up->setText(error_text);
          editline_password_sign_up->setStyleSheet(error_border_stylessheet);
          status=Status::Invalid;
      }

      // check combobox validation
      if (combobox_country->currentText() == placeholder_text)
      {
          if (appsettings.getThemeColor() == 1) // Dark
              combobox_country->setStyleSheet("color:red;");
          else // Light
               combobox_country->setStyleSheet("color:black;");
         status=Status::Invalid;
      }

      if (status==Status::Valid)
      {
          // get unique salt(symbols for making secure password)
          QString user_salt =  security.generateSalt();
          // make together salt + password
          // and for bigger security we will add to user_salt_password some symbol
          // this symbol change hash
          // and for access to password you will need to know it
          // so for rehash = salt + password + symbol '&'
          QString user_salt_password = user_salt + editline_password_sign_up->text() + '&';
          // hash our user_salt_password
          database.writeUserDataTUser(editline_username_sign_up->text(),
                                        editline_email_sign_up->text(),
                                        security.generateHashPassword(user_salt_password),
                                        user_salt,combobox_country->currentText());
          // let`s create a session
          // if value 1(true) - remember user on device
          QStringList user_data = database.getUserDataTUser
                  (editline_username_sign_up->text(), Field::Username);
          QString token = security.generateToken();
                         //0 - user_id
          createSession(user_data.at(0), token, true);
          // let`t save user sign in settings (for future quick acess to app)
                                 //1 - user_name
         appsettings.saveUserSignName(user_data.at(1));
         appsettings.saveUserSignToken(token);
        // settings by default
         appsettings.saveNotificationSide(1); // top right
         appsettings.saveOptionRandomColorTodo(false);
         appsettings.saveRecentBoardsCount(3);

         workwindow = new WorkWindow(this);
         workwindow->setUserName(user_data.at(1));
         workwindow->setDatabase(database);
         workwindow->setStackWidget(stacked_widget);
         workwindow->setResizing(window_main);
         workwindow->loadWindow();
         // let`s make window bigger to resize for our widgets
         window_main->resize(APP_MAX_WIDTH,APP_MAX_HEIGHT);
         window_main->setMinimumSize(APP_MIN_WIDTH,APP_MIN_HEIGHT);
         //now we trying to set workWindow to the center of our screen
         window_main->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                     window_main->size(), qApp->desktop()->availableGeometry()));
         stacked_widget->addWidget(workwindow);
         stacked_widget->setCurrentWidget(workwindow);
         removeUserInputs();
         sign_allowed = true;
      }
   }
   else
      isDataBaseConnected(); // call dialogBox for showing error
}

//// function to make acess to the account from facebook
//void SignWindow::userSignFacebook()
//{
//   if(database.isConnected())
//   {
//        qDebug()<<"userSignFacebook\n";
//   }
//   else
//      isDataBaseConnected(); // call dialogBox for showing error
//}

//// function to make acess to the account from google
//void SignWindow::userSignGoogle()
//{
//   if(database.isConnected())
//   {
//        qDebug()<<"userSignGoogle\n";
//   }
//   else
//      isDataBaseConnected(); // call dialogBox for showing error
//}

// // function that initialize widgets for sending new password
//void SignWindow::userForgotPassword()
//{
//  //  hide them in widget
//    tab_sign->setVisible(false);
//    label_or->setVisible(false);
//    button_facebook->setVisible(false);
//    button_google->setVisible(false);

//    label_forgot_password = new QLabel(this);
//    label_forgot_password = new QLabel(this);
//    label_forgot_password->setText(tr("Your email address that you"
//                                      " used when you signed up."));
//    label_forgot_password->setObjectName("labelForgotPassword");
//    label_forgot_password->setFixedHeight(30);
//    label_forgot_password->setAlignment(Qt::AlignHCenter);

//    editline_forgot_password = new QLineEdit(this);
//    editline_forgot_password->setObjectName("editlineForgotPassword");
//    editline_forgot_password->setPlaceholderText(tr("e.g., love@kanbax.com"));
//    editline_forgot_password->setAttribute(Qt::WA_MacShowFocusRect,0);
//    editline_forgot_password->setFont(StyleFont::loadFontSourceCodeVariable());

//    button_reset_password = new QPushButton(this);
//    button_reset_password->setObjectName("buttonResetPassword");
//    button_reset_password->setText(tr("Send new password"));
//    button_reset_password->setCursor(Qt::PointingHandCursor);

//    button_back_to_sign = new ExtendedLabel(this);
//    button_back_to_sign->setObjectName("buttonBackToSign");
//    button_back_to_sign->setText(tr("Return back to home"));
//    button_back_to_sign->setFixedHeight(25);
//    button_back_to_sign->setCursor(Qt::PointingHandCursor);

//    vbox_sign->addWidget(label_forgot_password);
//    vbox_sign->addWidget(editline_forgot_password);
//    vbox_sign->addWidget(button_reset_password);
//    vbox_sign->addWidget(button_back_to_sign, Qt::AlignHCenter);

//    vbox_sign->setAlignment(button_back_to_sign,Qt::AlignJustify);

//    connect(button_back_to_sign, SIGNAL(clicked()), this,
//            SLOT(backToSign()));
//    connect(button_reset_password, SIGNAL(clicked(bool)), this,
//            SLOT(sendUserNewPassword()));
//}

//// function that makes sending new password by SMTP protocol
//void SignWindow::sendUserNewPassword()
//{
//    // let`s check if user put an email
//    QString error_border_stylessheet_restore =
//            "border-bottom: 1px solid white";
//    Status status = Status::Valid;
//    label_forgot_password->setText("");
//    label_forgot_password->setStyleSheet("color: lavender;");
//    editline_forgot_password->setStyleSheet(error_border_stylessheet_restore);
//    QString error_border_stylessheet = "border-bottom: 1px solid red";
//if (database.isConnected())
//{
//   if (correctinput->isEmail(editline_forgot_password->text())== true)
//   {
//       // now let`s check if we have this email in database
//       if (database.checkMatchUsernameOrEmail(editline_forgot_password->text(),
//                                                   Field::Email) == true)
//       {
//            // good we have this email
//           // now send a new password

//           label_forgot_password->setText(tr(
//                           "Check email address with your new password to account."));
//       }
//       else
//           status = Status::Invalid;
//   }
//   else
//       status = Status::Invalid;

//   if (status == Status::Invalid)
//   {
//        label_forgot_password->setText(
//                    "Invalid email address. Look like you have made a mistake.");
//        editline_forgot_password->setStyleSheet(error_border_stylessheet);
//        label_forgot_password->setStyleSheet("color: black;");
//   }
//}
//else
//    isDataBaseConnected(); // call dialogBox for showing error
//}

//// function that makes return to the Sign in/up if user
//// change mind to resend new password with his email
//void SignWindow::backToSign()
//{
//    // hide them in widget
//    label_forgot_password->setVisible(false);
//    editline_forgot_password->setVisible(false);
//    button_reset_password->setVisible(false);
//    button_back_to_sign->setVisible(false );

//    // show recent hiden
//    tab_sign->setVisible(true);
//    label_or->setVisible(true);
//    button_facebook->setVisible(true);
//    button_google->setVisible(true);
//}

void SignWindow::isDataBaseConnected()
{
    //let`s check if we have connection to our database
    if (!database.isConnected())
    {
        DataBaseError *databaseerror = new DataBaseError(this);
        // let`s move our dialog to the center-top
        databaseerror->move(GlobalPositioning::parentPos(window_main).x()
                            - databaseerror->divideDialogWidth(), window_main->frameGeometry().y());
        block_window_moving = true;
        databaseerror->exec();
    }
    else if (load_fail == true)
        preLoad(); // call again this function
        // to check what we need to do again
}

//this function event helps us to make decision
// what widget to call at moment @show@
void SignWindow::showEvent(QShowEvent *event)
{
  //  QTimer::singleShot(2000, this, SLOT(preLoad()));
    QTimer::singleShot(1, this, SLOT(preLoad()));
}

// this event we need only for resending it to workwindow
void SignWindow::closeEvent(QCloseEvent *event)
{
    if (sign_allowed==true)
        QApplication::sendEvent(workwindow, event);
}
