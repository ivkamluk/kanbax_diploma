#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QApplication>
#include <QComboBox>
#include <QMouseEvent>

#include "customwidget/extendedwidget.h"
#include "customwidget/extendedcombobox.h"
#include "styleqss.h"

class TaskWidget : public ExtendedWidget
{
    Q_OBJECT
public:
    explicit TaskWidget(QWidget *parent = nullptr);

    void addImageDescription();
    void addLabelTaskName();
    void addComboBox();

    virtual void generateColorForTopBackground();

    virtual void loadWidget() = 0;
    virtual void addComboBoxItems() = 0;

//Methods @set
    void setTaskId(QString task_id);
    void setTaskName(QString task_name);
    void setTaskDescription(QString task_description);

//Methods @get

    QString getTaskId();
    QString getTaskName();
    QString getTaskDescription();

    ExtendedWidget* getWidgetTask();

    ExtendedComboBox *getComboBox();

    void descriptionEnabled();
    void descriptionDisabled();
    void changeTaskName();

signals:
    void openTaskEdit();

public slots:
     virtual void columnComboBoxSelected() = 0;

protected:
 // Event
    void mousePressEvent(QMouseEvent *event) override;

    QWidget *widget_task_top, *widget_task_main;
    ExtendedWidget *widget_task;
    QVBoxLayout *vbox_task, *vbox_task_main;
    QHBoxLayout *hbox_task_top;
    QLabel *label_image_description_enabled, *label_task_name;
    QString task_id, task_name, task_description;
    ExtendedComboBox *combobox_task;
};

#endif
