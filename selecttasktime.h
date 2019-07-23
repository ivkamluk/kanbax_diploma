#ifndef SELECTTASKTIME_H
#define SELECTTASKTIME_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

#include "customwidget/extendedlabel.h"
#include "customwidget/extendedwidget.h"
#include "datetimewidget.h"


class SelectTaskTime : public ExtendedWidget
{
    Q_OBJECT
public:
    explicit SelectTaskTime(QString task_id, QString task_name, QDateTime task_time, QWidget *parent = nullptr);

 //Methods @get
    QWidget* getWidgetTaskTime();

    QString getTaskEndDateTime();
    QString getTaskStartTime();

signals:
        void selectedButtonDecline();
        void selectedButtonAccept();
public slots:
        void openDateTime();
private:
    QWidget *widget_tasktime, *widget_disabled_taskname;
    QVBoxLayout *vbox_tasktime;
    QHBoxLayout *hbox_section_selectedtime,*hbox_section_buttons;
    QLineEdit *editline_tasktime;
    QLabel *label_text_endtime, *label_task_name;
    ExtendedLabel *label_selectdatetime;
    QPushButton *button_decline, *button_accept;
    QString task_id, task_name;
    QDateTime task_start_time;
    DateTimeWidget *datetime_widget;
    bool opendatetime_status;
};

#endif
