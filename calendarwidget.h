#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

/*
    this class show for user calendar
    where displaying his all tasks from all boards
*/

class CalendarWidget : public QWidget
{
    Q_OBJECT
public:

    explicit CalendarWidget(QWidget *parent = nullptr);

signals:

public slots:
};

#endif
