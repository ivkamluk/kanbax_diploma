#include "taskdoingwidget.h"
#include "styleqss.h"
#include "randomcolor.h"

#include <QDebug>
#include <QHBoxLayout>
#include <cmath>

TaskDoingWidget::TaskDoingWidget(QWidget *parent) : TaskToDoWidget(parent),
    task_time_elapsed(0), column_name_todo(tr("To Do")), column_name_done(tr("Done"))
{
    timer = new QTimer(this);

    widget_task->setStyleSheet
            (StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_taskdoingwidget_light.qss"));

}

void TaskDoingWidget::loadWidget()
{
    addImageDescription();
    addComboBox();
    addComboBoxItems();
    addTaskEndTime();
    addLabelTaskName();
    addProgressBarTaskTime();

  //  generateColorForTopBackground();

    timer->setInterval(1000); // 1 sec
    timer->start();

    task_time_total = getTaskStartDateTime().secsTo(getTaskEndDateTime());
    calculateTimePercentage();

    connect(timer, SIGNAL(timeout()), this, SLOT(calculateTaskTimeOver()));
    connect(combobox_task, SIGNAL(activated(int)), this, SLOT(columnComboBoxSelected()));
}

void TaskDoingWidget::addComboBoxItems()
{
    combobox_task->addItem(column_name_todo);
    combobox_task->addItem(column_name_done);
}

void TaskDoingWidget::stopTaskTime()
{
    timer->stop();
}

void TaskDoingWidget::calculateTaskTimeOver()
{
    if (QDateTime::currentDateTime() > getTaskEndDateTime())
    {
        emit taskOvered();
        timer->stop();
    }
    else
    {
        qDebug()<<"Calculating";
        calculateTimePercentage();
    }
}

void TaskDoingWidget::addTaskEndTime()
{
    hbox_section_endtime = new QHBoxLayout;
    vbox_task_main->addLayout(hbox_section_endtime);

    label_task_end_time_text = new QLabel(this);
    label_task_end_time_text->setObjectName("labelTaskEndTimeText");
    label_task_end_time_text->setText(tr("End time: "));

    label_task_end_time = new QLabel(this);
    label_task_end_time->setObjectName("labelTaskEndTime");

    label_task_end_time->setText(getTaskEndDateTime().toString("dd/MM/yyyy hh:mm"));

    hbox_section_endtime->addStretch(1);
    hbox_section_endtime->addWidget(label_task_end_time_text);
    hbox_section_endtime->addWidget(label_task_end_time);

    hbox_section_endtime->setMargin(0);
    hbox_section_endtime->setAlignment(label_task_end_time_text, Qt::AlignRight);
    hbox_section_endtime->setAlignment(label_task_end_time, Qt::AlignRight);
}

void TaskDoingWidget::addProgressBarTaskTime()
{
   progressbar_task_time = new QProgressBar(this);
   progressbar_task_time->setObjectName("progressbarTaskTime");

   vbox_task_main->addStretch(1);
   vbox_task_main->addWidget(progressbar_task_time);
   vbox_task_main->addSpacing(0);
   vbox_task_main->setMargin(5);
}

void TaskDoingWidget::calculateTimePercentage()
{
    task_time_elapsed = QDateTime::currentDateTime().secsTo(getTaskEndDateTime());
//    qDebug()<<task_time_elapsed;
    qint64 time_percent = task_time_elapsed * 100 / task_time_total;

    if (time_percent >90)
        progressbar_task_time->setStyleSheet("QProgressBar#progressbarTaskTime::chunk"
                                             " { background:#00CE30; }");
    else if (time_percent > 70)
        progressbar_task_time->setStyleSheet("QProgressBar#progressbarTaskTime::chunk"
                                             " { background:#5FCE79; }");
    else if (time_percent > 50)
        progressbar_task_time->setStyleSheet("QProgressBar#progressbarTaskTime::chunk"
                                             " { background:#F4D35E; }");
    else if (time_percent > 30)
        progressbar_task_time->setStyleSheet("QProgressBar#progressbarTaskTime::chunk"
                                             " { background:#F78764; }");
    else if (time_percent > 10)
        progressbar_task_time->setStyleSheet("QProgressBar#progressbarTaskTime::chunk"
                                             " { background:red; }");
    progressbar_task_time->setValue(time_percent);
    calculateTimeLeftDate();
  //  qDebug()<<time_percent;
}

void TaskDoingWidget::calculateTimeLeftDate()
{
    float seconds = task_time_elapsed;
    float minutes = seconds / 60;
    float hours = minutes / 60;
    float days = hours /  24;
    float weeks = days / 7;
    float months = weeks / 6.957;
    float years = months / 12;

  // this section will convert time to pair
    // for example, if we have 62 minuts
    // it will display 1 hour 2 minutes

    if ((int)years  > 0)
            splitTimeUnits(years, months, 12);

    else if ((int)months  > 0)
            splitTimeUnits(months, weeks, 4.3452);

    else if ((int)weeks  > 0)
            splitTimeUnits(weeks, days, 7);

    else if ((int)days  > 0)
             splitTimeUnits(days, hours, 24);

    else if ((int)hours  > 0)
             splitTimeUnits(hours, minutes, 60);

    else if ((int)minutes  > 0)
             splitTimeUnits(minutes, seconds, 60);

// this section will convert time to pair

    QString text_formate_result;

    if ((int)years >0 && years < 12)
        text_formate_result.append(QString::number(years, 'f', 0)).append(tr(" year(s) "));

    if ((int)months >0 && months < 12)
        text_formate_result.append(QString::number(months, 'f', 0)).append(tr(" month(s) "));

    if ((int)weeks >0 && weeks < 4335)
        text_formate_result.append(QString::number(weeks, 'f', 0)).append(tr(" week(s) "));

    if ((int)days >0 && days < 7)
        text_formate_result.append(QString::number(days, 'f', 0)).append(tr(" day(s) "));

    if ((int)hours >0 && hours < 24)
        text_formate_result.append(QString::number(hours, 'f', 0)).append(tr(" hour(s) "));

    if ((int)minutes >0 && minutes < 60)
        text_formate_result.append(QString::number(minutes, 'f', 0)).append(tr(" minute(s) "));

    if ((int)seconds> 0 && seconds < 60)
        text_formate_result.append(QString::number(seconds)).append(tr(" second(s) "));

    progressbar_task_time->setFormat(text_formate_result);
}

void TaskDoingWidget::columnComboBoxSelected()
{
    if (combobox_task->currentText() == column_name_todo)
        emit selectedColumnToDo();
    else if (combobox_task->currentText() == column_name_done)
        emit selectedColumnDone();
}

void TaskDoingWidget::setTaskStartTime(QString task_start_time)
{
    this->task_start_time = task_start_time;
}

void TaskDoingWidget::setTaskEndTime(QString task_end_time)
{
    this->task_end_time = task_end_time;
}

QDateTime TaskDoingWidget::getTaskStartDateTime()
{
    return fromStringToDate(task_start_time);
}

QDateTime TaskDoingWidget::getTaskEndDateTime()
{
    return fromStringToDate(task_end_time);
}

QDateTime TaskDoingWidget::fromStringToDate(QString task_time)
{
    // from database
    QDateTime datetime_task_time = QDateTime::fromString(task_time, "yyyy-MM-ddTHH:mm:ss");

    if (!datetime_task_time.isValid())  // value from saved time on computer
        datetime_task_time = QDateTime::fromString(task_time, "yyyy-MM-dd HH:mm:ss");

    return datetime_task_time;
}

void TaskDoingWidget::splitTimeUnits(float &time1, float &time2, float fractionaltime)
{
    float whole, fractional;
    fractional = std::modf(time1, &whole);

    time1 = whole;
    time2 = fractional * fractionaltime;
}
