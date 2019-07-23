#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QFrame>
#include <QListWidget>
#include <QListWidgetItem>
#include <QScrollArea>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QTextEdit>
#include <QVector>

#include "customwidget/animateddialog.h"
#include "customwidget/extendedlabel.h"
#include "database.h"
#include "appsettings.h"

class SettingsWidget : public AnimatedDialog
{
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = nullptr);

//Methods @set

    void setDataBase(DataBase &database);
    void setUserId(QString user_id);

//Methods @get
    QString getUserName();

    void loadWidget();
    void accountSettings();
    void privacySettings();
    void appereanceSettings();
    void aboutAppSettings();
    void intializePrivacyActiveSessions();

signals:
    void updateUserName();
    void accountDeleted();

public slots:
    void setListWidgetItem(QListWidgetItem *item);

    void enableButtonSaveFromAppearance();
    void enableButtonSaveFromAccount();

    void checkCorrectInputFromAccount();

    void enableReload();

    void saveUserSettings();

    void removeUserAccount();

    //this function gives opportunity to make
    //remotely disconection for another devices
    //that are still working with this app
    void removeAllSessions();

private:
    QFrame *frame_settings, *frame_account, *frame_privacy,
    *frame_appereance, *frame_about_app, *frame_privacy_line, *frame_current_session;
    QVBoxLayout *vbox_main, *vbox_frame, *vbox_control_settings, *vbox_section_other_sessions,
    *vbox_section_account, *vbox_section_privacy, *vbox_section_current_session;
    QHBoxLayout *hbox_settings_main, *hbox_section_buttons, *hbox_section_rnd_colors,
    *hbox_section_theme;
    QGridLayout *grid_section_account, *grid_section_appereance, *grid_section_about_app;
    QPushButton *button_close, *button_save;
    QStackedWidget *stackedwidget_settings;
    QListWidget *list_widget_settings;
    QListWidgetItem *list_item_privacy, *list_item_account, *list_item_appereance,
    *list_item_aboutapp;
    QString user_id, user_name, user_email, user_country;
    QStringList user_current_session;
    QVector<QStringList> user_active_sessions;
    DataBase database;
    QScrollArea *scrollarea_settings;

    QLabel *label_background_image, *label_settings_name, *label_text_name,
    *label_text_email, *label_text_password, *label_text_confrm_password,
    *label_text_country, *label_text_notifc_side, *label_text_rnd_colors, *label_text_boards_count,
    *label_text_theme, *label_text_language, *label_text_restartapp, *label_logo_kanbax, *label_text_powered,  *label_confirm_remove_account,  *label_error_name, *label_error_email, *label_error_password, *label_current_session, *label_active_sessions, *label_current_device_info,
    *label_current_network_info, *label_current_date_info;

    QLineEdit *editline_name, *editline_email, *editline_password, *editline_confrm_password;
    QComboBox *combobox_country, *combobox_notifc_side, *combobox_boards_count, *combobox_language;
    ExtendedLabel *label_remove_account, *label_terminate_sessions;
    QGroupBox *groupbuttons_rnd_colors, *groupbuttons_theme;
    QRadioButton *radbutton_rnd_colors_no, *radbutton_rnd_colors_yes, *radbutton_theme_light,
    *radbutton_theme_dark;
    QTextEdit *label_about_app;
    AppSettings appsettings;
    bool reload_app, name_changed, email_changed, password_changed, password_confrm_changed;
    unsigned short account_remove_confrm;

    // vectors for session - privacy
    QVector<QFrame *> vec_frame_active_sessions;
    QVector<QLabel *> vec_label_active_device_info, vec_label_active_network_info, vec_label_active_date_info;
    QVector<QVBoxLayout *> vec_vbox_active_sessions;
};

#endif
