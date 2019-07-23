#ifndef BOARDSETTINGS_H
#define BOARDSETTINGS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include "database.h"
#include "customwidget/animateddialog.h"
#include "customwidget/extendedlabel.h"
#include "customwidget/switchbutton.h"
#include "appsettings.h"

class BoardSettings : public AnimatedDialog
{
    Q_OBJECT
public:
    explicit BoardSettings(QWidget *parent = nullptr);

 //Methods @set
    void setDataBase(DataBase &database);
    void setBoardId(QString board_id);
    void setUserId(QString user_id);
    void setBoardName(QString board_name);
    void setBoardNotificationSound(bool board_notification_sound);

    void loadWidget();

signals:
       void allTasksDeleted();
       void boardDeleted();
       void boardUpdated();
public slots:
        void saveBoardSettings();
        void removeBoard();
        void removeTasks();
        void enableButtonSave();
private:
    DataBase database;
    QString board_id, user_id, board_name, alert_text;
    QVBoxLayout *vbox_main, *vbox_frame_boardsettings;
    QHBoxLayout *hbox_section_name, *hbox_section_sound, *hbox_section_buttons;
    QFrame *frame_boardsettings;
    QLabel *label_boardsettings_name, *label_background_image, *label_title_board_name,
    *label_title_board_notification, *label_alert;
    ExtendedLabel *label_image_remove_board, *label_title_remove_board, *label_remove_tasks;
    QPushButton *button_close, *button_save;
    QLineEdit *editline_board_name;
    AppSettings appsettings;
    bool board_notification_sound, tasks_available;
    unsigned short board_rmcount_click, tasks_rm_count_click;
    SwitchButton *switch_button_notification;
};

#endif
