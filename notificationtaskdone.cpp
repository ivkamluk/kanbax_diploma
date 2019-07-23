#include "notificationtaskdone.h"
#include "styleqss.h"

#include <QRect>
#include <QApplication>
#include <QDesktopWidget>

NotificationTaskDone::NotificationTaskDone(QString task_name, QString board_name, QWidget *parent) :
    QDialog(parent), task_name(task_name), board_name(board_name)
{

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    vbox_main = new QVBoxLayout(this);

    frame_notification = new QFrame(this);
    frame_notification->setObjectName("frameNotification");


    vbox_main->addWidget(frame_notification);

    hbox_section = new QHBoxLayout(this);
    frame_notification->setLayout(hbox_section);

// section 1
    widget_section_task = new QWidget(this);
    vbox_section_task = new QVBoxLayout(this);

    widget_section_task->setLayout(vbox_section_task);
    widget_section_task->setMinimumWidth(150);

    label_logo_name = new QLabel(this);
    label_logo_name->setObjectName("labelLogo");
    label_logo_name->setText("KanbaX");

    label_board_name = new QLabel(this);
    label_board_name->setObjectName("labelBoardName");
    label_board_name->setText(board_name);

    hbox_section_task = new QHBoxLayout(this);

    vbox_section_task->addWidget(label_logo_name);
    vbox_section_task->addWidget(label_board_name);
    vbox_section_task->addSpacing(-5);
    vbox_section_task->addLayout(hbox_section_task);

    vbox_section_task->setAlignment(label_board_name, Qt::AlignCenter);

    label_image_done = new QLabel(this);
    label_image_done->setObjectName("labelImageDone");
    label_image_done->setFixedSize(30,30);

    label_task_name = new QLabel(this);
    label_task_name->setObjectName("labelTaskName");
    label_task_name->setText(task_name);

    hbox_section_task->addWidget(label_image_done);
    hbox_section_task->addWidget(label_task_name);

    vbox_section_task->setAlignment(hbox_section_task, Qt::AlignCenter);

 // section 2

    frame_line = new QFrame(this);
    frame_line->setFrameShape(QFrame::VLine);

// section 3

    label_close = new ExtendedLabel(this);
    label_close->setObjectName("labelClose");
    label_close->setText(tr("Close"));

    hbox_section->addWidget(widget_section_task);
    hbox_section->addWidget(frame_line);
    hbox_section->addWidget(label_close);

    timer_close_notification = new QTimer(this);
    timer_close_notification->setInterval(10000); // 10 sec
    timer_close_notification->start();

    vbox_section_task->setMargin(0);
    hbox_section->setMargin(10);

    vbox_main->setMargin(0);

    setLayout(vbox_main);

    connect(timer_close_notification, SIGNAL(timeout()), this, SLOT(reject()));
    connect(label_close, SIGNAL(clicked()), this, SLOT(reject()));

    setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_notificationtaskdone_light.qss"));
}

void NotificationTaskDone::moveNotification(unsigned int pos)
{
    QRect screenpos = QApplication::desktop()->screenGeometry();
    if (pos == 0) // Top-Left
        move(70,0);

    else if (pos == 1) // Top-Right
    {
        int width = screenpos.width() - 320;
        move(width,0);
    }

    else if (pos == 2) // Bottom-Left
    {
       int height = screenpos.height()-100;
        move(70,height);
    }

    else if (pos == 3) // Bottom-Right
    {
       int height = screenpos.height()-100;
       int width = screenpos.width() - 320;
        move(width,height);
    }

}
