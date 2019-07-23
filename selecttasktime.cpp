#include "selecttasktime.h"
#include "styleqss.h"

//delete later
#include <QDebug>
#include <QDateTime>

SelectTaskTime::SelectTaskTime(QString task_id, QString task_name, QDateTime task_time, QWidget *parent) :
   task_id(task_id), task_name(task_name), task_start_time(task_time), ExtendedWidget(parent), opendatetime_status(true)
{
    widget_tasktime = new QWidget(this);
    widget_tasktime->setObjectName("widgetTaskTime");

    vbox_tasktime = new QVBoxLayout;
    widget_tasktime->setLayout(vbox_tasktime);

//section open widget @ Select DateTime @

    hbox_section_selectedtime = new QHBoxLayout;
    vbox_tasktime->addLayout(hbox_section_selectedtime);

    label_text_endtime = new QLabel;
    label_text_endtime->setObjectName("labelTextEndTime");
    label_text_endtime->setText(tr("End time: "));

    editline_tasktime = new QLineEdit;
    editline_tasktime->setObjectName("editlineTaskTime");
    editline_tasktime->setReadOnly(true);

    label_selectdatetime = new ExtendedLabel(this);
    label_selectdatetime->setObjectName("labelSelectDateTime");
    label_selectdatetime->setFixedSize(30,30);
    label_selectdatetime->setToolTip(tr("Select end time"));

    hbox_section_selectedtime->addWidget(label_text_endtime);
    hbox_section_selectedtime->addWidget(editline_tasktime);
    hbox_section_selectedtime->addWidget(label_selectdatetime);

// section widget @ Select DateTime @

    datetime_widget = new DateTimeWidget;
    datetime_widget->getWidgetDateTime()->setHidden(true);
    vbox_tasktime->addWidget(datetime_widget->getWidgetDateTime());

// section taskname

    label_task_name = new QLabel;
    label_task_name->setObjectName("labelTaskName");
    label_task_name->setText(task_name);

    vbox_tasktime->addWidget(label_task_name);
    vbox_tasktime->setAlignment(label_task_name, Qt::AlignLeading);

// section buttons

    hbox_section_buttons = new QHBoxLayout;
    vbox_tasktime->addLayout(hbox_section_buttons);

    button_decline = new QPushButton;
    button_decline->setObjectName("buttonDecline");
    button_decline->setText(tr("Decline"));

    button_accept = new QPushButton;
    button_accept->setObjectName("buttonAccept");
    button_accept->setText(tr("Accept"));
    button_accept->setCursor(Qt::PointingHandCursor);
;

    hbox_section_buttons->addStretch(1);
    hbox_section_buttons->addWidget(button_decline);
    hbox_section_buttons->addWidget(button_accept);

// by default we will set end datetime by current datetime
    QString date_time = datetime_widget->getTaskEndDate().
            append(" ").append(datetime_widget->getTaskEndTime());
    editline_tasktime->setText(date_time);

    connect(label_selectdatetime, SIGNAL(clicked()), this, SLOT(openDateTime()));

    connect(button_decline, &QPushButton::clicked, this, [this]{
            emit selectedButtonDecline();
    });

    connect(button_accept, &QPushButton::clicked, this, [this]{
            emit selectedButtonAccept();
    });

    connect(datetime_widget, &DateTimeWidget::selectedDate, this, [this]
    {
        editline_tasktime->setText(datetime_widget->getTaskEndDate().append(" ").
                            append(datetime_widget->getTaskEndTime()));
    });

    connect(datetime_widget, &DateTimeWidget::selectedTime, this, [this]
    {
        editline_tasktime->setText(datetime_widget->getTaskEndDate().append(" ").
                           append(datetime_widget->getTaskEndTime()));
    });

    widget_tasktime->setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_selecttasktime_light.qss"));
}

QWidget* SelectTaskTime::getWidgetTaskTime()
{
    return widget_tasktime;
}

QString SelectTaskTime::getTaskEndDateTime()
{
    QString task_end_datetime = datetime_widget->getTaskEndDate() + " " +
            datetime_widget->getTaskEndTime();
    QString converted_hours;

    qDebug()<<task_end_datetime;

    // if we have 12 hours format -> convert to 24 hours
    if (datetime_widget->is24hoursFormat() == false)
    {
        converted_hours = datetime_widget->convertFormat12To24(datetime_widget->getSelectedHours(),
                                                                 datetime_widget->getSelectedHoursFormat());
        task_end_datetime = datetime_widget->getTaskEndDate() + " " + converted_hours
                + ":" + datetime_widget->getSelectedMinutes();
    }

        task_end_datetime = QDateTime::fromString(task_end_datetime, "dd/MM/yyyy hh:mm").
                                            toString("yyyy-MM-dd HH:mm:ss");

   qDebug()<<task_end_datetime;

    return task_end_datetime;
}

QString SelectTaskTime::getTaskStartTime()
{
    return task_start_time.toString("yyyy-MM-dd HH:mm:ss");
}

void SelectTaskTime::openDateTime()
{
    if (opendatetime_status == true)
    {
         datetime_widget->getWidgetDateTime()->setHidden(false);
         opendatetime_status = false;
    }
    else
    {
        datetime_widget->getWidgetDateTime()->setHidden(true);
        opendatetime_status = true;
    }
}
