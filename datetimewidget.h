#ifndef DATETIMEWIDGET_H
#define DATETIMEWIDGET_H

#include <QWidget>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QStringList>
#include <QTime>
#include <QPair>
#include <QSlider>

#include "customwidget/extendedcombobox.h"
#include "customwidget/switchbutton.h"
#include "appsettings.h"

class DateTimeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DateTimeWidget(QWidget *parent = nullptr);

  //Methods @get
    QString getTaskEndDate();
    QString getTaskEndTime();
    bool is24hoursFormat();

    QWidget* getWidgetDateTime();

    QStringList getHoursFormat24();
    QStringList getHoursFormat12();
    QStringList getMinutes();

    QString getSelectedHours();
    QString getSelectedMinutes();
    QString getSelectedHoursFormat();

    QPair<QString, QString> convertFormat24To12(QString time);
    QString convertFormat12To24(QString time, QString format);

    void disableLessDateTime();

signals:
        void selectedDateTime();
        void selectedDate();
        void selectedTime();
public slots:
        void changeTimeFormat();
private:
    QWidget *widget_datetime, *widget_section_time;
    QVBoxLayout *vbox_datetime_main, *vbox_widget_calendar;
    QHBoxLayout *hbox_section_time;
    QCalendarWidget *calendar_datetime;
    QLabel *label_text_time;
    SwitchButton *button_format_hours;
    AppSettings appsettings;
    ExtendedComboBox *combobox_time_hours, *combobox_time_minutes,
    *combobox_format_12hours;
    QString task_creation_hours, task_creation_minutes;
    bool hours_format24;
    bool combobox_items_hided;
};

#endif
