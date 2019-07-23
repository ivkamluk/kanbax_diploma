#include "taskwidget.h"
#include "randomcolor.h"

TaskWidget::TaskWidget(QWidget *parent) : ExtendedWidget(parent)
{
    widget_task = new ExtendedWidget(this);
    widget_task->setObjectName("widgetTask");

    vbox_task = new QVBoxLayout;
    widget_task->setLayout(vbox_task);

    // ДОДАТИ ЗМОГУ ЗМІНЮВАТИ КОЛІР НА ВИБІР У НАЛАШТУВАННЯХ
    // для кожного віджета свій вибір

    widget_task->setStyleSheet(StyleQSS::loadQSSfromFile
        (":/styles/QSS/MAC_taskwidget_light.qss"));

    connect(widget_task, &ExtendedWidget::clicked, this, [this] {
                emit openTaskEdit();
    });
}

void TaskWidget::addImageDescription()
{
    label_image_description_enabled = new QLabel;
    label_image_description_enabled->setObjectName("labelImageDescriptionEnb");
    label_image_description_enabled->setFixedSize(20,20);

    hbox_task_top->addWidget(label_image_description_enabled);
    hbox_task_top->setAlignment(label_image_description_enabled, Qt::AlignLeft);

    if (task_description.isEmpty())
        label_image_description_enabled->setHidden(true);
}

void TaskWidget::addLabelTaskName()
{
    label_task_name = new QLabel(this);
    label_task_name->setObjectName("labelTaskName");
    label_task_name->setText(task_name);

    vbox_task_main->addWidget(label_task_name);
}

void TaskWidget::addComboBox()
{
    combobox_task = new ExtendedComboBox;
    combobox_task->setObjectName("comboBoxTask");
    combobox_task->setMaximumWidth(150);
    combobox_task->addItem(tr("Mark as: "));

    hbox_task_top->setMargin(3);
    hbox_task_top->addStretch(1);
    hbox_task_top->addWidget(combobox_task);
    hbox_task_top->setAlignment(label_image_description_enabled, Qt::AlignRight);
}

void TaskWidget::generateColorForTopBackground()
{
    RandomColor randomcolor;
    widget_task_top->setStyleSheet(randomcolor.randomColorStylesheet
                                   ("QWidget#widgetTaskTop"));
}

void TaskWidget::loadWidget()
{
    addImageDescription();
    addLabelTaskName();
    addComboBox();
}

void TaskWidget::setTaskId(QString task_id)
{
    this->task_id = task_id;
}

void TaskWidget::setTaskName(QString task_name)
{
    this->task_name = task_name;
}

void TaskWidget::setTaskDescription(QString task_description)
{
    this->task_description = task_description;
}

QString TaskWidget::getTaskId()
{
    return task_id;
}

QString TaskWidget::getTaskName()
{
    return task_name;
}

QString TaskWidget::getTaskDescription()
{
    return task_description;
}

ExtendedWidget* TaskWidget::getWidgetTask()
{
    return widget_task;
}

ExtendedComboBox* TaskWidget::getComboBox()
{
    return combobox_task;
}

void TaskWidget::descriptionEnabled()
{
    label_image_description_enabled->setHidden(false);
}

void TaskWidget::descriptionDisabled()
{
    label_image_description_enabled->setHidden(true);
}

void TaskWidget::changeTaskName()
{
    label_task_name->setText(task_name);
}

//Events

void TaskWidget::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
