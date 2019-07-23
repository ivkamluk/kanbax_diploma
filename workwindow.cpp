#include "workwindow.h"
#include "stylefont.h"
#include "styleqss.h"
#include "globalpositioning.h"
#include "boardsettings.h"

#include <QPropertyAnimation>
#include <QDesktopWidget>
#include <QApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QGraphicsOpacityEffect>
#include <QTranslator>

#define APP_MIN_WIDTH 466
#define APP_MIN_HEIGHT 610

#define APP_MAX_WIDTH 530
#define APP_MAX_HEIGHT 730

/*
     this class is main for user work

     with this class user can manage with all his boads
     add task, remove, open calendar and many other...
*/

//delete later
#include <QDebug>

WorkWindow::WorkWindow(QWidget *parent)
    :ExtendedWidget(parent)
{
    // thif stacked widget display current window in work window
    stacked_widget_current_window = new QStackedWidget(this);

    timer = new QTimer(this);

    top_widget = new QWidget(this);

    hbox_top = new QHBoxLayout;
    hbox_bottom = new QHBoxLayout;
    vbox_main = new QVBoxLayout;

    widget_connection = new QWidget(this);
    widget_connection->setObjectName("widgetConnection");

    vbox_connection = new QVBoxLayout(this);

    widget_connection->setLayout(vbox_connection);
    vbox_connection->setMargin(0);

    label_connection = new QLabel(this);
    label_connection->setObjectName("labelConnection");
    label_connection->setFixedSize(30,30);

    label_status_connection = new QLabel(this);
    label_status_connection->setObjectName("labelStatusConnection");

    vbox_connection->addWidget(label_connection);
    vbox_connection->addWidget(label_status_connection);

    vbox_connection->setAlignment(label_connection, Qt::AlignCenter);

    label_user = new QLabel(this);
    label_user->setObjectName("userName");

    label_quit = new QLabel(this);

    widget_settings_user = new ExtendedWidget(this);
    widget_settings_user->setObjectName("widgetSettingsUser");
    widget_settings_user->setCursor(Qt::PointingHandCursor);

    hbox_settings_user = new QHBoxLayout(this);

//    editline_search = new QLineEdit(this);
//    editline_search->setObjectName("userSearch");

//    // removes blue border from QLineEdit from focus on MAC
//    editline_search->setAttribute(Qt::WA_MacShowFocusRect,0);

//    // default text for searching
//    editline_search->setPlaceholderText(tr("Search in: boards, tasks, descriptions..."));
    //editline_search->setFrame(true);

//    hbox_icon_editline = new QHBoxLayout(this);
//    widget_icon_editline = new QWidget(this);
//    widget_icon_editline->setObjectName("widgetIconEditLine");
//    widget_icon_editline->setLayout(hbox_icon_editline);

//    label_icon_search = new ExtendedLabel(this);
//    label_icon_search->setPixmap(QPixmap(":/image/Image/search.png"));
//    label_icon_search->setCursor(Qt::PointingHandCursor);

//    // with spacing 0 - we make together editline and icon

    label_text_logo = new QLabel;
    label_text_logo->setObjectName("labelTextLogo");
    label_text_logo->setText("Kanba<font color='yellow'>X</font>");

   // label_text_logo->setFont(StyleFont::loadFontSourceCodeVariable());

//    hbox_icon_editline->addWidget(label_text_logo);
//    hbox_icon_editline->setSpacing(0);

    label_quit->setPixmap(QPixmap(":/image/Image/account_quit.png"));

    top_widget->setLayout(hbox_top);
    top_widget->setFixedHeight(62);

    widget_settings_user->setLayout(hbox_settings_user);

    hbox_settings_user->addWidget(label_user);
    hbox_settings_user->addWidget(label_quit);

    hbox_top->addSpacing(10);
    hbox_top->addWidget(widget_connection);
    hbox_top->addStretch(1);
    hbox_top->addSpacing(110);
    hbox_top->addWidget(label_text_logo);
    hbox_top->addStretch(1);
    hbox_top->addWidget(widget_settings_user);

    hbox_top->setAlignment(label_text_logo, Qt::AlignVCenter);
    hbox_top->setAlignment(widget_settings_user, Qt::AlignRight);

    // left indent (10)
    hbox_top->setMargin(10);
    // left indent (0) to edge of window app for listWidget_menu
    hbox_bottom->setMargin(0);
    hbox_bottom->setSpacing(0);
    vbox_main->setSpacing(0);
    // for correct use we set indent to 0 for mainwidget layout
    vbox_main->setMargin(0);

    widget_side_panel = new QWidget(this);
    vbox_side_panel = new QVBoxLayout(this);

    widget_side_panel->setLayout(vbox_side_panel);

    list_widget_menu = new QListWidget(this);
    list_widget_menu->setObjectName("listWidgetMenu");

    list_widget_menu->setFixedWidth(62);
    //now let`s remove horizontal scroll bar, making width only 62
    list_widget_menu->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    list_widget_menu->setAttribute(Qt::WA_MacShowFocusRect,0);

    list_widget_control = new QListWidget(this);
    list_widget_control->setObjectName("listWidgetControl");
    list_widget_control->setFixedWidth(62);
    // max size for widget settings
    list_widget_control->setFixedHeight(43);
    list_widget_control->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    list_widget_control->setAttribute(Qt::WA_MacShowFocusRect,0);

    vbox_side_panel->addWidget(list_widget_menu);
    vbox_side_panel->addWidget(list_widget_control);
    vbox_side_panel->setMargin(0);
    vbox_side_panel->setSpacing(0);

    widget_side_panel->setFixedWidth(62);

    hbox_bottom->addWidget(widget_side_panel);

    // opportunity to make window resizable
    scrollarea = new QScrollArea(this);
    //inside QSS it`s not working, so let it be here
    scrollarea->setStyleSheet("background:transparent;");
    //with this option widget can resize to all scrollarea
    scrollarea->setWidgetResizable(true);

    // now we need to set widget to scrollarea
    scrollarea->setWidget(stacked_widget_current_window);

    hbox_bottom->addWidget(scrollarea);

    vbox_main->addWidget(top_widget);
    // add splitter among empty widget, because it will not constrait
    //correct that 2 widgets
    vbox_main->addLayout(hbox_bottom);

    setLayout(vbox_main);

///
    // Later change here to an array

    list_item_home = new QListWidgetItem("");
    list_item_home->setIcon( QPixmap(":/image/Image/home.png") );
    list_item_home->setToolTip(tr("Home"));

    list_item_board = new QListWidgetItem("");
    list_item_board->setIcon( QPixmap(":/image/Image/board.png") );
    list_item_board->setToolTip(tr("Board"));

//    list_item_htask = new QListWidgetItem("");
//    list_item_htask->setIcon( QPixmap(":/image/Image/hot_tasks.png") );
//    list_item_htask->setToolTip(tr("Hot tasks"));

//    list_item_calendar = new QListWidgetItem("");
//    list_item_calendar->setIcon( QPixmap(":/image/Image/calendar.png") );
//    list_item_calendar->setToolTip(tr("Calendar"));

    list_widget_menu->addItem(list_item_home);
    list_widget_menu->addItem(list_item_board);
//    list_widget_menu->addItem(list_item_htask);
//    list_widget_menu->addItem(list_item_calendar);

    list_item_settings = new QListWidgetItem("");
    list_item_settings->setIcon( QPixmap(":/image/Image/settings.png") );
    list_item_settings->setToolTip(tr("Settings"));

    list_widget_control->addItem(list_item_settings);

    list_widget_menu->setIconSize( QSize( 45, 45) );
    list_widget_control->setIconSize( QSize( 35, 35) );
    QPalette palette =list_widget_control->palette();
    palette.setColor(QPalette::Background, Qt::transparent);
    list_widget_control->setPalette(palette);

    timer->start(3000); // every 3 seconds

    // now connect labels and buttons

    connect(timer, SIGNAL(timeout()), this, SLOT(isAvailableSession()));

    // connect remove all sessions and display all sessions
    // ->
 //   connect(widget_settings_user, SIGNAL(clicked()), this, SLOT(closeCurrentSession()));

    connect(widget_settings_user, SIGNAL(clicked()), this, SLOT(closeCurrentSession()));

    connect(list_widget_menu, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(setListWidgetItem(QListWidgetItem*)));

    connect(list_widget_control, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(setListWidgetItem(QListWidgetItem*)));


    if (appsettings.getThemeColor() == 1) // Dark
        setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_workwindow_dark.qss"));
    else
        setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_workwindow_light.qss"));
}

void WorkWindow::setUserName(QString username)
{
    user_name = username;
}

void WorkWindow::setDatabase(DataBase &database)
{
    this->database = database;
}


void WorkWindow::loadWindow()
{
    if (database.isConnected())
    {
        // so, let`s first find user id
        // and after that we will alwsays make operations to correct user
        user_data = database.getUserDataTUser(user_name, Field::Username);
        //let`s take user_token to update user_session
        // here we can get token from HKEY_CURRENT_USER or plist
        // because user can have a time to change this value
        // at MainWindow we have checking if this data`s are equivalent
        QStringList user_sign = appsettings.retrieveUserSignData();
        label_user->setText(user_data.at(1));

          //now update session    // 0 - id     1 - token
        database.updateSession(user_data.at(0), user_sign.at(1));

        // first let`s find our session_id for making it current session
        user_session = database.getUserDataTSession(user_name, Field::Username);

        setRecentWidget();
        // and now let`s add image to our connection if it`s good
         label_connection->setStyleSheet("border-image:url(':/image/Image/connection_on.png');");
         label_status_connection->setText(tr("Connected"));
         label_status_connection->setStyleSheet("background:lavender; color:green;");
         // color for Home
         list_widget_menu->item(0)->setSelected(true);
    }
    else
    {
        label_connection->setStyleSheet("border-image:url(':/image/Image/connection_off.png');");
        label_status_connection->setText(tr("Disconnected"));
        label_status_connection->setStyleSheet("background:red; color:yellow;");
    }
}

// this functions check if user are still in
// app, maybe you have been removed this session
// remotely
void WorkWindow::isAvailableSession()
{
    if (database.isConnected())
    {
        label_connection->setStyleSheet("border-image:url(':/image/Image/connection_on.png'); padding-left:4px;");
        label_status_connection->setText(tr("Connected"));
        label_status_connection->setStyleSheet("background:lavender; color:green;");
        // again we need to get every 3seconds our data from session
        // because user can  be deleted from session remotely
        // that`s why, we need every time get it from database
         QStringList user_current_session = database.getUserDataTSession(user_name,Field::Username);
        // if current session is not available
        // maybe user have closed it remotely
        if (user_current_session.empty())
        {
            appsettings.saveUserSignName("");
            appsettings.saveUserSignToken("");
        }
    }
    else
    {
      label_connection->setStyleSheet("border-image:url(':/image/Image/connection_off.png');");
      label_status_connection->setText(tr("Disconnected"));
      label_status_connection->setStyleSheet("background:red; color:yellow;");
    }
}

void WorkWindow::setStackWidget(QStackedWidget *stackedwidget)
{
   stacked_widget_window_main = stackedwidget;
}

void WorkWindow::setResizing(QMainWindow *widget)
{
    window_main = widget;
}

// this function allows user to exit from account
// and remove his current session
void WorkWindow::closeCurrentSession()
{
                                      // 0 - id      5 - token
    database.removeCurrentSession(user_data.at(0), user_session.at(5));

    // now close and reopen -> Sign in/up
    stacked_widget_window_main->setCurrentIndex(0);

    // let`s make window smaller to resize for our widgets
    window_main->setMinimumSize(APP_MIN_WIDTH,APP_MIN_HEIGHT);
    window_main->resize(APP_MAX_WIDTH,APP_MAX_HEIGHT);

    // now little hack
    // if user quit from his account, we will set our sign window
    // to the center of our workWindow, by that we don`t have illusion
    // that they locates on different positions
    int width = window_main->geometry().width()/2;
    // there we have divided all the size of WorkWindow

    // height we leave, what we have
    int height = window_main->pos().y();

      // and now, we just add to our signWidow width
    // width that we have divided from workWindow
    // and when we exit from account, our signWindow will be on center
    window_main->move(window_main->pos().x()+width, height);

}

void WorkWindow::setRecentWidget()
{
    recentwidget = new RecentWidget(this);
    recentwidget->setDatabase(database);
    recentwidget->setUserId(user_data.at(0));
    recentwidget->setStackedWidget(stacked_widget_current_window);
    recentwidget->setListWidget(list_widget_menu);
    recentwidget->setResizing(window_main);
    recentwidget->loadWidget();
    stacked_widget_current_window->addWidget(recentwidget);
    stacked_widget_current_window->setCurrentWidget(recentwidget);
}

void WorkWindow::setListWidgetItem(QListWidgetItem *item)
{
    if (list_item_home == item)
    {
         showRecentWidget();
         scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
         scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    if (list_item_board == item)
    {
        boardwidget = new BoardWidget(this);
        boardwidget->setUserId(user_data.at(0)); // 0 - user id
        boardwidget->setDataBase(database);
        boardwidget->setResizing(window_main);
        boardwidget->loadWidget();
        stacked_widget_current_window->addWidget(boardwidget);
        stacked_widget_current_window->setCurrentWidget(boardwidget);
    }

    if (list_item_settings == item)
    {
        settingswidget = new SettingsWidget(this);
        settingswidget->setDataBase(database);
        settingswidget->setUserId(user_data.at(0)); // 0 - user_id
        settingswidget->loadWidget();
//        settingswidget->move(GlobalPositioning::parentPos(window_main).x()-
//                          settingswidget->divideDialogWidth(), window_main->y());
       //  now we create effect if we need to make new board
        QGraphicsOpacityEffect *geffect = new QGraphicsOpacityEffect(this);
        geffect->setOpacity(0.2);
        this->setGraphicsEffect(geffect);

        // maybe we need to update userName
        connect(settingswidget, &SettingsWidget::updateUserName, this, [this] () {
            user_name = settingswidget->getUserName();
            label_user->setText(user_name);
        });

        connect(settingswidget, &SettingsWidget::accountDeleted, this, [this] () {

            // now will be open -> Sign in/up
            stacked_widget_window_main->setCurrentIndex(0);

            // let`s make window smaller to resize for our widgets
            window_main->setMinimumSize(APP_MIN_WIDTH,APP_MIN_HEIGHT);
            window_main->resize(APP_MAX_WIDTH,APP_MAX_HEIGHT);

            int width = window_main->geometry().width()/2;
            int height = window_main->pos().y();
            window_main->move(window_main->pos().x()+width, height);
        });

        settingswidget->exec();

        // after that we need to clear effect
        geffect->setEnabled(false);
    }
}

// function show recentwidget
void WorkWindow::showRecentWidget()
{
    recentwidget->clearWidget();
    recentwidget->loadWidget();
    stacked_widget_current_window->setCurrentWidget(recentwidget);
}

bool WorkWindow::eventFilter(QObject *watched, QEvent *event)
{
//   QWidget *widget = qobject_cast<QWidget *>(watched);
//   if (event->type() == QEvent::Enter)
//   {
//       widget_connection->setVisible(true);
//       widget_settings_user->setVisible(true);
//       vbox_main->setStretch(1, 0);
//       return true;
//   }
//   else if (event->type() == QEvent::Leave)
//   {
//       widget_connection->setVisible(false);
//       widget_settings_user->setVisible(false);
//       vbox_main->setStretch(1, 1);
//       return true;
//   }
//   return false;
}

// at window close we want to remove user current session
// it`s very important when user have session_remember=0
// because at this moment in databse will not removed his session
void WorkWindow::closeEvent(QCloseEvent *event)
{
    // now let`s compare if we have user_session and it`s not remembered
    // if user not select ->Remember Me than let`s delete this session
                                      // 6 - session_remember
    if (!user_session.empty() && user_session.at(6) == "0")
                                          // 0 -id      // 5 - token
        database.removeCurrentSession(user_data.at(0),user_session.at(5));

    // only at this moment we close our with database connection
        database.closeConnection();
    //now let`s save our window position for later restoring
   appsettings.saveWindowPosition(window_main->pos());
}
