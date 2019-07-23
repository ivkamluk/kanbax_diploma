#ifndef TASKDOINGWIDGET_H
#define TASKDOINGWIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QDateTime>
#include <QTimer>

//delete later
#include <QDebug>

#include "tasktodowidget.h"

class TaskDoingWidget : public TaskToDoWidget
{
    Q_OBJECT
public:
    explicit TaskDoingWidget(QWidget *parent = nullptr);

    void addTaskEndTime();
    void addProgressBarTaskTime();

    void calculateTimePercentage();
    void calculateTimeLeftDate();

    void splitTimeUnits(float &time1, float &time2, float fractionaltime);

    void loadWidget() override;
    void addComboBoxItems() override;

    void stopTaskTime();

    QDateTime fromStringToDate(QString task_time);

 //Methods @set
    void setTaskStartTime(QString task_start_time);
    void setTaskEndTime(QString task_end_time);

//Methods @get
    QDateTime getTaskStartDateTime();
    QDateTime getTaskEndDateTime();

signals:

      void taskOvered();
      void selectedColumnToDo();
public slots:

    void calculateTaskTimeOver();
    void columnComboBoxSelected() override;
private:
    QLabel *label_task_end_time_text, *label_task_end_time;
    QHBoxLayout *hbox_section_endtime;
    QProgressBar *progressbar_task_time;
    QString task_start_time, task_end_time, column_name_todo, column_name_done;
    QTimer *timer;
    qint64 task_time_total, task_time_elapsed;
};

#endif
