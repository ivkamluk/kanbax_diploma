#include "createtask.h"
#include "styleqss.h"
#include "usercorrectnessinput.h"

CreateTask::CreateTask(QWidget *parent) : QWidget(parent)
{
    widget_task = new QFrame(this);
    widget_task->setObjectName("widgetTask");

    form_task = new QFormLayout(this);
    widget_task->setLayout(form_task);

    label_task_name = new QLabel(this);
    label_task_name->setObjectName("labelTaskName");
    label_task_name->setText(tr("Task name: "));

    label_task_description = new QLabel(this);
    label_task_description->setObjectName("labelTaskDescription");
    label_task_description->setText(tr("Description: "));

    editline_task_name = new QLineEdit(this);
    editline_task_name->setObjectName("editlineTaskName");
    editline_task_name->setAttribute(Qt::WA_MacShowFocusRect,0);

    text_task_description = new QTextEdit(this);
    text_task_description->setObjectName("textTaskDescription");
    text_task_description->setAttribute(Qt::WA_MacShowFocusRect,0);

    button_decline = new QPushButton(this);
    button_decline->setObjectName("buttonDecline");
    button_decline->setText(tr("Decline"));

    button_accept = new QPushButton(this);
    button_accept->setObjectName("buttonAccept");
    button_accept->setText(tr("Accept"));
    button_accept->setEnabled(false);
    button_accept->setCursor(Qt::PointingHandCursor);

    form_task->addRow(label_task_name,editline_task_name);
    form_task->addRow(label_task_description,text_task_description);
    form_task->addRow(button_decline, button_accept);

    form_task->setFormAlignment(Qt::AlignLeft);
    form_task->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);


    widget_task->setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_createtask_light.qss"));

    connect(editline_task_name, SIGNAL(textEdited(QString)), this, SLOT(enableButtonAccept()));

    widget_task->setFixedHeight(200);
}

QWidget* CreateTask::getWidgetTask()
{
    return widget_task;
}

void CreateTask::setDataBase(DataBase &database)
{
    this->database = database;
}

void CreateTask::setUserId(QString user_id)
{
    this->user_id = user_id;
}

void CreateTask::setBoardId(QString board_id)
{
    this->board_id = board_id;
}

QPushButton* CreateTask::getAcceptButton()
{
    return button_accept;
}

QPushButton* CreateTask::getDeclineButton()
{
    return button_decline;
}

void CreateTask::addTask()
{
    if (database.isConnected())
    {
         database.writeUserDataTTask(user_id, board_id, editline_task_name->text(),
         text_task_description->toPlainText(), 1);
    }
}

void CreateTask::enableButtonAccept()
{
    UserCorrectnessInput *correctinput = new UserCorrectnessInput(this);
    correctinput->inputCheckCorrectLine(editline_task_name->text(), button_accept);
}
