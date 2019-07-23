#ifndef CREATETASK_H
#define CREATETASK_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

#include "database.h"

class CreateTask : public QWidget
{
    Q_OBJECT
public:
    explicit CreateTask(QWidget *parent = nullptr);

    //Methods @set
        void setDataBase(DataBase &database);
        void setUserId(QString user_id);
        void setBoardId(QString board_id);

        QWidget *getWidgetTask();

        QPushButton* getAcceptButton();
        QPushButton* getDeclineButton();

public slots:
        void addTask();
        void enableButtonAccept();

 private:
    QFormLayout *form_task;
    QVBoxLayout *layout_column;
    DataBase database;
    QString user_id, board_id;
    QWidget *widget_task;
    QLabel *label_task_name, *label_task_description;
    QLineEdit *editline_task_name;
    QTextEdit *text_task_description;
    QPushButton *button_accept, *button_decline;
};

#endif
