#include "tasktodowidget.h"

//delete later

#include <QDebug>

TaskToDoWidget::TaskToDoWidget(QWidget *parent) : TaskWidget(parent),
    column_name_doing(tr("Doing")), column_name_done(tr("Done"))
{
    widget_task_top = new QWidget(this);
    widget_task_top->setObjectName("widgetTaskTop");

    widget_task_main = new QWidget(this);
    widget_task_main->setObjectName("widgetTaskMain");

    hbox_task_top = new QHBoxLayout(this);
    vbox_task_main = new QVBoxLayout(this);

    widget_task_top->setLayout(hbox_task_top);
    widget_task_main->setLayout(vbox_task_main);

    vbox_task->setMargin(0);
    vbox_task->setSpacing(0);

    vbox_task->addWidget(widget_task_top);
    vbox_task->addWidget(widget_task_main);
}

void TaskToDoWidget::loadWidget()
{
    addImageDescription();
    addLabelTaskName();
    addComboBox();
    addComboBoxItems();

    if (appsettings.getOptionRandomColorToDo() == true)
         generateColorForTopBackground();

    connect(combobox_task, SIGNAL(activated(int)), this, SLOT(columnComboBoxSelected()));
}

void TaskToDoWidget::addComboBoxItems()
{
    combobox_task->addItem(tr("Doing"));
    combobox_task->addItem(tr("Done"));
}

void TaskToDoWidget::columnComboBoxSelected()
{
    if (combobox_task->currentText() == column_name_doing)
    {
        qDebug()<<"selected Doing";
        emit selectedColumnDoing();
    }
    else if (combobox_task->currentText() == column_name_done)
    {
         qDebug()<<"selected Done";
        emit selectedColumnDone();
    }
}

void TaskToDoWidget::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
