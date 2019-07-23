#include "taskdonewidget.h"
#include "styleqss.h"

//delete later
#include <QDebug>

TaskDoneWidget::TaskDoneWidget(QWidget *parent) : TaskWidget(parent),
    column_name_todo(tr("To Do")), column_name_doing(tr("Doing"))
{
    widget_background = new QWidget;
    widget_background->setObjectName("widgetBackground");

    vbox_background = new QVBoxLayout;
    widget_background->setLayout(vbox_background);

    widget_task_top = new QWidget;
    widget_task_top->setObjectName("widgetTaskTop");

    widget_task_main = new QWidget;
    widget_task_main->setObjectName("widgetTaskMain");

    hbox_task_top = new QHBoxLayout;
    vbox_task_main = new QVBoxLayout;

    widget_task_top->setLayout(hbox_task_top);
    widget_task_main->setLayout(vbox_task_main);

 // they must be inside widget_background

   vbox_background->setMargin(0);
   vbox_background->addWidget(widget_task_top);
   vbox_background->addWidget(widget_task_main);

   vbox_task->setMargin(0);
   vbox_task->addWidget(widget_background);

   widget_task->setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_taskdonewidget_light.qss"));
}

void TaskDoneWidget::loadWidget()
{
    addImageDescription();
    addLabelTaskName();
    addComboBox();
    addComboBoxItems();
    confirmationDone();

   connect(combobox_task, SIGNAL(activated(int)), this, SLOT(columnComboBoxSelected()));
}


void TaskDoneWidget::addComboBoxItems()
{
    combobox_task->addItem(tr("To Do"));
    combobox_task->addItem(tr("Doing"));
}

void TaskDoneWidget::generateColorForTopBackground()
{}

void TaskDoneWidget::columnComboBoxSelected()
{
    if (combobox_task->currentText() == column_name_todo)
    {
        qDebug()<<"selected To Do";
        emit selectedColumnToDo();
    }
    else if (combobox_task->currentText() == column_name_doing)
    {
         qDebug()<<"selected Doing";
         emit selectedColumnDoing();
    }
}

QVBoxLayout* TaskDoneWidget::getLayoutMain()
{
    return vbox_task;
}

void TaskDoneWidget::setTaskOverByUser(bool task_over_byuser)
{
    this->task_over_byuser = task_over_byuser;
}

void TaskDoneWidget::setTaskStartTime(QDateTime task_starttime)
{
    this->task_starttime = task_starttime;
}

void TaskDoneWidget::setTaskEndTime(QDateTime task_endtime)
{
    this->task_endtime = task_endtime;
}

QDateTime TaskDoneWidget::getTaskStartTime()
{
    return task_starttime;
}

QDateTime TaskDoneWidget::getTaskEndTime()
{
    return task_endtime;
}

void TaskDoneWidget::confirmationDone()
{
    label_image_done = new QLabel;
    label_image_done->setObjectName("labelImageDone");
    label_image_done->setFixedSize(40,40);
    vbox_task->addSpacing(-30);
    vbox_task->addWidget(label_image_done);
    vbox_task->setAlignment(label_image_done, Qt::AlignCenter);

    if (task_over_byuser == false)
    {
        label_image_done->setStyleSheet("#labelImageDone"
               " { border-image:url(':/image/Image/taskdone_bytimer.png');}");

        label_confirmation_text = new QLabel;
        label_confirmation_text->setObjectName("labelConfirmationText");
        label_confirmation_text->setText(tr("Confirm as done?"));
        vbox_task->addWidget(label_confirmation_text);
        vbox_task->setAlignment(label_confirmation_text, Qt::AlignCenter);
        vbox_task->addSpacing(0);

  // section confirm buttons

        hbox_section_confirm_buttons = new QHBoxLayout;
        vbox_task->addLayout(hbox_section_confirm_buttons);

        button_confirm_no = new QPushButton;
        button_confirm_no->setObjectName("buttonConfirmNo");
        button_confirm_no->setText(tr("No"));

        button_confirm_yes = new QPushButton;
        button_confirm_yes->setObjectName("buttonConfirmYes");
        button_confirm_yes->setText(tr("Yes"));

        hbox_section_confirm_buttons->setMargin(0);
        hbox_section_confirm_buttons->setSpacing(90);
        hbox_section_confirm_buttons->addStretch(1);
        hbox_section_confirm_buttons->addWidget(button_confirm_no);
        hbox_section_confirm_buttons->addWidget(button_confirm_yes);
        hbox_section_confirm_buttons->addStretch(1);

        connect(button_confirm_no, &QPushButton::clicked, this, [this]{
                emit taskConfirmNo();
        });

        connect(button_confirm_yes, &QPushButton::clicked, this, [this]{
                emit taskConfirmYes();
        });
    }
}

bool TaskDoneWidget::isTaskOverByUser()
{
    return task_over_byuser;
}

void TaskDoneWidget::markAsDoneByUser()
{
    vbox_task->removeWidget(label_image_done);
    vbox_task->removeWidget(label_confirmation_text);
    vbox_task->removeItem(hbox_section_confirm_buttons);
    label_image_done->setHidden(true);

    task_over_byuser = true;

    vbox_task->addSpacing(30);
    confirmationDone();
}
