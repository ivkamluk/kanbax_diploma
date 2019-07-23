#include "datetimewidget.h"
#include <styleqss.h>

#include <QDate>

//delete later
#include <QDebug>

DateTimeWidget::DateTimeWidget(QWidget *parent) : QWidget(parent), hours_format24(true),
    combobox_items_hided(false)
{
    widget_datetime = new QWidget;
    widget_datetime->setObjectName("widgetDateTime");

    vbox_datetime_main = new QVBoxLayout;
    widget_datetime->setLayout(vbox_datetime_main);

    calendar_datetime = new QCalendarWidget;
    calendar_datetime->setObjectName("calendarDateTime");
    calendar_datetime->setMinimumDate(QDate::currentDate());
    calendar_datetime->setGridVisible(true);
    calendar_datetime->setLocale(QLocale::English);
    calendar_datetime->setFirstDayOfWeek(Qt::Monday);

    if (appsettings.getUserLanguage() == 2) // Ukrainian
        calendar_datetime->setLocale(QLocale::Ukrainian);
    else if (appsettings.getUserLanguage() == 1) // Russian
       calendar_datetime->setLocale(QLocale::Russian);
    else
        calendar_datetime->setLocale(QLocale::English);

    vbox_datetime_main->setMargin(0);
    vbox_datetime_main->addWidget(calendar_datetime);

// section time

    widget_section_time = new QWidget(this);
    widget_section_time->setObjectName("widgetSectionTime");
    hbox_section_time = new QHBoxLayout;
    widget_section_time->setLayout(hbox_section_time);

    label_text_time = new QLabel;
    label_text_time->setObjectName("labelTextTime");
    label_text_time->setText(tr("Time 24h: "));

    button_format_hours = new SwitchButton;
    button_format_hours->setSwitchStatus(hours_format24);
    button_format_hours->setOffset(45);
    button_format_hours->setFixedWidth(60);

    combobox_time_hours = new ExtendedComboBox;
    combobox_time_hours->setObjectName("comboBoxTimeHours");

    combobox_time_minutes = new ExtendedComboBox;
    combobox_time_minutes->setObjectName("comboBoxTimeMinutes");
    combobox_format_12hours = new ExtendedComboBox;
    combobox_format_12hours->setObjectName("comboBoxFormat12Hours");
    combobox_format_12hours->addItem("AM");
    combobox_format_12hours->addItem("PM");
    combobox_format_12hours->setHidden(true);

    combobox_time_hours->addItems(getHoursFormat24());
    combobox_time_minutes->addItems(getMinutes());

    task_creation_hours = QTime::currentTime().toString("hh");
    task_creation_minutes = QTime::currentTime().toString("mm");

    // now, let`s add currenttime by default to comboboxhours and minutes
    combobox_time_hours->setCurrentText(task_creation_hours);
    combobox_time_minutes->setCurrentText(task_creation_minutes);

    disableLessDateTime();

    hbox_section_time->setMargin(0);
    hbox_section_time->addWidget(label_text_time);
    hbox_section_time->addWidget(button_format_hours);
    hbox_section_time->addWidget(combobox_time_hours);
    hbox_section_time->addWidget(combobox_time_minutes);
    hbox_section_time->addWidget(combobox_format_12hours);

    hbox_section_time->setAlignment(button_format_hours, Qt::AlignCenter);

    vbox_datetime_main->setSpacing(0);
    vbox_datetime_main->addWidget(widget_section_time);

    widget_datetime->setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_datetimewidget_light.qss"));

    connect(button_format_hours, SIGNAL(clicked(bool)), this, SLOT(changeTimeFormat()));

    connect(calendar_datetime, &QCalendarWidget::clicked, this, [this]{
           disableLessDateTime();
           emit selectedDate();
    });

    connect(combobox_time_hours, &ExtendedComboBox::currentTextChanged, this, [this]{
        emit selectedTime();
    });

    connect(combobox_time_minutes, &ExtendedComboBox::currentTextChanged, this, [this]{
        emit selectedTime();
    });

    connect(combobox_format_12hours, &ExtendedComboBox::currentTextChanged, this, [this]{
        emit selectedTime();
    });

}

QString DateTimeWidget::getTaskEndDate()
{
    return calendar_datetime->selectedDate().toString("dd/MM/yyyy");
}

QString DateTimeWidget::getTaskEndTime()
{
    QString time, hours, minutes;

    hours = combobox_time_hours->currentText();
    minutes = ":" + combobox_time_minutes->currentText();
    time = hours + minutes;

    if (!button_format_hours->isSwitchOn())
        time = hours + minutes + " " + combobox_format_12hours->currentText();

    return time;
}

bool DateTimeWidget::is24hoursFormat()
{
    return button_format_hours->isSwitchOn() ? true : false;
}

QWidget* DateTimeWidget::getWidgetDateTime()
{
    return widget_datetime;
}

QStringList DateTimeWidget::getHoursFormat24()
{
    QStringList list_hours;
    for (unsigned short i=0; i<24; i++)
    {
       if (i < 10)
        list_hours.push_back(QString("0"+QString::number(i)));
       else
        list_hours.push_back(QString::number(i));
    }
    return list_hours;
}

QStringList DateTimeWidget::getHoursFormat12()
{
    QStringList list_hours;
        for (unsigned short j=1; j<=12; j++)
           list_hours.push_back(QString::number(j));
    return list_hours;
}

QStringList DateTimeWidget::getMinutes()
{
    QStringList list_minutes;
    for (unsigned short i=0; i<60; i++)
    {
       if (i < 10)
        list_minutes.push_back(QString("0"+QString::number(i)));
       else
        list_minutes.push_back(QString::number(i));
    }
    return list_minutes;
}

QString DateTimeWidget::getSelectedHours()
{
    return combobox_time_hours->currentText();
}

QString DateTimeWidget::getSelectedMinutes()
{
    return combobox_time_minutes->currentText();
}

QString DateTimeWidget::getSelectedHoursFormat()
{
    return combobox_format_12hours->currentText();
}

QPair<QString, QString> DateTimeWidget::convertFormat24To12(QString time)
{
    QString format12;
    QString format24;
    QString format12text;

    QPair<QString, QString> pair_format12;

    for (unsigned short i=0, j=0; i<24; i++)
    {
        if (i<10)
            format24 = "0" + QString::number(i);
        else
            format24 = QString::number(i);

        if (i < 12)
            format12text = "AM";
        else
            format12text = "PM";

        if (i == 12)
            j=0;

        if (time == "00")
        {
            format12 = "12";
            format12text = "AM";
            break;
        }
        else if (time == "12")
        {
            format12 = "12";
            format12text = "PM";
            break;
        }
        else if (format24 == time)
        {
            format12 = QString::number(j);
            break;
        }

        j++;
    }

    pair_format12.first = format12;
    pair_format12.second = format12text;

    return pair_format12;
}

QString DateTimeWidget::convertFormat12To24(QString time, QString format)
{
    QString format24;
    unsigned short min_time, max_time;


    if (time == "12" && format=="AM")
    {
        format24 = "00";
        return format24;
    }
     else if (time =="12" && format =="PM")
    {
        format24 = "12";
        return format24;
    }
    else if (format == "AM")
    {
        min_time = 0;
        max_time = 11;
    }
    else if (format == "PM")
    {
        min_time = 12;
        max_time = 23;
    }
    else
        return time;

    for (unsigned short i=min_time, j=0; i<=max_time; i++)
    {
        if (format == "AM" && i<10)
         format24 = "0" +QString::number(i);
        else
            format24 = QString::number(i);

        if (time == QString::number(i) || time == QString::number(j))
                break;
        j++;
    }

    return format24;
}


void DateTimeWidget::changeTimeFormat()
{
    QString tmp_time;

    if (button_format_hours->isSwitchOn())
    {
        tmp_time = convertFormat12To24(combobox_time_hours->currentText(),
                                                            combobox_format_12hours->currentText());
        combobox_time_hours->clear();
        combobox_time_hours->addItems(getHoursFormat24());
        combobox_time_hours->setCurrentText(tmp_time);
        combobox_format_12hours->setHidden(true);
    }
    else
    {
        QPair <QString, QString> time = convertFormat24To12(combobox_time_hours->currentText());
        tmp_time = time.first;
        combobox_time_hours->clear();
        combobox_time_hours->addItems(getHoursFormat12());
        combobox_time_hours->setCurrentText(tmp_time);
        combobox_format_12hours->setCurrentText(time.second);
        combobox_format_12hours->setHidden(false);
    }

    disableLessDateTime();
}

void DateTimeWidget::disableLessDateTime()
{
  if (QDate::currentDate() == calendar_datetime->minimumDate() &&
          QDate::currentDate() == calendar_datetime->selectedDate())
{
    if (task_creation_minutes != "59")
    {
        combobox_time_minutes->setCurrentText(task_creation_minutes);
        unsigned short index = combobox_time_minutes->currentIndex();
        for (unsigned short i=0; i< combobox_time_minutes->count(); i++)
        {
             if (i<= index)
                combobox_time_minutes->disableItem(i);
        }
        combobox_time_minutes->setCurrentIndex(index+1);
    }

    if (button_format_hours->isSwitchOn())
    {
        combobox_time_hours->setCurrentText(task_creation_hours);
        unsigned short index = combobox_time_hours->currentIndex();
        for (unsigned short i=0; i< combobox_time_hours->count(); i++)
        {
            if (i<index)
                combobox_time_hours->disableItem(i);
        }

        // if we have 59 minutes we need to add 1 hour and 1 minute
        if (task_creation_minutes == "59")
        {
            combobox_time_minutes->setCurrentIndex(0);
            unsigned short index = combobox_time_hours->currentIndex();
            combobox_time_hours->disableItem(index);
            combobox_time_hours->setCurrentIndex(index+1);
        }

    }
    else
    {
        QString format = convertFormat24To12(QString::number(12)).second;
        qDebug()<<format;

            if (format == "PM")
                combobox_format_12hours->disableItem(0); // AM
            else
                 combobox_format_12hours->disableItem(1); // PM

            unsigned short index = combobox_time_hours->currentIndex();
          if (combobox_format_12hours->currentText() == "PM" && task_creation_hours == "12")
              return;
          else
          {
              for (unsigned short i=0; i< combobox_time_hours->count(); i++)
            {
                if (i<index)
                    combobox_time_hours->disableItem(i);
            }
          }
    }
    combobox_items_hided = true;
  return;
 }
  else if (combobox_items_hided == true)
  {
     for(unsigned short i=0; i< combobox_time_hours->count(); i++)
         combobox_time_hours->enableItem(i);

     for(unsigned short i=0; i< combobox_time_minutes->count(); i++)
         combobox_time_minutes->enableItem(i);

     for(unsigned short i=0; i< combobox_format_12hours->count(); i++)
         combobox_format_12hours->enableItem(i);
  }
}
