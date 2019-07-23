#ifndef NOTIFICATIONTASKDONE_H
#define NOTIFICATIONTASKDONE_H

#include <QDialog>
#include <QFrame>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QLabel>

#include "customwidget/extendedlabel.h"

class NotificationTaskDone : public QDialog
{
    Q_OBJECT
public:
    explicit NotificationTaskDone(QString task_name, QString board_name, QWidget *parent = nullptr);

    void moveNotification (unsigned int pos);

private:
    QFrame *frame_notification, *frame_line;
    QVBoxLayout *vbox_main, *vbox_section_task;
    QHBoxLayout *hbox_section, *hbox_section_task;
    QTimer *timer_close_notification;
    QString task_name, board_name;
    QWidget *widget_section_task;
    QLabel *label_image_done, *label_board_name, *label_task_name, *label_logo_name;
    ExtendedLabel *label_close;
};

#endif
