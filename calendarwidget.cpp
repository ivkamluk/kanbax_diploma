#include "calendarwidget.h"

/*
    this class show for user calendar
    where displaying his all tasks from all boards
*/


CalendarWidget::CalendarWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vbox_main = new QVBoxLayout(this);
    QLabel *label_test = new QLabel(this);

    label_test->setText("Calendar widget");

    vbox_main->addWidget(label_test);

    setLayout(vbox_main);
}
