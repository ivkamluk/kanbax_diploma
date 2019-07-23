#ifndef VIEWTASK_H
#define VIEWTASK_H

#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>

#include "customwidget/animateddialog.h"
#include "database.h"
#include "customwidget/extendedlabel.h"
#include "appsettings.h"


enum class TaskStatus { ToDo, Doing, Done };


class ViewTask : public AnimatedDialog
{
    Q_OBJECT
public:
    explicit ViewTask(QWidget *parent = nullptr);

 //Methods @set
    void setDataBase(DataBase &database);
    void setUserId(QString user_id);
    void setTaskId(QString task_id);
    void setTaskName(QString task_name);
    void setTaskDescription(QString task_description);
    void setTaskStartTime(QString task_starttime);
    void setTaskEndTime(QString task_endtime);
    void setTaskStatus(TaskStatus status);

//Methods @get

    QString getTaskName();
    QString getTaskDescription();

    void initializeWidgets();
    void loadWidget();

signals:
    void taskRemoved();
    void descriptionOn();
    void descriptionOff();
    void taskUpdated();

public slots:
     void updateCurrentTask();
     void removeTask();
     void enableButtonSave();

 private:
    QFrame *frame_viewtask, *frame_vline;
    QVBoxLayout *vbox_main, *vbox_frame, *vbox_section_endtime, *vbox_section_starttime;
    QHBoxLayout *hbox_section_status_time, *hbox_section_buttons;
    QGridLayout *grid_section_task;
    QLabel *label_dialog_name, *label_background_image, *label_text_task_name,
    *label_text_task_description, *label_text_startime, *label_task_starttime, *label_text_endtime,
    *label_task_status, *label_task_endtime, *label_alert;
    QString user_id, task_id, task_name, task_description, task_starttime,
    task_endtime;
    DataBase database;
    ExtendedLabel *label_image_remove_task, *label_remove_task;
    QPushButton *button_close, *button_save;
    QLineEdit *editline_task_name;
    QTextEdit *textedit_task_name;
    AppSettings appsettings;
    TaskStatus task_status;
    unsigned short task_remove_confrm;
};

#endif
