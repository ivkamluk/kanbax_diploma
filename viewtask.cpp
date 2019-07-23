#include "viewtask.h"
#include "styleqss.h"
#include "usercorrectnessinput.h"

//delete later
#include <QDebug>
#include <QApplication>
//#include QDesktopWidget


ViewTask::ViewTask(QWidget *parent) : AnimatedDialog(parent), task_remove_confrm(0)
{
    setAttribute(Qt::WA_TranslucentBackground, true);

    setAnimateMiliseconds(0); // let`s disable animation

    frame_viewtask = new QFrame(this);
    frame_viewtask->setObjectName("frameViewTask");

    vbox_main = new QVBoxLayout(this);
    vbox_main->addWidget(frame_viewtask);

    vbox_frame = new QVBoxLayout;
    frame_viewtask->setLayout(vbox_frame);

    setLayout(vbox_main);
}

void ViewTask::setDataBase(DataBase &database)
{
    this->database = database;
}

void ViewTask::setUserId(QString user_id)
{
    this->user_id = user_id;
}

void ViewTask::setTaskId(QString task_id)
{
    this->task_id = task_id;
}

void ViewTask::setTaskName(QString task_name)
{
    this->task_name = task_name;
}

void ViewTask::setTaskDescription(QString task_description)
{
    this->task_description = task_description;
}

void ViewTask::setTaskStartTime(QString task_starttime)
{
    this->task_starttime = task_starttime;
}

void ViewTask::setTaskEndTime(QString task_endtime)
{
    this->task_endtime = task_endtime;
}

void ViewTask::setTaskStatus(TaskStatus status)
{
    this->task_status = status;
}

QString ViewTask::getTaskName()
{
    return task_name;
}

QString ViewTask::getTaskDescription()
{
    return task_description;
}

void ViewTask::loadWidget()
{
    initializeWidgets();

    if (appsettings.getThemeColor() == 1) // Dark
       setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_viewtask_dark.qss"));
    else // Light
       setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_viewtask_light.qss"));
}

void ViewTask::initializeWidgets()
{
    label_dialog_name = new QLabel;
    label_dialog_name->setObjectName("labelDialogName");
    label_dialog_name->setText(tr("My task"));

    label_background_image = new QLabel(this);
    label_background_image->setObjectName("labelBackgroundImage");
    label_background_image->setPixmap(QPixmap(":/image/Image/background_allboards.png"));
    // the maximum size for perfect background
    label_background_image->setMaximumSize(500,150);

    label_alert = new QLabel;
    label_alert->setObjectName("labelAlert");
    label_alert->setText(tr("Are you sure, that you would like to remove this task?\n"
                            "All actions can`t be undone! To remove task click it again!"));
    label_alert->setHidden(true);

    hbox_section_status_time = new QHBoxLayout;

 // section status

    label_task_status = new QLabel;
    label_task_status->setObjectName("labelTaskStatus");

// vline

    frame_vline = new QFrame;
    frame_vline->setObjectName("frameVLine");
    frame_vline->setFrameShape(QFrame::VLine);
    frame_vline->setFrameShadow(QFrame::Plain);
    frame_vline->setFixedWidth(1);

// section endtime

    vbox_section_starttime = new QVBoxLayout;

    label_text_startime = new QLabel;
    label_text_startime->setObjectName("labelTextStartTime");
    label_text_startime->setText(tr("Start time"));

    label_task_starttime = new QLabel;
    label_task_starttime->setObjectName("labelTaskStartTime");
    label_task_starttime->setText(task_starttime);

    vbox_section_starttime->addWidget(label_text_startime);
    vbox_section_starttime->addWidget(label_task_starttime);

    vbox_section_starttime->setAlignment(label_text_startime, Qt::AlignCenter);
    vbox_section_starttime->setAlignment(label_task_starttime, Qt::AlignCenter);

    vbox_section_endtime = new QVBoxLayout;

    label_text_endtime = new QLabel;
    label_text_endtime->setObjectName("labelTextEndTime");
    label_text_endtime->setText(tr("End time"));

    label_task_endtime = new QLabel;
    label_task_endtime->setObjectName("labelTaskEndTime");
    label_task_endtime->setText(task_endtime);

    vbox_section_endtime->addWidget(label_text_endtime);
    vbox_section_endtime->addWidget(label_task_endtime);

    vbox_section_endtime->setAlignment(label_text_endtime, Qt::AlignCenter);
    vbox_section_endtime->setAlignment(label_task_endtime, Qt::AlignCenter);

 // hbox section status and time

    hbox_section_status_time->setMargin(4);
    hbox_section_status_time->addWidget(label_task_status);
    hbox_section_status_time->addWidget(frame_vline);
    hbox_section_status_time->addLayout(vbox_section_starttime);
    hbox_section_status_time->addLayout(vbox_section_endtime);

    hbox_section_status_time->setAlignment(label_task_status, Qt::AlignCenter);

// section task name

    grid_section_task = new QGridLayout;

    label_text_task_name = new QLabel;
    label_text_task_name->setObjectName("labelTextTaskName");
    label_text_task_name->setText(tr("Task name: "));

    editline_task_name = new QLineEdit;
    editline_task_name->setObjectName("editlineTaskName");
    editline_task_name->setText(task_name);
    editline_task_name->setAttribute(Qt::WA_MacShowFocusRect, false);

// section task description

    label_text_task_description = new QLabel;
    label_text_task_description->setObjectName("labelTextTaskDescription");
    label_text_task_description->setText(tr("Task description: "));

    textedit_task_name = new QTextEdit;
    textedit_task_name->setObjectName("textTaskDescription");
    textedit_task_name->setAttribute(Qt::WA_MacShowFocusRect,0);
    textedit_task_name->setText(task_description);

    grid_section_task->setMargin(15);
    grid_section_task->addWidget(label_text_task_name, 0, 0);
    grid_section_task->addWidget(editline_task_name, 0, 1);

    grid_section_task->addWidget(label_text_task_description, 1, 0);
    grid_section_task->addWidget(textedit_task_name, 1, 1);

  // section buttons

    hbox_section_buttons = new QHBoxLayout;

    button_close = new QPushButton(this);
    button_close->setObjectName("buttonClose");
    button_close->setText(tr("Close"));

    button_save = new QPushButton(this);
    button_save->setObjectName("buttonSave");
    button_save->setText(tr("Save"));
    button_save->setEnabled(false);
    button_save->setCursor(Qt::PointingHandCursor);

    label_image_remove_task = new ExtendedLabel(this);
    label_image_remove_task->setObjectName("labelImageRemoveTask");
    label_image_remove_task->setFixedSize(40,40);
    label_image_remove_task->setCursor(Qt::PointingHandCursor);

    label_remove_task = new ExtendedLabel(this);
    label_remove_task->setObjectName("labelRemoveTask");
    label_remove_task->setText(tr("Remove task"));
    label_remove_task->setCursor(Qt::PointingHandCursor);

    hbox_section_buttons->addWidget(label_image_remove_task);
    hbox_section_buttons->addWidget(label_remove_task);
    hbox_section_buttons->addStretch(1);
    hbox_section_buttons->addWidget(button_close);
    hbox_section_buttons->addWidget(button_save);

    hbox_section_buttons->setMargin(10);
    hbox_section_buttons->setSpacing(5);
    hbox_section_buttons->setAlignment(button_close, Qt::AlignRight);
    hbox_section_buttons->setAlignment(button_save, Qt::AlignRight);

    vbox_frame->setMargin(0);
    vbox_frame->addWidget(label_dialog_name);
    vbox_frame->addWidget(label_background_image);
    vbox_frame->addWidget(label_alert);
    vbox_frame->addLayout(hbox_section_status_time);
    vbox_frame->addLayout(grid_section_task);
    vbox_frame->addLayout(hbox_section_buttons);

    vbox_frame->setAlignment(label_dialog_name, Qt::AlignLeft);
    vbox_frame->setAlignment(label_background_image, Qt::AlignHCenter);
    vbox_frame->setAlignment(label_alert, Qt::AlignHCenter);
    vbox_frame->setAlignment(grid_section_task, Qt::AlignLeft);

    if (task_starttime.isEmpty() && task_endtime.isEmpty())
    {
        label_task_starttime->setText("-");
        label_task_endtime->setText("-");
    }

    switch (task_status) {
    case TaskStatus::ToDo:
          label_task_status->setText(tr("To Do"));
          label_task_status->setStyleSheet("background:#DEFEFC;");
          frame_vline->setHidden(true);
          label_text_startime->setHidden(true);
          label_task_starttime->setHidden(true);
          label_text_endtime->setHidden(true);
          label_task_endtime->setHidden(true);
        break;
    case TaskStatus::Doing:
          label_task_status->setText(tr("Doing"));
          label_task_status->setStyleSheet("background:#FAFCC7;");
        break;
    case TaskStatus::Done:
          label_task_status->setText(tr("Done"));
          label_task_status->setStyleSheet("background:#D1F7B0;");
        break;
    }

    connect(label_image_remove_task, SIGNAL(clicked()), this, SLOT(removeTask()));
    connect(label_remove_task, SIGNAL(clicked()), this, SLOT(removeTask()));
    connect(button_close, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(button_save, SIGNAL(clicked(bool)), this, SLOT(updateCurrentTask()));
    connect(editline_task_name, &QLineEdit::textChanged, this, [this] {
        UserCorrectnessInput *correctline = new UserCorrectnessInput(this);
        correctline->inputCheckCorrectLine(editline_task_name->text(), button_save);
    });
    connect(editline_task_name, SIGNAL(textEdited(QString)), this, SLOT(enableButtonSave()));
    connect(textedit_task_name, SIGNAL(textChanged()), this, SLOT(enableButtonSave()));
}

void ViewTask::enableButtonSave()
{
    UserCorrectnessInput *correctline = new UserCorrectnessInput(this);
    correctline->inputCheckCorrectLine(editline_task_name->text(), button_save);
}

void ViewTask::updateCurrentTask()
{
    if (database.isConnected())
    {
        if (editline_task_name->text() != task_name)
        {
            database.updateTaskName(user_id, task_id, editline_task_name->text());
            task_name = editline_task_name->text();
        }

        if (textedit_task_name->toPlainText().isEmpty())
            emit descriptionOff();
        else if (textedit_task_name->toPlainText() != task_description)
        {
            database.updateTaskDescription(user_id, task_id, textedit_task_name->toPlainText());
            task_description = textedit_task_name->toPlainText();
            emit descriptionOn();
        }

        label_alert->setText(tr("Great! Task has been updated."));
        label_alert->setStyleSheet("background: #3AA341; color:white;");
        label_alert->setHidden(false);

        button_save->setEnabled(false);


        emit taskUpdated();
    }
}

void ViewTask::removeTask()
{
    task_remove_confrm++;
    label_alert->setHidden(false);

    if (task_remove_confrm > 1)
    {
        if (database.isConnected())
        {
            database.removeOneTask(user_id, task_id);
            emit taskRemoved();

            // let`s close now
            reject();
        }
    }
}

