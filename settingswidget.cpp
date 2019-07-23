#include "settingswidget.h"
#include "styleqss.h"
#include "usercorrectnessinput.h"
#include "usersecurity.h"

#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QProcess>
#include <QDateTime>

//delete later
#include <QDebug>

SettingsWidget::SettingsWidget(QWidget *parent) : AnimatedDialog(parent), reload_app(false),
    name_changed(false), email_changed(false), password_changed(false),
    password_confrm_changed(false), account_remove_confrm(0)
{
      setAttribute(Qt::WA_TranslucentBackground, true);

      frame_settings = new QFrame(this);
      frame_settings->setObjectName("frameSettings");

      vbox_main = new QVBoxLayout(this);
      vbox_main->addWidget(frame_settings);

      vbox_frame = new QVBoxLayout(this);
      frame_settings->setLayout(vbox_frame);

      setLayout(vbox_main);

      setFixedSize(820,650);

      hbox_settings_main = new QHBoxLayout(this);

      label_settings_name = new QLabel(this);
      label_settings_name->setObjectName("lobelSettingsName");
      label_settings_name->setText(tr("My settings"));

      label_background_image = new QLabel(this);
      label_background_image->setObjectName("labelBackgroundImage");
      label_background_image->setPixmap(QPixmap(":/image/Image/background_allboard_800px.png"));

      stackedwidget_settings = new QStackedWidget(this);

      list_widget_settings = new QListWidget(this);
      list_widget_settings->setObjectName("listWidgetSettings");

      list_widget_settings->setAttribute(Qt::WA_MacShowFocusRect,0);

      if (appsettings.getUserLanguage() == 2) // Ukainian
        list_widget_settings->setFixedWidth(165);
      else if (appsettings.getUserLanguage() == 1) // Russian
          list_widget_settings->setFixedWidth(185);
      else // English
           list_widget_settings->setFixedWidth(125);

      scrollarea_settings = new QScrollArea(this);
      scrollarea_settings->setObjectName("scrollareaSettings");
      scrollarea_settings->setStyleSheet("background:transparent");
      scrollarea_settings->setWidgetResizable(true);    
      scrollarea_settings->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      scrollarea_settings->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

      list_item_account = new QListWidgetItem(tr("Account"));
      list_item_privacy = new QListWidgetItem(tr("Privacy"));
      list_item_appereance = new QListWidgetItem(tr("Appearance"));
      list_item_aboutapp = new QListWidgetItem(tr("About app"));

      list_widget_settings->addItem(list_item_account);
      list_widget_settings->addItem(list_item_privacy);
      list_widget_settings->addItem(list_item_appereance);
      list_widget_settings->addItem(list_item_aboutapp);

      list_item_account->setSizeHint(QSize(45,45));
      list_item_privacy->setSizeHint(QSize(45,45));
      list_item_appereance->setSizeHint(QSize(45,45));
      list_item_aboutapp->setSizeHint(QSize(45,45));

      list_widget_settings->setCurrentItem(list_item_account);

      // section for buttons

      hbox_section_buttons = new QHBoxLayout;

      button_close = new QPushButton;
      button_close->setObjectName("buttonClose");
      button_close->setText(tr("Close"));

      button_save = new QPushButton;
      button_save->setObjectName("buttonSave");
      button_save->setText(tr("Save"));
      button_save->setCursor(Qt::PointingHandCursor);
      button_save->setEnabled(false);

      hbox_section_buttons->setMargin(15);
      hbox_section_buttons->setSpacing(10);
      hbox_section_buttons->addWidget(button_close);
      hbox_section_buttons->addWidget(button_save);

// layout control

      accountSettings();
      privacySettings();
      appereanceSettings();
      aboutAppSettings();

      stackedwidget_settings->addWidget(frame_account);
      stackedwidget_settings->setCurrentWidget(frame_account);
      scrollarea_settings->setWidget(stackedwidget_settings);
      scrollarea_settings->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
      scrollarea_settings->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
      scrollarea_settings->setMinimumSize(scrollarea_settings->size());

      vbox_control_settings = new QVBoxLayout;
      vbox_control_settings->setSpacing(10);
      vbox_control_settings->addWidget(scrollarea_settings);
      vbox_control_settings->addLayout(hbox_section_buttons);

      vbox_control_settings->setAlignment(hbox_section_buttons, Qt::AlignRight);

      hbox_settings_main->setMargin(0);
      hbox_settings_main->setSpacing(0);
      hbox_settings_main->addWidget(list_widget_settings);
      hbox_settings_main->addLayout(vbox_control_settings);

      vbox_frame->setMargin(0);
      vbox_frame->setSpacing(0);
      vbox_frame->addWidget(label_settings_name);
      vbox_frame->addSpacing(-10);
      vbox_frame->addWidget(label_background_image);
      vbox_frame->addLayout(hbox_settings_main);

      vbox_frame->setAlignment(label_settings_name, Qt::AlignLeft);

      connect(button_close, SIGNAL(clicked()), this, SLOT(reject()));
      connect(button_save, SIGNAL(clicked(bool)), this, SLOT(saveUserSettings()));
      connect(list_widget_settings, SIGNAL(itemClicked(QListWidgetItem*)),
              this, SLOT(setListWidgetItem(QListWidgetItem*)));

      if (appsettings.getThemeColor() == 1) // Dark
          setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_settingswidget_dark.qss"));
      else
          setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_settingswidget_light.qss"));
}

void SettingsWidget::setDataBase(DataBase &database)
{
    this->database = database;
}

void SettingsWidget::setUserId(QString user_id)
{
    this->user_id = user_id;
}

QString SettingsWidget::getUserName()
{
    return editline_name->text();
}

void SettingsWidget::loadWidget()
{
    if (database.isConnected())
    {
        //let`s retrive information about user
        QStringList user_data = database.getUserDataTUser(user_id);
        user_name = user_data.at(1);
        user_email = user_data.at(2);
        user_country = user_data.at(3);
        editline_name->setText(user_name);
        editline_email->setText(user_email);

      // we need to figure out, country and if it need translation
        combobox_country->setCurrentText(user_country);
//        int index_country =  combobox_country->currentIndex();
//        combobox_country->clear();

//        if (appsettings.getUserLanguage() == 2) // Ukrainian
//            combobox_country->addItems(appsettings.readCountriesFromFile(
//                                           ":/files/File/list_countries_ua.txt"));
//        else if (appsettings.getUserLanguage() == 1) // Russian
//            combobox_country->addItems(appsettings.readCountriesFromFile(
//                                           ":/files/File/list_countries_ru.txt"));

//        combobox_country->setCurrentIndex(index_country);

        // let`s try to find all current sessions

        // first let`s find our session_id for making it current session
       QStringList user_session = database.getUserDataTSession(user_name, Field::Username);
                                                    //0 - id    0 - session_id
       user_active_sessions = database.getUserOtherSessions
                                                (user_id, user_session.at(0));
                                                                          // 0 - session_id
       user_current_session = database.getUserCurrentSession(user_session.at(0));

                   //3 device_OS    // 4 - device_name
        QString device_info = "KanbaX, " + user_current_session.at(4) + ", "
        + user_current_session.at(3);
        label_current_device_info->setText(device_info);


        // 0 - location_ip // 1 - location_city // 2 - location_country
        QString network_info = user_current_session.at(0) + " - " +
        user_current_session.at(1) + "," +  user_current_session.at(2);

        label_current_network_info->setText(network_info);

        // now we convert date                    // 5 - session_date
        QDateTime date = QDateTime::fromString(user_current_session.at(5), "yyyy-MM-ddTHH:mm:ss");
        label_current_date_info->setText(date.toString("dd/MM/yyyy hh:mm:ss"));

        intializePrivacyActiveSessions();

    }
}

void SettingsWidget::accountSettings()
{
//    QStringList user_data = database.get

    frame_account = new QFrame;
    vbox_section_account = new QVBoxLayout;
    frame_account->setObjectName("frameAccount");
    frame_account->setLayout(vbox_section_account);
    frame_account->setFrameStyle(QFrame::Box | QFrame::Raised);

    grid_section_account = new QGridLayout;

    label_error_name = new QLabel;
    label_error_name->setObjectName("labelErrorName");
    label_error_name->setHidden(true);

    label_text_name = new QLabel;
    label_text_name->setObjectName("labelTextName");
    label_text_name->setText(tr("Your name: "));

    editline_name = new QLineEdit;
    editline_name->setObjectName("editlineName");
    editline_name->setAttribute(Qt::WA_MacShowFocusRect,0);

    label_error_email = new QLabel;
    label_error_email->setObjectName("labelErrorEmail");
    label_error_email->setHidden(true);

    label_text_email = new QLabel;
    label_text_email->setObjectName("labelTextEmail");
    label_text_email->setText(tr("Your email: "));

    editline_email = new QLineEdit;
    editline_email->setObjectName("editlineEmail");
    editline_email->setAttribute(Qt::WA_MacShowFocusRect,0);

    label_error_password = new QLabel;
    label_error_password->setObjectName("labelErrorPassword");
    label_error_password->setHidden(true);

    label_text_password = new QLabel;
    label_text_password->setObjectName("labelTextPassword");
    label_text_password->setText(tr("Change password: "));

    editline_password = new QLineEdit;
    editline_password->setObjectName("editlinePassword");
    editline_password->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    editline_password->setAttribute(Qt::WA_MacShowFocusRect,0);

    label_text_confrm_password = new QLabel;
    label_text_confrm_password->setObjectName("labelTextConfrmPassword");
    label_text_confrm_password->setText(tr("Confirm new password: "));

    editline_confrm_password = new QLineEdit;
    editline_confrm_password->setObjectName("editlineConfrmPassword");
    editline_confrm_password->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    editline_confrm_password->setAttribute(Qt::WA_MacShowFocusRect,0);

    label_text_country = new QLabel;
    label_text_country->setObjectName("labelTextCountry");
    label_text_country->setText(tr("Your country: "));

    combobox_country = new QComboBox;
    combobox_country->setObjectName("comboboxCountry");

    combobox_country->addItems(appsettings.readCountriesFromFile(
                                   ":/files/File/list_countries_en.txt"));

    label_confirm_remove_account = new QLabel;
    label_confirm_remove_account->setObjectName("labelConfirmRemoveAccount");
    label_confirm_remove_account->setText(tr("Are you sure, that you would like to remove your account?\n"
                                             "All actions can`t be undone! To remove account click it again!"));
    label_confirm_remove_account->setHidden(true);

    label_remove_account = new ExtendedLabel(this);
    label_remove_account->setObjectName("labelRemoveAccount");
    label_remove_account->setText(tr("Remove account"));
    label_remove_account->setCursor(Qt::PointingHandCursor);

    grid_section_account->setHorizontalSpacing(8);
    grid_section_account->setVerticalSpacing(5);

    grid_section_account->addWidget(label_error_name, 0, 1);
    grid_section_account->addWidget(label_text_name, 1, 0);
    grid_section_account->addWidget(editline_name, 1, 1);

    grid_section_account->addWidget(label_error_email, 2, 1);
    grid_section_account->addWidget(label_text_email, 3, 0);
    grid_section_account->addWidget(editline_email, 3, 1);

    grid_section_account->addWidget(label_error_password, 4, 1);
    grid_section_account->addWidget(label_text_password, 5, 0);
    grid_section_account->addWidget(editline_password, 5, 1);

    grid_section_account->addWidget(label_text_confrm_password, 6, 0);
    grid_section_account->addWidget(editline_confrm_password, 6, 1);

    grid_section_account->addWidget(label_text_country, 7, 0);
    grid_section_account->addWidget(combobox_country, 7, 1);

    // we need to style objects that iside qstackedwidget here
    // this is a problem of Qt

    editline_name->setStyleSheet("#editlineName {"
                                 "color:white; font-weight:bold;  border:none; "
                                 "background:transparent;"
                                 "font-size:14px; } #editlineName:hover { "
                                 "border-bottom:1px solid #9b59b6; }");

    editline_email->setStyleSheet("#editlineEmail {"
                                 "color:white; font-weight:bold;  border:none; "
                                 "background:transparent;"
                                 "font-size:14px; } #editlineEmail:hover { "
                                  "border-bottom:1px solid #9b59b6; }");

    editline_password->setStyleSheet("#editlinePassword {"
                                 "color:white; font-weight:bold;  border:none; "
                                 "border-bottom: 1px solid #9b59b6;  background:transparent;"
                                 "font-size:14px; }");

    editline_confrm_password->setStyleSheet("#editlineConfrmPassword {"
                                 "color:white; font-weight:bold;  border:none; "
                                 "border-bottom: 1px solid #9b59b6;  background:transparent;"
                                 "font-size:14px; }");

    combobox_country->setStyleSheet("#comboboxCountry {"
                                    "combobox-popup: 0; color:white;"
                                    "background-color:transparent;"
                                    "min-height:22px; margin-left:4px; font-size: 14px; } "
                                    "#comboboxCountry QAbstractItemView {"
                                    "background-color:white; border-radius:5px; } ");

    label_confirm_remove_account->setStyleSheet("font-size:11px; text-align:center; "
                                                "font-weight:bold; background: #F7BAA8;"
                                                "padding:10px 10px 10px 10px; "
                                                "border-radius:5px; ");

    if (appsettings.getThemeColor() == 1) // Dark
    {
    // style text: name, email, password, confrm-password, country
    label_text_name->setStyleSheet("#labelTextName {"
                                 "color:white; font-size:12px; }");

    label_text_email->setStyleSheet("#labelTextEmail {"
                                 "color:white; font-size:12px; }");

    label_text_password->setStyleSheet("#labelTextPassword {"
                                 "color:white; font-size:12px; }");

    label_text_confrm_password->setStyleSheet("#labelTextConfrmPassword {"
                                 "color:white; font-size:12px; }");

    label_text_country->setStyleSheet("#labelTextCountry {"
                                 "color:white; font-size:12px; }");

    // errors

    label_error_name->setStyleSheet("font-size: 11px; color:#DA4167;");

    label_error_email->setStyleSheet("font-size: 11px; color: #DA4167;");

    label_error_password->setStyleSheet("font-size: 11px; color: #DA4167;");

    // now style ediline and combobox

    // style text: remove account
    label_remove_account->setStyleSheet("#labelRemoveAccount { "
                                 "font-size:14px; font-weight:bold; color:#DA4167; "
                                 "border-radius:10px; padding:5px 5px 5px 5px; }"
                                 " #labelRemoveAccount:hover {"
                                 " color: red; }");
    }
    else // Light
    {
        // style text: name, email, password, confrm-password, country
        label_text_name->setStyleSheet("#labelTextName {"
                                     "color:black; font-size:12px; }");

        label_text_email->setStyleSheet("#labelTextEmail {"
                                     "color:black; font-size:12px; }");

        label_text_password->setStyleSheet("#labelTextPassword {"
                                     "color:black; font-size:12px; }");

        label_text_confrm_password->setStyleSheet("#labelTextConfrmPassword {"
                                     "color:black; font-size:12px; }");

        label_text_country->setStyleSheet("#labelTextCountry {"
                                     "color:black; font-size:12px; }");

        // errors

        label_error_name->setStyleSheet("font-size: 11px; color: white;");

        label_error_email->setStyleSheet("font-size: 11px; color: white;");

        label_error_password->setStyleSheet("font-size: 11px; color: white;");

        // now style ediline and combobox

        // style text: remove account
        label_remove_account->setStyleSheet("#labelRemoveAccount { "
                                     "font-size:14px; font-weight:bold; color: white; "
                                     "border-radius:10px; padding:5px 5px 5px 5px; }"
                                     " #labelRemoveAccount:hover {"
                                     " color: #9b59b6; }");
    }

    vbox_section_account->addLayout(grid_section_account);
    vbox_section_account->addWidget(label_confirm_remove_account);
    vbox_section_account->addWidget(label_remove_account);

    vbox_section_account->setAlignment(label_confirm_remove_account, Qt::AlignCenter);
    vbox_section_account->setAlignment(label_remove_account, Qt::AlignCenter);

    scrollarea_settings->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    qDebug()<<scrollarea_settings->sizeHint();

    connect(editline_name, &QLineEdit::textEdited, this, [this] () {
            name_changed = true;
            checkCorrectInputFromAccount();
    });

    connect(editline_email, &QLineEdit::textEdited, this, [this] () {
            email_changed = true;
            checkCorrectInputFromAccount();
    });

    connect(editline_password, &QLineEdit::textChanged, this, [this] () {
            password_changed = true;
            checkCorrectInputFromAccount();
    });

    connect(editline_confrm_password, &QLineEdit::textChanged, this, [this] () {
            password_confrm_changed = true;
            checkCorrectInputFromAccount();
    });

    connect(combobox_country, SIGNAL(currentIndexChanged(int)), this,
            SLOT(checkCorrectInputFromAccount()));

    connect(label_remove_account, SIGNAL(clicked()), this, SLOT(removeUserAccount()));
}

void SettingsWidget::privacySettings()
{
    frame_privacy = new QFrame;  
    vbox_section_privacy = new QVBoxLayout;
    frame_privacy->setLayout(vbox_section_privacy);

    label_current_session = new QLabel;
    label_current_session->setObjectName("labelCurrentSession");
    label_current_session->setText(tr("Current session"));

    frame_current_session = new QFrame;
    frame_current_session->setObjectName("frameSession");

    frame_current_session->setFixedSize(425,105);

    vbox_section_current_session = new QVBoxLayout;
    frame_current_session->setLayout(vbox_section_current_session);

    label_current_device_info = new QLabel;
    label_current_device_info->setObjectName("labelDeviceInfo");

    label_current_device_info->setStyleSheet("font-weight: bold; font-size: 14px; ");

    label_current_network_info = new QLabel;
    label_current_network_info->setObjectName("labelNetworkInfo");

    label_current_date_info = new QLabel;
    label_current_date_info->setObjectName("labelDateInfo");
    label_current_date_info->setStyleSheet("color: #2d2d2d;");

    frame_privacy_line = new QFrame;
    frame_privacy_line->setObjectName("framePrivacyLine");
    frame_privacy_line->setFrameShape(QFrame::HLine);
    frame_privacy_line->setFrameShadow(QFrame::Sunken);
    frame_privacy_line->setFixedHeight(1);

    label_terminate_sessions = new ExtendedLabel(this);
    label_terminate_sessions->setObjectName("labelTerminateSessions");
    label_terminate_sessions->setText(tr("Terminate All Other Sessions"));
    label_terminate_sessions->setCursor(Qt::PointingHandCursor);

    label_active_sessions = new QLabel;
    label_active_sessions->setObjectName("labelActiveSessions");
    label_active_sessions->setText(tr("Active sessions"));

    vbox_section_other_sessions = new QVBoxLayout;

    //stylesheet

    frame_current_session->setStyleSheet("#frameSession { border-radius:5px;"
                                        " background: #C9A4DA; } ");

    if (appsettings.getThemeColor() == 1) // Dark
    {

         label_current_session->setStyleSheet("font-weight: bold; color: white; ");



         label_terminate_sessions->setStyleSheet("#labelTerminateSessions { "
                                                 "font-size:14px; font-weight:bold; color:#DA4167; "
                                                 "border-radius:10px; padding:5px 5px 5px 5px; }"
                                                 " #labelTerminateSessions:hover {"
                                                 " color: red; }");


         frame_privacy_line->setStyleSheet(" background: gray; ");

         label_active_sessions->setStyleSheet("font-weight: bold; color: white; ");

    }
    else
    {

        label_current_session->setStyleSheet("font-weight: bold; color: black; ");


        label_terminate_sessions->setStyleSheet("#labelTerminateSessions { "
                                                "font-size:14px; font-weight:bold; color: white; "
                                                "border-radius:10px; padding:5px 5px 5px 5px; }"
                                                " #labelTerminateSessions:hover {"
                                                " color: #9b59b6; }");

         frame_privacy_line->setStyleSheet(" background: black; ");

         label_active_sessions->setStyleSheet("font-weight: bold; color: black; ");
    }

    vbox_section_current_session->setMargin(15);
    vbox_section_current_session->setSpacing(15);
    vbox_section_current_session->addWidget(label_current_device_info);
    vbox_section_current_session->addWidget(label_current_network_info);
    vbox_section_current_session->addWidget(label_current_date_info);

    vbox_section_current_session->setAlignment(label_current_device_info, Qt::AlignLeft);
    vbox_section_current_session->setAlignment(label_current_date_info, Qt::AlignRight);
    vbox_section_current_session->setAlignment(label_current_network_info, Qt::AlignLeft);

    vbox_section_privacy->setMargin(8);
    vbox_section_privacy->addWidget(label_current_session);
    vbox_section_privacy->addWidget(frame_current_session);
    vbox_section_privacy->addWidget(label_terminate_sessions);
    vbox_section_privacy->addWidget(frame_privacy_line);
    vbox_section_privacy->addWidget(label_active_sessions);
    vbox_section_privacy->addLayout(vbox_section_other_sessions);

    vbox_section_privacy->setAlignment(label_current_session, Qt::AlignTop);
    vbox_section_privacy->setAlignment(label_terminate_sessions, Qt::AlignCenter);
    vbox_section_privacy->setAlignment(frame_current_session, Qt::AlignCenter);

//    scrollarea_settings->setWidgetResizable(true);

    connect(label_terminate_sessions, SIGNAL(clicked()), this, SLOT(removeAllSessions()));
}

void SettingsWidget::intializePrivacyActiveSessions()
{
    for (unsigned long long int i=0; i< user_active_sessions.size(); i++)
    {
        vec_vbox_active_sessions.append(new QVBoxLayout);

        vec_frame_active_sessions.append(new QFrame(this));
        vec_frame_active_sessions.at(i)->setObjectName("frameSession");

        vbox_section_other_sessions->addWidget(vec_frame_active_sessions.at(i));
        vec_frame_active_sessions.at(i)->setLayout(vec_vbox_active_sessions.at(i));

        vec_frame_active_sessions.at(i)->setStyleSheet(" background: #F0E6F4; border-radius: 5px; "
                                                       "margin:2px 40px 2px 40px; ");

        vec_label_active_device_info.append(new QLabel);
        //3 device_OS    // 4 - device_name
        QString device_info = "KanbaX, " + user_active_sessions.at(i).at(4) + ", "
                            + user_active_sessions.at(i).at(3);
        vec_label_active_device_info.at(i)->setText(device_info);
        vec_label_active_device_info.at(i)->setStyleSheet("font-weight: bold; font-size: 14px;");

        vec_label_active_network_info.append(new QLabel);
        // 0 - location_ip // 1 - location_city // 2 - location_country
        QString network_info = user_active_sessions.at(i).at(0) + " - " +
        user_active_sessions.at(i).at(1) + "," +  user_active_sessions.at(i).at(2);
        vec_label_active_network_info.at(i)->setText(network_info);

        vec_label_active_date_info.append(new QLabel);
        // now we convert date                    // 5 - session_date
        QDateTime date = QDateTime::fromString(user_active_sessions.at(i).at(5), "yyyy-MM-ddTHH:mm:ss");
        vec_label_active_date_info.at(i)->setText(date.toString("dd/MM/yyyy hh:mm:ss"));
        vec_label_active_date_info.at(i)->setStyleSheet("padding-left:20px;");


         vec_vbox_active_sessions.at(i)->setMargin(5);
         vec_vbox_active_sessions.at(i)->setSpacing(4);
         vec_vbox_active_sessions.at(i)->addWidget(vec_label_active_device_info.at(i));
         vec_vbox_active_sessions.at(i)->addWidget(vec_label_active_network_info.at(i));
         vec_vbox_active_sessions.at(i)->addWidget(vec_label_active_date_info.at(i));

         vec_vbox_active_sessions.at(i)->setAlignment
                 (vec_label_active_device_info.at(i), Qt::AlignLeft);

         vec_vbox_active_sessions.at(i)->setAlignment
                 (vec_label_active_network_info.at(i), Qt::AlignLeft);

         vec_vbox_active_sessions.at(i)->setAlignment
                 (vec_label_active_date_info.at(i), Qt::AlignRight);

    }

    if (user_active_sessions.isEmpty())
        label_active_sessions->setText("");
}

void SettingsWidget::appereanceSettings()
{
    frame_appereance = new QFrame;
    grid_section_appereance = new QGridLayout;
    frame_appereance->setLayout(grid_section_appereance);

    label_text_notifc_side = new QLabel;
    label_text_notifc_side->setObjectName("labelTextNotifcSide");
    label_text_notifc_side->setText(tr("Notification: "));

    combobox_notifc_side = new QComboBox;
    combobox_notifc_side->setObjectName("comboboxNotifcSide");
    combobox_notifc_side->addItem(tr("Top Left"));
    combobox_notifc_side->addItem(tr("Top Right"));
    combobox_notifc_side->addItem(tr("Bottom Left"));
    combobox_notifc_side->addItem(tr("Bottom Right"));

    combobox_notifc_side->setCurrentIndex(appsettings.getNotificationSide());

    label_text_rnd_colors = new QLabel;
    label_text_rnd_colors->setObjectName("labelTextRndColors");
    label_text_rnd_colors->setText(tr("Random Colors for Todo tasks: "));

    groupbuttons_rnd_colors = new QGroupBox;
    hbox_section_rnd_colors = new QHBoxLayout;

    radbutton_rnd_colors_yes = new QRadioButton;
    radbutton_rnd_colors_yes->setObjectName("radbuttonRndColorsYes");
    radbutton_rnd_colors_yes->setText(tr("Yes"));
    radbutton_rnd_colors_yes->setCheckable(true);

    radbutton_rnd_colors_no = new QRadioButton;
    radbutton_rnd_colors_no->setObjectName("radbuttonRndColorsNo");
    radbutton_rnd_colors_no->setText(tr("No"));
    radbutton_rnd_colors_yes->setCheckable(true);

    bool rnd_colors = appsettings.getOptionRandomColorToDo();

    if (rnd_colors == true)
        radbutton_rnd_colors_yes->setChecked(true);
    else
        radbutton_rnd_colors_no->setChecked(true);

    hbox_section_rnd_colors->addWidget(radbutton_rnd_colors_yes);
    hbox_section_rnd_colors->addWidget(radbutton_rnd_colors_no);

    groupbuttons_rnd_colors->setLayout(hbox_section_rnd_colors);

    label_text_boards_count = new QLabel;
    label_text_boards_count->setObjectName("labelTextBoardsCount");
    label_text_boards_count->setText(tr("Recent boards count: "));

    combobox_boards_count = new QComboBox;
    combobox_boards_count->setObjectName("comboboxBoardsCount");
    combobox_boards_count->addItem("2");
    combobox_boards_count->addItem("3");
    combobox_boards_count->addItem("4");

    combobox_boards_count->setCurrentIndex(appsettings.getRecentBoardsCount());

    label_text_theme = new QLabel;
    label_text_theme->setObjectName("labelTextTheme");
    label_text_theme->setText(tr("Theme color: "));

    groupbuttons_theme = new QGroupBox;
    hbox_section_theme = new QHBoxLayout;

    radbutton_theme_light = new QRadioButton;
    radbutton_theme_light->setObjectName("radbuttonThemeLight");
    radbutton_theme_light->setText(tr("Light Ui"));

    radbutton_theme_dark = new QRadioButton;
    radbutton_theme_dark->setObjectName("radbuttonThemeDark");
    radbutton_theme_dark->setText(tr("Dark Ui"));

    // now we need to check if it`s have been saved
    if (appsettings.getThemeColor() == 1) //
        radbutton_theme_dark->setChecked(true);
    else
        radbutton_theme_light->setChecked(true);

    hbox_section_theme->addWidget(radbutton_theme_light);
    hbox_section_theme->addWidget(radbutton_theme_dark);

    groupbuttons_theme->setLayout(hbox_section_theme);

    label_text_language = new QLabel;
    label_text_language->setObjectName("labelTextLanguage");
    label_text_language->setText(tr("Language: "));

    combobox_language = new QComboBox;
    combobox_language->setObjectName("comboboxLanguage");
    combobox_language->addItem(tr("English"));
    combobox_language->addItem(tr("Russian"));
    combobox_language->addItem(tr("Ukrainian"));

    combobox_language->setCurrentIndex(appsettings.getUserLanguage());

    label_text_restartapp = new QLabel;
    label_text_restartapp->setObjectName("labelTextRestartApp");
    label_text_restartapp->setText(tr("*Theme and language changes requires "
                                      "reload app"));

    // we need to style objects that iside qstackedwidget here
    // this is a problem of Qt

    if (appsettings.getThemeColor() == 1) // Dark
    {
    // style text: name, email, password, confrm-password, country
    label_text_notifc_side->setStyleSheet("#labelTextNotifcSide {"
                                 "color:white; font-size:12px; }");

    label_text_rnd_colors->setStyleSheet("#labelTextRndColors {"
                                 "color:white; font-size:12px; }");

    label_text_boards_count->setStyleSheet("#labelTextBoardsCount {"
                                 "color:white; font-size:12px; }");

    label_text_theme->setStyleSheet("#labelTextTheme {"
                                 "color:white; font-size:12px; }");

    label_text_language->setStyleSheet("#labelTextLanguage {"
                                 "color:white; font-size:12px; }");

    label_text_restartapp->setStyleSheet("#labelTextRestartApp {"
                                 "color:gray; font-size:12px; }");

    radbutton_rnd_colors_no->setStyleSheet("color: white;");
    radbutton_rnd_colors_yes->setStyleSheet("color: white;");
    radbutton_theme_dark->setStyleSheet("color: white;");
    radbutton_theme_light->setStyleSheet("color: white;");

    // now style radiobuttons and combobox

    combobox_notifc_side->setStyleSheet("#comboboxNotifcSide {"
                                    "combobox-popup: 0; color:white;"
                                    "background-color:transparent;"
                                    "min-height:22px;  margin-left:4px;  font-size: 14px; } "
                                    "#comboboxNotifcSide QAbstractItemView {"
                                    "background-color:white; border-radius:5px; } ");

    combobox_boards_count->setStyleSheet("#comboboxBoardsCount {"
                                    "combobox-popup: 0; color:white;"
                                    "background-color:transparent;"
                                    "min-height:22px; margin-left:4px; font-size: 14px;  } "
                                    "#comboboxBoardsCount QAbstractItemView {"
                                    "background-color:white; border-radius:5px; } ");

    combobox_language->setStyleSheet("#comboboxLanguage {"
                                    "combobox-popup: 0; color:white;"
                                    "background-color:transparent;"
                                    "min-height:22px;  margin-left:4px;  font-size: 14px; } "
                                    "#comboboxLanguage QAbstractItemView {"
                                    "background-color:white; border-radius:5px; } ");

    }
    else // Light
    {
        // style text: name, email, password, confrm-password, country
        label_text_notifc_side->setStyleSheet("#labelTextNotifcSide {"
                                     "color:black; font-size:12px; }");

        label_text_rnd_colors->setStyleSheet("#labelTextRndColors {"
                                     "color:black; font-size:12px; }");

        label_text_boards_count->setStyleSheet("#labelTextBoardsCount {"
                                     "color:black; font-size:12px; }");

        label_text_theme->setStyleSheet("#labelTextTheme {"
                                     "color:black; font-size:12px; }");

        label_text_language->setStyleSheet("#labelTextLanguage {"
                                     "color:black; font-size:12px; }");

        label_text_restartapp->setStyleSheet("#labelTextRestartApp {"
                                     "color:lavender; font-size:12px; }");

        radbutton_rnd_colors_no->setStyleSheet("color: black;");
        radbutton_rnd_colors_yes->setStyleSheet("color: black;");
        radbutton_theme_dark->setStyleSheet("color: black;");
        radbutton_theme_light->setStyleSheet("color: black;");

        // now style radiobuttons and combobox

        combobox_notifc_side->setStyleSheet("#comboboxNotifcSide {"
                                        "combobox-popup: 0; color:black;"
                                        "background-color:transparent;"
                                        "min-height:22px; margin-left:4px; font-size: 14px; } "
                                        "#comboboxNotifcSide QAbstractItemView {"
                                        "background-color:white; border-radius:5px; } ");

        combobox_boards_count->setStyleSheet("#comboboxBoardsCount {"
                                        "combobox-popup: 0; color:black;"
                                        "background-color:transparent;"
                                        "min-height:22px; margin-left:4px; font-size: 14px; } "
                                        "#comboboxBoardsCount QAbstractItemView {"
                                        "background-color:white; border-radius:5px; } ");

        combobox_language->setStyleSheet("#comboboxLanguage {"
                                        "combobox-popup: 0; color:black;"
                                        "background-color:transparent;"
                                        "min-height:22px; margin-left:4px; font-size: 14px; } "
                                        "#comboboxLanguage QAbstractItemView {"
                                        "background-color:white; border-radius:5px; } ");
    }

    grid_section_appereance->setSpacing(6);

    grid_section_appereance->addWidget(label_text_notifc_side, 0, 0);
    grid_section_appereance->addWidget(combobox_notifc_side, 0, 1, Qt::AlignLeft);

    grid_section_appereance->addWidget(label_text_rnd_colors, 1, 0);
    grid_section_appereance->addWidget(groupbuttons_rnd_colors, 1, 1);

    grid_section_appereance->addWidget(label_text_boards_count, 2, 0);
    grid_section_appereance->addWidget(combobox_boards_count, 2, 1, Qt::AlignLeft);

    grid_section_appereance->addWidget(label_text_theme, 3, 0);
    grid_section_appereance->addWidget(groupbuttons_theme, 3, 1);

    grid_section_appereance->addWidget(label_text_language, 4, 0);
    grid_section_appereance->addWidget(combobox_language, 4, 1, Qt::AlignLeft);

    grid_section_appereance->addWidget(label_text_restartapp, 5, 0);

    connect(combobox_notifc_side, SIGNAL(currentIndexChanged(int)), this, SLOT(enableButtonSaveFromAppearance()));
    connect(radbutton_rnd_colors_no, SIGNAL(toggled(bool)), this, SLOT(enableButtonSaveFromAppearance()));
    connect(radbutton_rnd_colors_yes, SIGNAL(toggled(bool)), this, SLOT(enableButtonSaveFromAppearance()));
    connect(combobox_boards_count, SIGNAL(currentIndexChanged(int)), this, SLOT(enableButtonSaveFromAppearance()));
    connect(radbutton_theme_light, SIGNAL(toggled(bool)), this, SLOT(enableReload()));
    connect(radbutton_theme_dark, SIGNAL(toggled(bool)), this, SLOT(enableReload()));
    connect(radbutton_rnd_colors_yes, SIGNAL(toggled(bool)), this, SLOT(enableButtonSaveFromAppearance()));
    connect(combobox_language, SIGNAL(currentIndexChanged(int)), this, SLOT(enableReload()));
}

void SettingsWidget::aboutAppSettings()
{
    frame_about_app = new QFrame;
    grid_section_about_app = new QGridLayout;
    frame_about_app->setLayout(grid_section_about_app);

    label_logo_kanbax = new QLabel;
    label_logo_kanbax->setStyleSheet("border-image:url(':/image/Image/logo35px.png');");
    label_logo_kanbax->setFixedSize(85,85);

    label_about_app = new QTextEdit;
    label_about_app->setReadOnly(true);

    QString filename;

    if (appsettings.getUserLanguage() == 2) // Ukrainian
        filename = ":/files/File/terms&conditions_ua.txt";
    else if (appsettings.getUserLanguage() == 1) // Russian
        filename = ":/files/File/terms&conditions_ru.txt";
    else
        filename = ":/files/File/terms&conditions_en.txt";

    QFile file(filename);

    QString terms_conditions;
    if (file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&file);
       while (!in.atEnd())
          terms_conditions.append(in.readLine());
       file.close();
    }

    label_text_powered = new QLabel;
    label_text_powered->setText(tr("Powered by <b><font color='#258235'>Qt</font></b>"));

    label_about_app->setText(terms_conditions);

    if (appsettings.getThemeColor() ==1) // Dark
    {
         label_text_powered->setStyleSheet("font-size:15px; color: white;");
         label_about_app->setStyleSheet("color: white;");
    }
    else
    {
        label_text_powered->setStyleSheet("font-size:15px; color: black;");
        label_about_app->setStyleSheet("color: black;");
    }

    grid_section_about_app->addWidget(label_about_app, 1,0);
    grid_section_about_app->addWidget(label_text_powered, 2,0);
}

void SettingsWidget::setListWidgetItem(QListWidgetItem *item)
{
    if (list_item_account == item)
        stackedwidget_settings->setCurrentWidget(frame_account);

    if (list_item_privacy == item)
    {
        stackedwidget_settings->addWidget(frame_privacy);
        stackedwidget_settings->setCurrentWidget(frame_privacy);
    }

    if (list_item_appereance == item)
    {
        stackedwidget_settings->addWidget(frame_appereance);
        stackedwidget_settings->setCurrentWidget(frame_appereance);
    }

    if (list_item_aboutapp == item)
    {
        stackedwidget_settings->addWidget(frame_about_app);
        stackedwidget_settings->setCurrentWidget(frame_about_app);
    }

    scrollarea_settings->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollarea_settings->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void SettingsWidget::enableButtonSaveFromAppearance()
{
   button_save->setEnabled(true);

   // but also, we need to check, if user have made some changes in Account
   // if he does, then we need to disable button

// function for checking
   enableButtonSaveFromAccount();
}

void SettingsWidget::checkCorrectInputFromAccount()
{
    QString error_border_stylessheet = "border: none; border-bottom: 1px solid red; color: lavender";

      UserCorrectnessInput correctline;

      if (name_changed == true)
      {
          label_error_name->setText(correctline.checkValidationUsername
                                    (editline_name->text(), database));

          // now we need to get, if we have a an error
          if (label_error_name->text().isEmpty())
          {
              editline_name->setStyleSheet("#editlineName {"
                                           "color:white; font-weight:bold;  border:none; "
                                           "background:transparent;"
                                           "font-size:14px; } #editlineName:hover { "
                                           "border-bottom:1px solid #9b59b6; }");
               label_error_name->setHidden(true);

          }
          else
          {
            editline_name->setStyleSheet(error_border_stylessheet);
            label_error_name->setHidden(false);
          }

         name_changed = false;
      }

      if (email_changed == true)
      {
          label_error_email->setText(correctline.checkValidationEmail
                                     (editline_email->text(), database));
          email_changed = false;

          if (label_error_email->text().isEmpty())
          {
              editline_email->setStyleSheet("#editlineEmail {"
                                           "color:white; font-weight:bold;  border:none; "
                                           "background:transparent;"
                                           "font-size:14px; } #editlineEmail:hover { "
                                            "border-bottom:1px solid #9b59b6; }");

               label_error_email->setHidden(true);
          }
          else
          {
            editline_email->setStyleSheet(error_border_stylessheet);
            label_error_email->setHidden(false);
          }

      }

      if (password_changed == true)
      {
        label_error_password->setText(correctline.checkValidationPassword
                                      (editline_password->text()));
        password_changed = false;

        if (label_error_password->text().isEmpty())
        {
            editline_password->setStyleSheet("#editlinePassword {"
                                         "color:white; font-weight:bold;  border:none; "
                                         "border-bottom: 1px solid #9b59b6;  background:transparent;"
                                         "font-size:14px; }");
            label_error_password->setHidden(true);
        }
        else
        {
          editline_password->setStyleSheet(error_border_stylessheet);
          label_error_password->setHidden(false);
        }
      }

      if (password_confrm_changed == true)
      {
           if (editline_password->text() == editline_confrm_password->text())
           {
               // and again let`s check if we have an error for a password
                QString password_error = correctline.checkValidationPassword(editline_password->text());

                 if (password_error.isEmpty())
                 {
                     editline_password->setStyleSheet("#editlinePassword {"
                                                  "color:white; font-weight:bold;  border:none; "
                                                  "border-bottom: 1px solid #9b59b6;  background:transparent;"
                                                  "font-size:14px; }");

                     editline_confrm_password->setStyleSheet("#editlineConfrmPassword {"
                                                  "color:white; font-weight:bold;  border:none; "
                                                  "border-bottom: 1px solid #9b59b6;  background:transparent;"
                                                  "font-size:14px; }");
                    label_error_password->setHidden(true);
                 }
                else
                 {
                     label_error_password->setText(password_error);
                     editline_password->setStyleSheet(error_border_stylessheet);
                     editline_confrm_password->setStyleSheet(error_border_stylessheet);
                     label_error_password->setHidden(false);
                 }
           }
           else
           {
               label_error_password->setText(tr("Passwords don`t match"));
               editline_password->setStyleSheet(error_border_stylessheet);
               editline_confrm_password->setStyleSheet(error_border_stylessheet);
               label_error_password->setHidden(false);
           }
      }

    enableButtonSaveFromAccount();
}

void SettingsWidget::enableButtonSaveFromAccount()
{
    // now we are trying to decide, if we need to enable button_save
    // if user_have made some errors, button, must be disabled

    // So,  button will be enabled if
    /*
     * 1) there is no errors in: name, email, password
     * 2) password and confirmation password are equals
     * 3) that was not the first launch
     * P.S. we need to check, if it was the first launch, because
     *  when we insert data from database inside editline
     * then we have a signal and buttons can be enabled, so to prevent it
     * we say, that, we have not the first launch
     */

    bool password_ok = editline_password->text() == editline_confrm_password->text() &&
           label_error_password->isHidden() == true;

   if (label_error_name->isHidden() == true && label_error_email->isHidden() == true &&
           password_ok == true)
       button_save->setEnabled(true);
   else
       button_save->setEnabled(false);
}

void SettingsWidget::enableReload()
{
    reload_app = true;
    enableButtonSaveFromAppearance();
}

void SettingsWidget::saveUserSettings()
{
    // Account Settings let`s save
    qDebug()<<"Saved Account";
       if (database.isConnected())
       {
           // now we will update this value
           // username, email, password and account

           if (editline_name->text() != user_name)
           {
                 database.updateUserName(user_id, editline_name->text());
                 appsettings.saveUserSignName(editline_name->text());
                 emit updateUserName();
           }
                 // after update username

           if (editline_email->text() != user_email)
                 database.updateUserEmail(user_id, editline_email->text());

           if (!editline_password->text().isEmpty())
           {
               // then we update password
               // and agian we need to make hash for it
               // so for rehash = salt + password + symbol '&'
               // get unique salt(symbols for making secure password)
               UserSecurity security;
               QString user_salt =  security.generateSalt();
               QString user_salt_password = user_salt + editline_password->text() + '&';

               database.updateUserPassword(user_id, security.generateHashPassword(user_salt_password));
               database.updateUserSalt(user_id, user_salt);
           }

           if (combobox_country->currentText() != user_country)
               database.updateUserCountry(user_id, combobox_country->currentText());

       }

// Appearance Settings let`s save
       qDebug()<<"Saved Appearance";
       appsettings.saveNotificationSide(combobox_notifc_side->currentIndex());

       if (radbutton_rnd_colors_no->isChecked())
           appsettings.saveOptionRandomColorTodo(false);
       else if (radbutton_rnd_colors_yes->isChecked())
            appsettings.saveOptionRandomColorTodo(true);

       appsettings.saveRecentBoardsCount(combobox_boards_count->currentIndex());
       appsettings.saveUserLanguage(combobox_language->currentIndex());

       if (radbutton_theme_light->isChecked())
           appsettings.saveThemeColor(0); // Light
       else if (radbutton_theme_dark->isChecked())
            appsettings.saveThemeColor(1); // Dark

       if (reload_app == true)
       {
           qApp->quit();
           QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
           reload_app = false;
       }

       // after all we will close it
       reject();
}

void SettingsWidget::removeUserAccount()
{
    account_remove_confrm++;
    label_confirm_remove_account->setHidden(false);

    if (account_remove_confrm > 1)
    {
        if (database.isConnected())
        {
            qDebug()<<"Deleted\n";
            database.removeAccount(user_id);
            appsettings.saveUserSignName("");
            appsettings.saveUserSignToken("");
            emit accountDeleted();

            // let`s close now
            reject();
        }
    }
}

void SettingsWidget::removeAllSessions()
{
    if (database.isConnected())
    {
        // we get token from database, because user can change his value in HKEY_CURRENT_USER
        // or in plist, so we need to find it on another way : in database
        QStringList current_session = database.getUserDataTSession(user_name, Field::Username);
                                                    // 0 -id       // 5- token
        database.removeOtherAvailableSessions(user_id, current_session.at(5));

        for (unsigned long long int i=0; i<user_active_sessions.size(); i++)
            vec_frame_active_sessions.at(i)->setVisible(false);

        label_active_sessions->setText("");

    }
}


