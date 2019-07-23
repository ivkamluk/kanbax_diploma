#ifndef TASKTODOWIDGET_H
#define TASKTODOWIDGET_H

#include <QWidget>
#include <QMouseEvent>

#include "taskwidget.h"
#include "appsettings.h"

class TaskToDoWidget : public TaskWidget
{
    Q_OBJECT
public:
    explicit TaskToDoWidget(QWidget *parent = nullptr);

    void loadWidget() override;
    void addComboBoxItems() override;

signals:
    void clicked();

    void selectedColumnDoing();
    virtual void selectedColumnDone();

public slots:
    void columnComboBoxSelected() override;

protected:

    void mousePressEvent(QMouseEvent *event);

    QString column_name_doing, column_name_done;
    AppSettings appsettings;
};

#endif
