#ifndef TASKDONEWIDGET_H
#define TASKDONEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>

#include "taskwidget.h"

class TaskDoneWidget : public TaskWidget
{
    Q_OBJECT
public:
    explicit TaskDoneWidget(QWidget *parent = nullptr);

    void loadWidget() override;
    void addComboBoxItems() override;
    void generateColorForTopBackground() override;

//Methods @set

    void setTaskOverByUser(bool task_over_byuser);
    void setTaskStartTime(QDateTime task_starttime);
    void setTaskEndTime(QDateTime task_endtime);

// Methods @get
    QDateTime getTaskStartTime();
    QDateTime getTaskEndTime();

    void confirmationDone();

    bool isTaskOverByUser();

    void markAsDoneByUser();

//Methods @get
    QVBoxLayout *getLayoutMain();

signals:
    void selectedColumnToDo();
    void selectedColumnDoing();
    void taskConfirmYes();
    void taskConfirmNo();

public slots:
    void columnComboBoxSelected() override;

protected:
    QWidget *widget_background;
    QVBoxLayout *vbox_background;
    QHBoxLayout *hbox_section_confirm_buttons;
    QString column_name_todo, column_name_doing;
    bool task_over_byuser;
    QLabel *label_image_done, *label_confirmation_text;
    QPushButton *button_confirm_no, *button_confirm_yes;
    QDateTime task_starttime, task_endtime;
};

#endif
