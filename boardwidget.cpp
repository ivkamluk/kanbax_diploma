#include "boardwidget.h"
#include "styleqss.h"
#include "notificationtaskdone.h"
#include "viewtask.h"
#include "workwindow.h"
#include "globalpositioning.h"


#include <QMediaPlayer>
#include <QUrl>
#include <QModelIndex>
#include <QVariant>
#include <QVariantList>

/*
    this class show all functionality
    for user current selected board
*/

//delete later
#include <QDebug>

BoardWidget::BoardWidget(QWidget *parent) : ExtendedWidget(parent), task_is_creating(false),
    tasks_done_byuser(0), tasks_done_bytimer(0)
{
    databaseerror = new DataBaseError(this);

    vbox_main = new QVBoxLayout;
    setLayout(vbox_main);

    setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_boardwindow_light.qss"));
}

//Methods @set
void BoardWidget::setBoardId(QString board_id)
{
    this->board_id = board_id;
}

void BoardWidget::setUserId(QString user_id)
{
    this->user_id = user_id;
}

void BoardWidget::setDataBase(DataBase &database)
{
    this->database = database;
}

void BoardWidget::setStackedWidget(QStackedWidget *stackwidget)
{
    this->stacked_widget_current_window = stackwidget;
}

void BoardWidget::setListWidget(QListWidget *listwidget)
{
    this->list_widget_menu = listwidget;
}

void BoardWidget::setResizing(QMainWindow *window_main)
{
    this->window_main = window_main;
}

//Methods @get
QString BoardWidget::getBoardId()
{
    return board_id;
}

QString BoardWidget::getUserId()
{
    return user_id;
}

void BoardWidget::loadWidget()
{
    if (database.isConnected())
    {
        // but if we open app, and begin with this menu
        // so it will load last user modified board
        if (board_id.isEmpty())
            board_id = database.getUserModifiedBoardId(user_id);

        // now again let`s check if board is not empty
        // it can be empty if we don`t have any board
        if (board_id.isEmpty())
            emptyBoardWidget();
        else
        {
            // now let`s take board_name and notification_sound
            QPair<QString, bool> board = database.getUserBoardNameNotification(user_id, board_id);
            board_name = board.first;
            board_notification_sound = board.second;

            initializeBoard();
            database.updateBoardDateModification(user_id, board_id);
        }
    }
    else
      databaseerror->exec();
}

void BoardWidget::initializeBoard()
{
    widget_board_title = new QWidget;
    widget_board_title->setObjectName("widgetBoardTitle");

    hbox_board_title = new QHBoxLayout;
    hbox_board_title->setMargin(0);

    widget_board_title->setLayout(hbox_board_title);

    label_board_name = new ExtendedLabel(this);
    label_board_name->setText(board_name);
    label_board_name->setObjectName("labelBoardName");
    label_board_name->setAlignment(Qt::AlignRight);

    label_image_editboard = new ExtendedLabel(this);
    label_image_editboard->setObjectName("labelImageEditBoard");
    label_image_editboard->setFixedSize(20,20);

    hbox_board_title->addWidget(label_board_name);
    hbox_board_title->addWidget(label_image_editboard);

    widget_columns = new QWidget;
    hbox_board_columns = new QHBoxLayout;

    widget_columns->setLayout(hbox_board_columns);
    widget_columns->setObjectName("widgetColumns");

    hbox_board_columns->setMargin(0);

    widget_column_todo = new ExtendedWidget;
    widget_column_doing = new ExtendedWidget;
    widget_column_done = new ExtendedWidget;

    widget_column_todo->setObjectName("widgetColumnToDo");
    widget_column_doing->setObjectName("widgetColumnDoing");
    widget_column_done->setObjectName("widgetColumnDone");

    hbox_board_columns->addWidget(widget_column_todo);
    hbox_board_columns->addWidget(widget_column_doing);
    hbox_board_columns->addWidget(widget_column_done);

    vbox_column_todo = new QVBoxLayout;
    vbox_column_doing = new QVBoxLayout;
    vbox_column_done = new QVBoxLayout;

    widget_column_todo->setLayout(vbox_column_todo);
    widget_column_doing->setLayout(vbox_column_doing);
    widget_column_done->setLayout(vbox_column_done);

    widget_header_todo = new QWidget;
    widget_header_doing = new QWidget;
    widget_header_done = new QWidget;

    widget_header_todo->setObjectName("widgetHeaderToDo");
    widget_header_doing->setObjectName("widgetHeaderDoing");
    widget_header_done->setObjectName("widgetHeaderDone");

    // the best max height for dispalying label headers
    widget_header_todo->setFixedHeight(45);
    widget_header_doing->setFixedHeight(45);
    widget_header_done->setFixedHeight(45);

    vbox_column_todo->addWidget(widget_header_todo);
    vbox_column_doing->addWidget(widget_header_doing);
    vbox_column_done->addWidget(widget_header_done);

    vbox_column_todo->setMargin(0);
    vbox_column_doing->setMargin(0);
    vbox_column_done->setMargin(0);

    vbox_column_todo->setSpacing(0);
    vbox_column_doing->setSpacing(0);
    vbox_column_done->setSpacing(0);

    hbox_header_todo = new QHBoxLayout;
    hbox_header_doing = new QHBoxLayout;
    hbox_header_done = new QHBoxLayout;

    widget_header_todo->setLayout(hbox_header_todo);
    widget_header_doing->setLayout(hbox_header_doing);
    widget_header_done->setLayout(hbox_header_done);

    label_header_todo = new QLabel;
    label_header_doing = new QLabel;
    label_header_done = new QLabel;

    label_header_todo->setText(tr("To Do"));
    label_header_doing->setText(tr("Doing"));
    label_header_done->setText(tr("Done"));

    label_header_todo->setObjectName("labelHeaderToDo");
    label_header_doing->setObjectName("labelHeaderDoing");
    label_header_done->setObjectName("labelHeaderDone");

    label_image_addtask = new ExtendedLabel(this);

    label_image_addtask->setFixedSize(25,25);
    label_image_addtask->setObjectName("labelImageAddTask");

    label_tasks_count_todo = new QLabel(this);
    label_tasks_count_todo->setObjectName("labelTasksCountToDo");

    label_tasks_count_doing = new QLabel(this);
    label_tasks_count_doing->setObjectName("labelTasksCountDoing");

    label_image_tasks_accpt_done = new QLabel;
    label_image_tasks_accpt_done->setObjectName("labelImageTasksAccptDone");
    label_image_tasks_accpt_done->setFixedSize(15,15);

    label_tasks_count_accpt_done = new QLabel;
    label_tasks_count_accpt_done->setObjectName("labelTasksCountAccptDone");

    label_image_tasks_wait_done = new QLabel;
    label_image_tasks_wait_done->setObjectName("labelImageTasksWaitDone");
    label_image_tasks_wait_done->setFixedSize(15,15);

    label_tasks_count_wait_done = new QLabel;
    label_tasks_count_wait_done->setObjectName("labelTasksCountWaitDone");

    hbox_header_todo->addWidget(label_header_todo);
    hbox_header_todo->addWidget(label_tasks_count_todo);
    hbox_header_todo->addStretch(1);
    hbox_header_todo->addWidget(label_image_addtask);

    hbox_header_doing->addWidget(label_header_doing);
    hbox_header_doing->addWidget(label_tasks_count_doing);
    hbox_header_doing->addStretch(1);

    hbox_header_done->addWidget(label_header_done);
    hbox_header_done->addWidget(label_image_tasks_accpt_done);
    hbox_header_done->addWidget(label_tasks_count_accpt_done);
    hbox_header_done->addWidget(label_image_tasks_wait_done);
    hbox_header_done->addWidget(label_tasks_count_wait_done);
    hbox_header_done->addStretch(1);

//    scrollarea_todo = new QScrollArea;
//    scrollarea_doing = new QScrollArea;
//    scrollarea_done = new QScrollArea;

//    scrollarea_todo->setWidgetResizable(true);
//    scrollarea_doing->setWidgetResizable(true);
//    scrollarea_done->setWidgetResizable(true);

    widget_header_tasks_todo = new QWidget;
    widget_header_tasks_todo->setObjectName("widgetHeaderTasksToDo");

    vbox_header_tasks_todo = new QVBoxLayout;
    widget_header_tasks_todo->setLayout(vbox_header_tasks_todo);

    widget_header_tasks_doing = new QWidget;
    widget_header_tasks_doing->setObjectName("widgetHeaderTasksDoing");

    vbox_header_tasks_doing = new QVBoxLayout;
    widget_header_tasks_doing->setLayout(vbox_header_tasks_doing);

    widget_header_tasks_done = new QWidget;
    widget_header_tasks_done->setObjectName("widgetHeaderTasksDone");

    vbox_header_tasks_done = new QVBoxLayout;
    widget_header_tasks_done->setLayout(vbox_header_tasks_done);

//    scrollarea_todo->setWidget(widget_header_tasks_todo);
//    scrollarea_doing->setWidget(widget_header_tasks_doing);
//    scrollarea_done->setWidget(widget_header_tasks_done);

//    vbox_column_todo->addWidget(scrollarea_todo);
//    vbox_column_doing->addWidget(scrollarea_doing);
//    vbox_column_done->addWidget(scrollarea_done);

    vbox_column_todo->addWidget(widget_header_tasks_todo);
    vbox_column_doing->addWidget(widget_header_tasks_doing);
    vbox_column_done->addWidget(widget_header_tasks_done);

    vbox_header_tasks_todo->setMargin(10);
    vbox_header_tasks_doing->setMargin(10);
    vbox_header_tasks_done->setMargin(10);

    // we need to do this, because if we have a lot of tasks inside ToDo/Doing/Done
    // every of this widgets will growing, but we don`t need it

    // we just need to make growing of widget, when it has a task
   // not everyone should grows, just one with task

//    widget_column_todo->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
//    widget_column_doing->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
//    widget_column_done->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

//    widget_column_todo->setMinimumHeight(100);
//    widget_column_todo->setMaximumHeight(900);

    hbox_board_columns->setAlignment(widget_column_todo, Qt::AlignTop);
    hbox_board_columns->setAlignment(widget_column_doing, Qt::AlignTop);
    hbox_board_columns->setAlignment(widget_column_done, Qt::AlignTop);

    //main layout for this widget
  //  vbox_main->setMargin(5);
    vbox_main->addWidget(widget_board_title);
    vbox_main->addWidget(widget_columns);

    vbox_main->addStretch(1);

    retrieveTaskToDoWidgets();
    retrieveTaskDoingWidgets();
    retrieveTaskDoneWidgets();

    showInfoTasksTips();

    connect(label_image_addtask, SIGNAL(clicked()), this, SLOT(createNewTask()));
    connect(label_image_editboard, SIGNAL(clicked()), this, SLOT(showBoardSettings()));
    connect(label_board_name, SIGNAL(clicked()), this, SLOT(showBoardSettings()));
}

void BoardWidget::updateCountColumnToDo()
{
    label_tasks_count_todo->setText("("+QString::number(list_tasks_todo.size())+")");
}

void BoardWidget::updateCountColumnDoing()
{
    label_tasks_count_doing->setText("("+QString::number(list_tasks_doing.size())+")");
}

void BoardWidget::updateCountColumnDone()
{
   label_tasks_count_accpt_done->setText("("+QString::number(tasks_done_byuser)+")");
   label_tasks_count_wait_done->setText("("+QString::number(tasks_done_bytimer)+")");
}

void BoardWidget::showInfoTasksTips()
{
    if (list_tasks_todo.isEmpty() && (!list_tasks_doing.isEmpty() || !list_tasks_done.isEmpty()))
        widget_todo_tip_move->setHidden(false);
    else if (list_tasks_todo.isEmpty() && list_tasks_doing.isEmpty() &&
            list_tasks_done.isEmpty())
        widget_todo_tip_add->setHidden(false);


    if (list_tasks_doing.isEmpty() && (!list_tasks_todo.isEmpty() || !list_tasks_done.isEmpty()))
        widget_doing_tip_move->setHidden(false);


    if (list_tasks_done.isEmpty() && (!list_tasks_todo.isEmpty() || !list_tasks_doing.isEmpty()))
        widget_done_tip_move->setHidden(false);

}

void BoardWidget::emptyBoardWidget()
{
    label_empty_board = new QLabel;
    label_empty_board->setObjectName("labelEmptyBoardsWidget");
    label_empty_board->setText(tr("Have you create your own board?"));

    label_image_empty_board = new QLabel;
    label_image_empty_board->setPixmap(QPixmap(":/image/Image/empty_boardwidget.png"));
    label_image_empty_board->setAlignment(Qt::AlignTop | Qt::AlignCenter);

    vbox_main->addSpacing(50);
    vbox_main->addWidget(label_empty_board);
    vbox_main->addWidget(label_image_empty_board);

    vbox_main->setStretchFactor(label_image_empty_board,1);

    vbox_main->setAlignment(label_empty_board, Qt::AlignCenter);
}

QWidget* BoardWidget::infoColumnAddTasks()
{
    QWidget *widget_tip_addtask = new QWidget;
    hbox_tip_addtask = new QHBoxLayout;
    widget_tip_addtask->setLayout(hbox_tip_addtask);

    hbox_tip_addtask->setMargin(0);
    vbox_header_tasks_todo->addWidget(widget_tip_addtask);

    label_image_tip_addtask = new QLabel;
    label_image_tip_addtask->setObjectName("labelImageTipAddTask");
    label_image_tip_addtask->setFixedSize(50,50);

    label_tip_addtask = new QLabel;
    label_tip_addtask->setObjectName("labelTipAddTask");
    label_tip_addtask->setText(tr("Create your first task"));

    hbox_tip_addtask->addStretch(1);
    hbox_tip_addtask->addWidget(label_image_tip_addtask);
    hbox_tip_addtask->setSpacing(5);
    hbox_tip_addtask->addWidget(label_tip_addtask);
    hbox_tip_addtask->addStretch(1);

    hbox_tip_addtask->setAlignment(label_image_tip_addtask, Qt::AlignVCenter);
    hbox_tip_addtask->setAlignment(label_tip_addtask, Qt::AlignVCenter);

    vbox_header_tasks_todo->setAlignment(hbox_tip_addtask, Qt::AlignVCenter);

    return widget_tip_addtask;
}

QWidget* BoardWidget::infoColumnMoveTask(QVBoxLayout *layout)
{
    QWidget* widget_tip_movetask = new QWidget;
    hbox_tip_movetask = new QHBoxLayout;
    widget_tip_movetask->setLayout(hbox_tip_movetask);

    hbox_tip_movetask->setMargin(0);
    layout->addWidget(widget_tip_movetask);

    label_image_movetask = new QLabel;
    label_image_movetask->setObjectName("labelImageMoveTask");
    label_image_movetask->setFixedSize(50,50);

    label_tip_movetask = new QLabel;
    label_tip_movetask->setObjectName("labelTipMoveTask");
    label_tip_movetask->setText(tr("Move your task here"));

    hbox_tip_movetask->addStretch(1);
    hbox_tip_movetask->addWidget(label_image_movetask);
    hbox_tip_movetask->setSpacing(5);
    hbox_tip_movetask->addWidget(label_tip_movetask);
    hbox_tip_movetask->addStretch(1);

    hbox_tip_movetask->setAlignment(label_image_movetask, Qt::AlignVCenter);
    hbox_tip_movetask->setAlignment(label_tip_movetask, Qt::AlignVCenter);

    layout->setAlignment(hbox_tip_movetask, Qt::AlignCenter);

    return widget_tip_movetask;
}

void BoardWidget::showNotification(QString task_name)
{
    if (board_notification_sound != false)
    {
        QMediaPlayer *sound = new QMediaPlayer(this);
        sound->setMedia(QUrl("qrc:/sound/Sound/notification_sound_1.wav"));
        sound->play();
     }

    NotificationTaskDone *notification = new NotificationTaskDone(task_name, board_name,
                                                                  this);
    notification->moveNotification(appsettings.getNotificationSide());
    notification->show();
}

void BoardWidget::createNewTask()
{
  if (task_is_creating == false)
  {
       task_creation = new CreateTask;
       vbox_header_tasks_todo->insertWidget(0, task_creation->getWidgetTask());
       task_creation->setDataBase(database);
       task_creation->setUserId(user_id);
       task_creation->setBoardId(board_id);
       task_is_creating = true;

       connect(task_creation->getAcceptButton(), SIGNAL(clicked(bool)), this, SLOT(acceptNewTask()));
       connect(task_creation->getDeclineButton(), SIGNAL(clicked(bool)), this, SLOT(declineNewTask()));
  }
}

void BoardWidget::acceptNewTask()
{
     task_creation->addTask();
     task_creation->getWidgetTask()->setHidden(true);
     vbox_header_tasks_todo->removeWidget(task_creation->getWidgetTask());
     task_is_creating = false;

     if (database.isConnected())
     {
         QStringList user_tasks = database.getUserLastAddedTask(user_id, board_id);
                                // 0 - task_id    // 1 - task_name   // 2 - task_description
         TaskToDoWidget *todotask = createTaskToDoWidget(user_tasks.at(0), user_tasks.at(1), user_tasks.at(2));
         vbox_header_tasks_todo->insertWidget(0, todotask->getWidgetTask());
     }

     if (!list_tasks_todo.isEmpty())
         widget_todo_tip_add->setHidden(true);

     if (list_tasks_doing.isEmpty() && !list_tasks_todo.isEmpty())
         widget_doing_tip_move->setHidden(false);

     if (list_tasks_done.isEmpty() && !list_tasks_todo.isEmpty())
         widget_done_tip_move->setHidden(false);

     if (!list_tasks_todo.isEmpty() && (!list_tasks_doing.isEmpty() || !list_tasks_done.isEmpty()))
         widget_todo_tip_move->setHidden(true);

     updateCountColumnToDo();
}

void BoardWidget::declineNewTask()
{
    task_creation->getWidgetTask()->setHidden(true);
    task_is_creating = false;
    vbox_header_tasks_todo->removeWidget(task_creation->getWidgetTask());
    updateCountColumnToDo();
}

TaskToDoWidget* BoardWidget::createTaskToDoWidget(QString task_id, QString task_name,
                                       QString task_description)
{
    TaskToDoWidget *todowidget = new TaskToDoWidget;
            // 0 - task_id   1 - task_name 2 - task_description
    todowidget->setTaskId(task_id);
    todowidget->setTaskName(task_name);
    todowidget->setTaskDescription(task_description);
    todowidget->loadWidget();
    list_tasks_todo.push_back(todowidget);

    operateDataTaskToDoWidgets(todowidget);

    return todowidget;
}

TaskDoingWidget* BoardWidget::createTaskDoingWidget(QString task_id, QString task_name,
    QString task_description, QString task_end_time, QString task_start_time)
{
    TaskDoingWidget *doingwidget = new TaskDoingWidget;
    doingwidget->setTaskId(task_id);
    doingwidget->setTaskName(task_name);
    doingwidget->setTaskDescription(task_description);
    doingwidget->setTaskStartTime(task_end_time);
    doingwidget->setTaskEndTime(task_start_time);
    doingwidget->loadWidget();
    list_tasks_doing.push_back(doingwidget);

    // let`s sort them by datetime
    // all tasks that must be finished earlier, will be on top

    std::sort(list_tasks_doing.begin(), list_tasks_doing.end(),
       [] (TaskDoingWidget *object1,  TaskDoingWidget *object2) -> bool {
        return object1->getTaskEndDateTime() < object2->getTaskEndDateTime();
    });

    // now we need to find its index
    int index = list_tasks_doing.indexOf(doingwidget);

    vbox_header_tasks_doing->insertWidget(index, doingwidget->getWidgetTask());

    operateDataTaskDoingWidgets(doingwidget);

    return doingwidget;
}

TaskDoneWidget* BoardWidget::createTaskDoneWidget(QString task_id, QString task_name,
                                            QString task_description, bool overbyuser)
{
    TaskDoneWidget *donewidget = new TaskDoneWidget;
    donewidget->setTaskId(task_id);
    donewidget->setTaskName(task_name);
    donewidget->setTaskDescription(task_description);
    donewidget->setTaskOverByUser(overbyuser);
    donewidget->loadWidget();
    list_tasks_done.push_back(donewidget);

    operateDataTaskDoneWidgets(donewidget);

    return donewidget;
}


void BoardWidget::operateDataTaskToDoWidgets(TaskToDoWidget* task_object)
{
    // signal Doing
        connect(task_object, &TaskToDoWidget::selectedColumnDoing, this, [this, task_object] () {

               SelectTaskTime* task_time = new SelectTaskTime(task_object->getTaskId(),
                              task_object->getTaskName(), QDateTime::currentDateTime());

                // we need to do that if we have left just a one widget
                // if it`s last he can`t hide, because we need to repaint our widget
                // and we can`t do this when we have a signal of this object
                // so, we just hide it, and then it will be repainted
                task_object->getWidgetTask()->setHidden(true);

                // we need to show it, because if we select @Decline@ when edit on Doing
                // later task_time will be hidden, so, we need to show it here
                task_time->getWidgetTaskTime()->setHidden(false);

                vbox_header_tasks_doing->insertWidget(0,task_time->getWidgetTaskTime());

                // let`s remove him from a list

                list_tasks_todo.removeOne(task_object);
                vbox_header_tasks_todo->removeWidget(task_object);

                updateCountColumnToDo();

                if (list_tasks_todo.isEmpty())
                    widget_todo_tip_move->setHidden(false);

                if (list_tasks_doing.isEmpty())
                    widget_doing_tip_move->setHidden(true);

      // signal inside
            connect(task_time, &SelectTaskTime::selectedButtonDecline, this, [this, task_time, task_object] () {

               task_object->getWidgetTask()->setHidden(false);
               task_time->getWidgetTaskTime()->setHidden(true);

               task_object->getComboBox()->setCurrentIndex(0); // Mark as

               vbox_header_tasks_doing->removeWidget(task_time->getWidgetTaskTime());

               // or let`s add our object again to the list
                list_tasks_todo.push_back(task_object);

                updateCountColumnToDo();

                if (!list_tasks_todo.isEmpty())
                {
                    widget_todo_tip_move->setHidden(true);
                    widget_todo_tip_add->setHidden(true);
                }

                if (list_tasks_doing.isEmpty())
                    widget_doing_tip_move->setHidden(false);

                vbox_header_tasks_todo->addWidget(task_object->getWidgetTask());

                task_time->deleteLater();
         });

     // signal inside
          connect(task_time, &SelectTaskTime::selectedButtonAccept, this, [this, task_time, task_object] () {

              QString task_id = task_object->getTaskId();
              QString task_name = task_object->getTaskName();
              QString task_description = task_object->getTaskDescription();
              QString task_end_time = task_time->getTaskEndDateTime();
              QString task_start_time = task_time->getTaskStartTime();

              task_object->getWidgetTask()->setHidden(true);
              vbox_header_tasks_todo->removeWidget(task_object->getWidgetTask());

              task_time->getWidgetTaskTime()->setHidden(true);
              vbox_header_tasks_doing->removeWidget(task_time->getWidgetTaskTime());

              database.updateTaskColumnDoing(user_id, board_id, task_object->getTaskId(),
                   task_time->getTaskStartTime(), task_time->getTaskEndDateTime(), 2);

              task_object->deleteLater();

               TaskDoingWidget* taskdoing =
                       createTaskDoingWidget(task_id, task_name, task_description, task_start_time, task_end_time);

              if (!list_tasks_doing.isEmpty())
                   widget_doing_tip_move->setHidden(true);

              updateCountColumnDoing();

              task_time->deleteLater();
       });
});

 // signal Done
        connect(task_object, &TaskToDoWidget::selectedColumnDone, this, [this, task_object] {

            task_object->getWidgetTask()->setHidden(true);
            vbox_header_tasks_todo->removeWidget(task_object->getWidgetTask());
            list_tasks_todo.removeOne(task_object);

            TaskDoneWidget* task = createTaskDoneWidget(task_object->getTaskId(),
                                                        task_object->getTaskName(),
                                                        task_object->getTaskDescription(),
                                                        true); // overed by user
             tasks_done_byuser++;

             vbox_header_tasks_done->addWidget(task->getWidgetTask());

             updateCountColumnToDo();
             updateCountColumnDone();

            if (list_tasks_todo.isEmpty())
                widget_todo_tip_move->setHidden(false);

            if (!list_tasks_done.isEmpty())
                widget_done_tip_move->setHidden(true);

             showNotification(task->getTaskName());

             database.updateTaskColumnDone(user_id, board_id, task->getTaskId(), 3, true);

        });

  // Signal Edit Task
        connect(task_object, &TaskToDoWidget::openTaskEdit, this, [this, task_object] {
            ViewTask *view_task_todo = new ViewTask;
            view_task_todo->setDataBase(database);
            view_task_todo->setUserId(user_id);
            view_task_todo->setTaskId(task_object->getTaskId());
            view_task_todo->setTaskName(task_object->getTaskName());
            view_task_todo->setTaskDescription(task_object->getTaskDescription());
            view_task_todo->setTaskStatus(TaskStatus::ToDo);
            view_task_todo->loadWidget();

            view_task_todo->move(GlobalPositioning::parentPos(window_main).x()-
                              view_task_todo->divideDialogWidth() + 30, window_main->y() + 55);

            connect(view_task_todo, &ViewTask::taskRemoved, this, [this, task_object] {
                 task_object->getWidgetTask()->setHidden(true);
                 vbox_header_tasks_todo->removeWidget(task_object->getWidgetTask());
                 list_tasks_todo.removeOne(task_object);
                 updateCountColumnToDo();

                 if (list_tasks_doing.isEmpty())
                     widget_doing_tip_move->setHidden(false);

                 if (list_tasks_todo.isEmpty())
                     widget_todo_tip_move->setHidden(false);

                 if (list_tasks_done.isEmpty())
                     widget_done_tip_move->setHidden(false);

                 if (list_tasks_todo.isEmpty() && list_tasks_doing.isEmpty()
                         && list_tasks_done.isEmpty())
                 {
                     widget_todo_tip_add->setHidden(false);
                     widget_todo_tip_move->setHidden(true);
                 }
            });

            connect(view_task_todo, &ViewTask::taskUpdated, this, [this, task_object, view_task_todo] {
                  task_object->setTaskName(view_task_todo->getTaskName());
                  task_object->changeTaskName();
            });

            connect(view_task_todo, &ViewTask::descriptionOn, this, [this, task_object, view_task_todo] {
                 task_object->setTaskDescription(view_task_todo->getTaskDescription());
                 task_object->descriptionEnabled();
            });

            connect(view_task_todo, &ViewTask::descriptionOff, this, [this, task_object] {
                 task_object->descriptionDisabled();
            });

            view_task_todo->exec();
        });
}

void BoardWidget::operateDataTaskDoingWidgets(TaskDoingWidget* task_object)
{

    // signal ToDo
    connect(task_object, &TaskDoingWidget::selectedColumnToDo, this, [this, task_object]
  {
        task_object->getWidgetTask()->setHidden(true);
        vbox_header_tasks_doing->removeWidget(task_object->getWidgetTask());
        list_tasks_doing.removeOne(task_object);
        task_object->stopTaskTime();

        TaskToDoWidget *task = createTaskToDoWidget(task_object->getTaskId(), task_object->getTaskName(),
                                                    task_object->getTaskDescription());

        updateCountColumnDoing();
        updateCountColumnToDo();

        if (list_tasks_doing.isEmpty())
            widget_doing_tip_move->setHidden(false);

        if (!list_tasks_todo.isEmpty())
            widget_todo_tip_move->setHidden(true);

        vbox_header_tasks_todo->addWidget(task->getWidgetTask());

        database.updateTaskColumnDoing(user_id, board_id, task_object->getTaskId(),
                                       NULL, NULL, 1);
    });

    // signal Done
    connect(task_object, &TaskDoingWidget::selectedColumnDone, this, [this, task_object] {

        task_object->getWidgetTask()->setHidden(true);
        vbox_header_tasks_doing->removeWidget(task_object->getWidgetTask());
        list_tasks_doing.removeOne(task_object);
        task_object->stopTaskTime();

        TaskDoneWidget* task = createTaskDoneWidget(task_object->getTaskId(),
                                                    task_object->getTaskName(),
                                                    task_object->getTaskDescription(),
                                                    true); // overed by user
        task->setTaskStartTime(task_object->getTaskStartDateTime());
        task->setTaskEndTime(task_object->getTaskEndDateTime());
        tasks_done_byuser++;

        updateCountColumnDoing();
        updateCountColumnDone();

        if (list_tasks_doing.isEmpty())
            widget_doing_tip_move->setHidden(false);

        if (!list_tasks_done.isEmpty())
            widget_done_tip_move->setHidden(true);

        vbox_header_tasks_done->addWidget(task->getWidgetTask());

        showNotification(task->getTaskName());

        database.updateTaskColumnDone(user_id, board_id, task->getTaskId(), 3, true);

    });

  // signal TaskOvered -> Notification
    connect(task_object, &TaskDoingWidget::taskOvered, this, [this, task_object]
    {
            task_object->getWidgetTask()->setHidden(true);
            vbox_header_tasks_doing->removeWidget(task_object->getWidgetTask());
            list_tasks_doing.removeOne(task_object);

            TaskDoneWidget* task = createTaskDoneWidget(task_object->getTaskId(),
                                                        task_object->getTaskName(),
                                                        task_object->getTaskDescription(),
                                                        false); // overed by timer
            task->setTaskStartTime(task_object->getTaskStartDateTime());
            task->setTaskEndTime(task_object->getTaskEndDateTime());
            tasks_done_bytimer++;

            vbox_header_tasks_done->addWidget(task->getWidgetTask());

            if(list_tasks_doing.isEmpty())
                widget_doing_tip_move->setHidden(false);

            if(!list_tasks_done.isEmpty())
                widget_done_tip_move->setHidden(true);

            updateCountColumnDoing();
            updateCountColumnDone();

            showNotification(task->getTaskName());

            database.updateTaskColumnDone(user_id, board_id, task->getTaskId(), 3, false);

    });

    // Signal Edit Task
          connect(task_object, &TaskDoingWidget::openTaskEdit, this, [this, task_object] {

              ViewTask *view_task_doing = new ViewTask;
              view_task_doing->setDataBase(database);
              view_task_doing->setUserId(user_id);
              view_task_doing->setTaskId(task_object->getTaskId());
              view_task_doing->setTaskName(task_object->getTaskName());
              view_task_doing->setTaskDescription(task_object->getTaskDescription());
              view_task_doing->setTaskStartTime
                      (task_object->getTaskStartDateTime().toString("dd/MM/yyyy hh:mm"));
              view_task_doing->setTaskEndTime
                      (task_object->getTaskEndDateTime().toString("dd/MM/yyyy hh:mm"));
              view_task_doing->setTaskStatus(TaskStatus::Doing);
              view_task_doing->loadWidget();

              view_task_doing->move(GlobalPositioning::parentPos(window_main).x() -
                     view_task_doing->divideDialogWidth() + 30, window_main->y() + 55);

              connect(view_task_doing, &ViewTask::taskRemoved, this, [this, task_object] {
                   task_object->stopTaskTime();
                   task_object->getWidgetTask()->setHidden(true);
                   vbox_header_tasks_doing->removeWidget(task_object->getWidgetTask());
                   list_tasks_doing.removeOne(task_object);
                   updateCountColumnDoing();

                   if (list_tasks_doing.isEmpty())
                       widget_doing_tip_move->setHidden(false);

                   if (list_tasks_todo.isEmpty())
                       widget_todo_tip_move->setHidden(false);

                   if (list_tasks_done.isEmpty())
                       widget_done_tip_move->setHidden(false);

                   if (list_tasks_todo.isEmpty() && list_tasks_doing.isEmpty()
                           && list_tasks_done.isEmpty())
                   {
                       widget_todo_tip_add->setHidden(false);
                       widget_todo_tip_move->setHidden(true);
                   }
             });

              connect(task_object, SIGNAL(taskOvered()), view_task_doing, SLOT(reject()));

              connect(view_task_doing, &ViewTask::taskUpdated, this, [this, task_object, view_task_doing] {
                    task_object->setTaskName(view_task_doing->getTaskName());
                    task_object->changeTaskName();
              });

              connect(view_task_doing, &ViewTask::descriptionOn, this, [this, task_object, view_task_doing] {
                   task_object->setTaskDescription(view_task_doing->getTaskDescription());
                   task_object->descriptionEnabled();
              });

              connect(view_task_doing, &ViewTask::descriptionOff, this, [this, task_object] {
                   task_object->descriptionDisabled();
              });

              view_task_doing->exec();
          });
}


void BoardWidget::operateDataTaskDoneWidgets(TaskDoneWidget *task_object)
{
    // signal ToDo
    connect(task_object, &TaskDoneWidget::selectedColumnToDo, this, [this, task_object] {

           task_object->getWidgetTask()->setHidden(true);
           vbox_header_tasks_done->removeWidget(task_object->getWidgetTask());
           list_tasks_done.removeOne(task_object);

           TaskToDoWidget *task = createTaskToDoWidget(task_object->getTaskId(), task_object->getTaskName(),
                                                       task_object->getTaskDescription());

           if (task_object->isTaskOverByUser() == true)
               tasks_done_byuser--;
           else
               tasks_done_bytimer--;

           updateCountColumnDone();
           updateCountColumnToDo();

           if (list_tasks_done.isEmpty())
               widget_done_tip_move->setHidden(false);

           if (!list_tasks_todo.isEmpty())
               widget_todo_tip_move->setHidden(true);

           vbox_header_tasks_todo->addWidget(task->getWidgetTask());

           database.updateTaskColumnDoing(user_id, board_id, task_object->getTaskId(),
                   NULL, NULL, 1);

    });

   //signal Doing
    connect(task_object, &TaskDoneWidget::selectedColumnDoing, this, [this, task_object]{

            SelectTaskTime* task_time = new SelectTaskTime(task_object->getTaskId(),
                          task_object->getTaskName(), QDateTime::currentDateTime());

            // we need to do that if we have left just a one widget
            // if it`s last he can`t hide, because we need to repaint our widget
            // and we can`t do this when we have a signal of this object
            // so, we just hide it, and then it will be repainted
            task_object->getWidgetTask()->setHidden(true);

            // we need to show it, because if we select @Decline@ when edit on Doing
            // later task_time will be hidden, so, we need to show it here
            task_time->getWidgetTaskTime()->setHidden(false);

            vbox_header_tasks_doing->insertWidget(0,task_time->getWidgetTaskTime());

            // let`s remove him from a list

            list_tasks_done.removeOne(task_object);
            vbox_header_tasks_done->removeWidget(task_object);

            if (task_object->isTaskOverByUser() == true)
                tasks_done_byuser--;
            else
                tasks_done_bytimer--;

            updateCountColumnDone();

            if (list_tasks_done.isEmpty())
                widget_done_tip_move->setHidden(false);

            if (list_tasks_doing.isEmpty())
                widget_doing_tip_move->setHidden(true);

  // signal inside
        connect(task_time, &SelectTaskTime::selectedButtonDecline, this, [this, task_time, task_object] () {

           task_object->getWidgetTask()->setHidden(false);
           task_time->getWidgetTaskTime()->setHidden(true);
           vbox_header_tasks_doing->removeWidget(task_time->getWidgetTaskTime());
           task_object->getComboBox()->setCurrentIndex(0);

            list_tasks_done.push_back(task_object);

            if (task_object->isTaskOverByUser() == true)
                tasks_done_byuser++;
            else
                tasks_done_bytimer++;

           updateCountColumnDone();

           if (list_tasks_doing.isEmpty())
               widget_doing_tip_move->setHidden(false);

            if (!list_tasks_done.isEmpty())
                widget_done_tip_move->setHidden(true);

           vbox_header_tasks_done->addWidget(task_object->getWidgetTask());

           task_time->deleteLater();
     });

 // signal inside
      connect(task_time, &SelectTaskTime::selectedButtonAccept, this, [this, task_time, task_object] () {

          QString task_id = task_object->getTaskId();
          QString task_name = task_object->getTaskName();
          QString task_description = task_object->getTaskDescription();
          QString task_end_time = task_time->getTaskEndDateTime();
          QString task_start_time = task_time->getTaskStartTime();

          task_object->getWidgetTask()->setHidden(true);
          vbox_header_tasks_done->removeWidget(task_object->getWidgetTask());

          task_time->getWidgetTaskTime()->setHidden(true);
          vbox_header_tasks_doing->removeWidget(task_time->getWidgetTaskTime());

          database.updateTaskColumnDoing(user_id, board_id, task_object->getTaskId(),
               task_time->getTaskStartTime(), task_time->getTaskEndDateTime(), 2);

           TaskDoingWidget* taskdoing =
                   createTaskDoingWidget(task_id, task_name, task_description, task_start_time, task_end_time);

          updateCountColumnDoing();

          task_time->deleteLater();
          task_object->deleteLater();
        });
  });

 // signals when objects have been done by timer from TaskDoingWidget

    // signal (Confrim No -> TaskDoing)
    connect(task_object, &TaskDoneWidget::taskConfirmNo, this, [this, task_object] {
         emit task_object->selectedColumnDoing();
    });

    connect(task_object, &TaskDoneWidget::taskConfirmYes, this, [this, task_object] {
           task_object->markAsDoneByUser();
           database.updateTaskColumnDone(user_id, board_id, task_object->getTaskId(), 3,
                                         true);
// update counting
           tasks_done_bytimer--;
           tasks_done_byuser++;
// and let`s update values
           updateCountColumnDone();
    });

    // Signal Edit Task
          connect(task_object, &TaskDoingWidget::openTaskEdit, this, [this, task_object] {

              ViewTask *view_task_done = new ViewTask;
              view_task_done->setDataBase(database);
              view_task_done->setUserId(user_id);
              view_task_done->setTaskId(task_object->getTaskId());
              view_task_done->setTaskName(task_object->getTaskName());
              view_task_done->setTaskDescription(task_object->getTaskDescription());
              view_task_done->setTaskStartTime
                      (task_object->getTaskStartTime().toString("dd/MM/yyyy hh:mm"));
              view_task_done->setTaskEndTime
                      (task_object->getTaskEndTime().toString("dd/MM/yyyy hh:mm"));
              view_task_done->setTaskStatus(TaskStatus::Done);
              view_task_done->loadWidget();

              view_task_done->move(GlobalPositioning::parentPos(window_main).x() -
                     view_task_done->divideDialogWidth() + 30, window_main->y() + 55);

              connect(view_task_done, &ViewTask::taskRemoved, this, [this, task_object] {
                   task_object->getWidgetTask()->setHidden(true);
                   vbox_header_tasks_done->removeWidget(task_object->getWidgetTask());
                   list_tasks_done.removeOne(task_object);
                   updateCountColumnDone();

                   if (list_tasks_doing.isEmpty())
                       widget_doing_tip_move->setHidden(false);

                   if (list_tasks_todo.isEmpty())
                       widget_todo_tip_move->setHidden(false);

                   if (list_tasks_done.isEmpty())
                       widget_done_tip_move->setHidden(false);

                   if (list_tasks_todo.isEmpty() && list_tasks_doing.isEmpty()
                           && list_tasks_done.isEmpty())
                   {
                       widget_todo_tip_add->setHidden(false);
                       widget_todo_tip_move->setHidden(true);
                   }
              });


              connect(view_task_done, &ViewTask::taskUpdated, this, [this, task_object, view_task_done] {
                    task_object->setTaskName(view_task_done->getTaskName());
                    task_object->changeTaskName();
              });

              connect(view_task_done, &ViewTask::descriptionOn, this, [this, task_object, view_task_done] {
                   task_object->setTaskDescription(view_task_done->getTaskDescription());
                   task_object->descriptionEnabled();
              });

              connect(view_task_done, &ViewTask::descriptionOff, this, [this, task_object] {
                   task_object->descriptionDisabled();
              });

              view_task_done->exec();
          });
}

void BoardWidget::retrieveTaskToDoWidgets()
{
    if (database.isConnected())
    {
        QVector<QStringList> user_tasks = database.getUserDataTTaskToDo(user_id, board_id, 1);

        if (!user_tasks.isEmpty() && list_tasks_todo.isEmpty())
        {
            for (unsigned long long i=0; i<user_tasks.size(); i++)
            {
                     // 0 - task_id   1 - task_name 2 - task_description
               TaskToDoWidget *task = createTaskToDoWidget(user_tasks.at(i).at(0),
                                                           user_tasks.at(i).at(1),
                                                           user_tasks.at(i).at(2));

                vbox_header_tasks_todo->addWidget(task->getWidgetTask());
            }
        }
    }
    widget_todo_tip_add = infoColumnAddTasks();
    widget_todo_tip_move = infoColumnMoveTask(vbox_header_tasks_todo);
    widget_todo_tip_add->setHidden(true);
    widget_todo_tip_move->setHidden(true);
    updateCountColumnToDo();
}

void BoardWidget::retrieveTaskDoingWidgets()
{
    if (database.isConnected())
    {
        QVector<QStringList> user_tasks = database.getUserDataTTaskDoing(user_id, board_id);

        if (!user_tasks.isEmpty() && list_tasks_doing.isEmpty())
        {
            for (unsigned long long i=0; i<user_tasks.size(); i++)
            {
                // 0 - task_id   1 - task_name 2 - task_description 3 - task_start_time
                // 4 - task_end_time
                TaskDoingWidget *task = createTaskDoingWidget(user_tasks.at(i).at(0),
                    user_tasks.at(i).at(1), user_tasks.at(i).at(2),
                    user_tasks.at(i).at(3), user_tasks.at(i).at(4));
            }
        }
    }
     widget_doing_tip_move = infoColumnMoveTask(vbox_header_tasks_doing);
     widget_doing_tip_move->setHidden(true);
     updateCountColumnDoing();
}

void BoardWidget::retrieveTaskDoneWidgets()
{
    if (database.isConnected())
    {
        QVector<QVariantList> user_tasks = database.getUserDataTTaskDone(user_id, board_id, 3);

        if (!user_tasks.isEmpty() && list_tasks_done.isEmpty())
        {
            for (unsigned long long i=0; i<user_tasks.size(); i++)
            {
                // 0 - task_id   1 - task_name 2 - task_description // 3 - task_over_byuser
                TaskDoneWidget *task = createTaskDoneWidget(user_tasks.at(i).at(0).toString(),
                   user_tasks.at(i).at(1).toString(), user_tasks.at(i).at(2).toString(),
                   user_tasks.at(i).at(3).toBool());

                if (user_tasks.at(i).at(3).toBool() == true)
                    tasks_done_byuser++;
                else
                    tasks_done_bytimer++;

                vbox_header_tasks_done->addWidget(task->getWidgetTask());
            }
        }
    }
    widget_done_tip_move = infoColumnMoveTask(vbox_header_tasks_done);
    widget_done_tip_move->setHidden(true);
    updateCountColumnDone();
}

void BoardWidget::showBoardSettings()
{
    boardsettings = new BoardSettings(this);
    boardsettings->setDataBase(database);
    boardsettings->setUserId(user_id);
    boardsettings->setBoardId(board_id);
    boardsettings->setBoardName(board_name);
    boardsettings->setBoardNotificationSound(board_notification_sound);

    boardsettings->loadWidget();

    connect(boardsettings, SIGNAL(boardUpdated()), this, SLOT(updateBoard()));
    connect(boardsettings, SIGNAL(allTasksDeleted()), this, SLOT(updateBoardTasks()));
    connect(boardsettings, SIGNAL(boardDeleted()), this, SLOT(loadRecentWidget()));

    boardsettings->exec();
}

void BoardWidget::updateBoard()
{
    if (database.isConnected())
    {
        QPair<QString, bool> user_board =
                database.getUserBoardNameNotification(user_id, board_id);
        board_name = user_board.first;
        label_board_name->setText(board_name);
        board_notification_sound = user_board.second;
    }
}

void BoardWidget::updateBoardTasks()
{
    // and also i need to clear data inside variables
    tasks_done_bytimer = tasks_done_byuser = 0;

    for (unsigned short i=0; i<list_tasks_todo.size(); i++)
    {
        vbox_header_tasks_todo->removeWidget(list_tasks_todo.at(i)->getWidgetTask());
        list_tasks_todo.at(i)->getWidgetTask()->setHidden(true);
    }

    for (unsigned short i=0; i<list_tasks_doing.size(); i++)
    {
        vbox_header_tasks_doing->removeWidget(list_tasks_doing.at(i)->getWidgetTask());
        list_tasks_doing.at(i)->getWidgetTask()->setHidden(true);
        // and also we need to stop all of our tasks
        list_tasks_doing.at(i)->stopTaskTime();
    }

    for (unsigned short i=0; i<list_tasks_done.size(); i++)
    {
        vbox_header_tasks_done->removeWidget(list_tasks_done.at(i)->getWidgetTask());
        list_tasks_done.at(i)->getWidgetTask()->setHidden(true);
    }

    list_tasks_todo.clear();
    list_tasks_doing.clear();
    list_tasks_done.clear();

    // and let`s hide widgets tips

    widget_todo_tip_add->setHidden(true);

    widget_todo_tip_move->setHidden(true);
    widget_doing_tip_move->setHidden(true);
    widget_done_tip_move->setHidden(true);

    if (!vbox_header_tasks_doing->isEmpty())
        vbox_header_tasks_doing->itemAt(0)->widget()->setHidden(true);

    showInfoTasksTips();

// and show info and update tasks count
    updateCountColumnToDo();
    updateCountColumnDoing();
    updateCountColumnDone();
}
