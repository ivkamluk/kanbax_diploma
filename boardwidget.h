#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QList>
#include <QScrollArea>
#include <QStackedWidget>
#include <QListWidget>
#include <QMainWindow>

#include "database.h"
#include "databaseerror.h"
#include "customwidget/extendedwidget.h"
#include "createtask.h"
#include "tasktodowidget.h"
#include "taskdoingwidget.h"
#include "taskdonewidget.h"
#include "boardsettings.h"
#include "selecttasktime.h"
#include "appsettings.h"

/*
    this class show all functionality
    for user current selected board
*/

class BoardWidget : public ExtendedWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = nullptr);

 //Methods @set
    void setBoardId(QString board_id);
    void setUserId(QString user_id);
    void setDataBase(DataBase &database);
    void setStackedWidget(QStackedWidget *stackwidget);
    void setListWidget(QListWidget *listwidget);
    void setResizing(QMainWindow *window_main);

 //Methods @get
    QString getBoardId();
    QString getUserId();

    void loadWidget();
    void updateCountColumnToDo();
    void updateCountColumnDoing();
    void updateCountColumnDone();

    void showInfoTasksTips();

    void initializeBoard();

    void emptyBoardWidget();

    QWidget* infoColumnAddTasks();
    QWidget* infoColumnMoveTask(QVBoxLayout *vbox_column);

    void showNotification(QString task_name);

    void retrieveTaskToDoWidgets();
    void retrieveTaskDoingWidgets();
    void retrieveTaskDoneWidgets();

    TaskToDoWidget* createTaskToDoWidget(QString task_id, QString task_name,
                              QString task_description);

    TaskDoingWidget* createTaskDoingWidget(QString task_id, QString task_name,
                                           QString task_description, QString task_end_time,
                                           QString task_start_time);

    TaskDoneWidget* createTaskDoneWidget(QString task_id, QString task_name,
                                         QString task_description, bool overbyuser);

    void operateDataTaskToDoWidgets(TaskToDoWidget* task_object);
    void operateDataTaskDoingWidgets(TaskDoingWidget* task_object);
    void operateDataTaskDoneWidgets(TaskDoneWidget* task_object);

signals:

public slots:
    void createNewTask();
    void acceptNewTask();
    void declineNewTask();

    void showBoardSettings();

    void updateBoard();
    void updateBoardTasks();

private:
    QVBoxLayout *vbox_main, *vbox_column_todo, *vbox_column_doing, *vbox_column_done,
    *vbox_header_tasks_todo, *vbox_header_tasks_doing, *vbox_header_tasks_done;
    QHBoxLayout *hbox_board_columns, *hbox_header_todo, *hbox_header_doing, *hbox_header_done, *hbox_board_title, *hbox_tip_addtask, *hbox_tip_movetask;
    QString board_id, user_id, board_name;
    DataBase database;
    DataBaseError *databaseerror;
    QLabel *label_empty_board, *label_image_empty_board,*label_header_todo, *label_header_doing, *label_header_done,
    *label_tasks_count_todo, *label_tasks_count_doing, *label_image_tasks_accpt_done,
    *label_image_tasks_wait_done, *label_tasks_count_accpt_done,*label_tasks_count_wait_done,
    *label_image_tip_addtask, *label_tip_addtask, *label_image_movetask, *label_tip_movetask;
    QWidget *widget_columns, *widget_header_todo, *widget_header_doing, *widget_header_done,
    *widget_board_title, *widget_header_tasks_todo, *widget_header_tasks_doing,
    *widget_header_tasks_done, *widget_tip_addtask, *widget_tip_movetask, *widget_todo_tip_add,
    *widget_todo_tip_move, *widget_doing_tip_move, *widget_done_tip_move;
    ExtendedWidget *widget_column_todo, *widget_column_doing, *widget_column_done, *widget_task;
    ExtendedLabel *label_image_addtask, *label_image_editboard, *label_board_name;
    QList<TaskToDoWidget *> list_tasks_todo;
    QList<TaskDoingWidget *> list_tasks_doing;
    QList<TaskDoneWidget *> list_tasks_done;
    CreateTask *task_creation;
    bool board_notification_sound;
    bool task_is_creating;
    TaskToDoWidget *task_todo_widget;
    QScrollArea *scrollarea_todo, *scrollarea_doing, *scrollarea_done;
    QStackedWidget *stacked_widget_current_window;
    QListWidget *list_widget_menu;
    QMainWindow *window_main;
    BoardSettings *boardsettings;
    AppSettings appsettings;
    unsigned short tasks_done_byuser, tasks_done_bytimer;
};

#endif
