#include "boardsettings.h"
#include "styleqss.h"
#include "usercorrectnessinput.h"

#include <QPair>

// delete later
#include <QDebug>

BoardSettings::BoardSettings(QWidget *parent) : AnimatedDialog(parent),
    board_rmcount_click(0), tasks_rm_count_click(0)
{
    setAttribute(Qt::WA_TranslucentBackground, true);

    frame_boardsettings = new QFrame(this);
    frame_boardsettings->setObjectName("frameBoardSettings");

    vbox_main = new QVBoxLayout(this);
    vbox_main->addWidget(frame_boardsettings);

    vbox_frame_boardsettings = new QVBoxLayout;
    frame_boardsettings->setLayout(vbox_frame_boardsettings);

    setLayout(vbox_main);

    setFixedSize(500,600);
}

void BoardSettings::setDataBase(DataBase &database)
{
    this->database = database;
}

void BoardSettings::setUserId(QString user_id)
{
    this->user_id = user_id;
}

void BoardSettings::setBoardId(QString board_id)
{
    this->board_id = board_id;
}

void BoardSettings::setBoardName(QString board_name)
{
    this->board_name = board_name;
}

void BoardSettings::setBoardNotificationSound(bool board_notification_sound)
{
    this->board_notification_sound = board_notification_sound;
}

void BoardSettings::loadWidget()
{
    label_boardsettings_name = new QLabel(this);
    label_boardsettings_name->setObjectName("labelBoardSettingsName");
    label_boardsettings_name->setText(tr("Board editing"));

    label_background_image = new QLabel(this);
    label_background_image->setObjectName("labelBackgroundImage");
    label_background_image->setPixmap(QPixmap(":/image/Image/background_allboards.png"));
    // the maximum size for perfect background
    label_background_image->setMaximumSize(500,150);

 // section alert

      label_alert = new QLabel(this);
      label_alert->setObjectName("labelAlert");
      label_alert->setHidden(true);

// section name

    hbox_section_name = new QHBoxLayout(this);

    label_title_board_name = new QLabel(this);
    label_title_board_name->setObjectName("labelTitleBoardName");
    label_title_board_name->setText(tr("Board name: "));

    editline_board_name = new QLineEdit(this);
    editline_board_name->setObjectName("editlineBoardName");
    editline_board_name->setText(board_name);
    editline_board_name->setAttribute(Qt::WA_MacShowFocusRect, false);

    hbox_section_name->setMargin(10);
    hbox_section_name->addWidget(label_title_board_name);
    hbox_section_name->addWidget(editline_board_name);

// section notification

    hbox_section_sound = new QHBoxLayout(this);

    label_title_board_notification = new QLabel(this);
    label_title_board_notification->setObjectName("labelTitleNotificationSound");
    label_title_board_notification->setText(tr("Sound notification: "));

// switch button

    switch_button_notification = new SwitchButton(this);
    switch_button_notification->setSwitchStatus(board_notification_sound);

    if (appsettings.getUserLanguage() == 2) // Ukrainian
            switch_button_notification->setOffset(65);
    else if (appsettings.getUserLanguage() == 1) // Russian
            switch_button_notification->setOffset(25);
    else // English
           switch_button_notification->setOffset(64);

    hbox_section_sound->setMargin(10);
    hbox_section_sound->addWidget(label_title_board_notification);
    hbox_section_sound->addWidget(switch_button_notification);
    hbox_section_sound->addSpacing(250);

// remove tasks

    label_remove_tasks = new ExtendedLabel(this);
    label_remove_tasks->setObjectName("labelRemoveTasks");
    label_remove_tasks->setText(tr("Remove all tasks"));
    label_remove_tasks->setCursor(Qt::PointingHandCursor);

// section buttons

    hbox_section_buttons = new QHBoxLayout(this);

    label_image_remove_board = new ExtendedLabel(this);
    label_image_remove_board->setObjectName("labelImageRemoveBoard");
    label_image_remove_board->setFixedSize(40,40);
    label_image_remove_board->setCursor(Qt::PointingHandCursor);

    label_title_remove_board = new ExtendedLabel(this);
    label_title_remove_board->setObjectName("labelTitleRemoveBoard");
    label_title_remove_board->setText(tr("Remove board"));
    label_title_remove_board->setCursor(Qt::PointingHandCursor);

    button_close = new QPushButton(this);
    button_close->setObjectName("buttonClose");
    button_close->setText(tr("Close"));

    button_save = new QPushButton(this);
    button_save->setObjectName("buttonSave");
    button_save->setText(tr("Save"));
    button_save->setCursor(Qt::PointingHandCursor);
    button_save->setEnabled(false);

    hbox_section_buttons->setMargin(10);

    hbox_section_buttons->addWidget(label_image_remove_board);
    hbox_section_buttons->addWidget(label_title_remove_board);
    hbox_section_buttons->addStretch(1);
    hbox_section_buttons->addWidget(button_close);
    hbox_section_buttons->addWidget(button_save);

 //

    vbox_frame_boardsettings->setMargin(0);
    vbox_frame_boardsettings->addWidget(label_boardsettings_name);
    vbox_frame_boardsettings->addWidget(label_background_image);
    vbox_frame_boardsettings->addWidget(label_alert);
    vbox_frame_boardsettings->addStretch(1);
    vbox_frame_boardsettings->addLayout(hbox_section_name);
    vbox_frame_boardsettings->addLayout(hbox_section_sound);
    vbox_frame_boardsettings->addSpacing(50);
    vbox_frame_boardsettings->addWidget(label_remove_tasks);
    vbox_frame_boardsettings->addStretch(1);
    vbox_frame_boardsettings->addLayout(hbox_section_buttons);

    vbox_frame_boardsettings->setAlignment(label_alert, Qt::AlignCenter);
    vbox_frame_boardsettings->setAlignment(label_boardsettings_name, Qt::AlignLeft);
    vbox_frame_boardsettings->setAlignment(label_remove_tasks, Qt::AlignCenter);

    tasks_available = database.getUserBoardAvailableTasks(user_id, board_id);

    connect (button_close, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(label_image_remove_board, SIGNAL(clicked()), this, SLOT(removeBoard()));
    connect(label_title_remove_board, SIGNAL(clicked()), this, SLOT(removeBoard()));
    connect(label_remove_tasks, SIGNAL(clicked()), this, SLOT(removeTasks()));
    connect(switch_button_notification, SIGNAL(clicked(bool)), this, SLOT(enableButtonSave()));
    connect(editline_board_name, SIGNAL(textEdited(QString)), this, SLOT(enableButtonSave()));
    connect(button_save, SIGNAL(clicked(bool)), this, SLOT(saveBoardSettings()));

    if (appsettings.getThemeColor() == 1) // Dark
        setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_boardsettings_dark.qss"));
    else
        setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_boardsettings_light.qss"));
}

void BoardSettings::enableButtonSave()
{
    UserCorrectnessInput *correctline = new UserCorrectnessInput(this);
    correctline->inputCheckCorrectLine(editline_board_name->text(),button_save);
}

void BoardSettings::saveBoardSettings()
{
    if (database.isConnected())
    {
        label_alert->setText(tr("Great! All data have been updated."));
        label_alert->setStyleSheet("background: #3AA341; color:white;");
        label_alert->setHidden(false);
        database.updateBoardName(user_id, board_id, editline_board_name->text());
        database.updateBoardNotificationSound(user_id, board_id, switch_button_notification->isSwitchOn());
        emit boardUpdated();
    }
    // now we need to update our values
    // name and notification sound
}

void BoardSettings::removeBoard()
{
    board_rmcount_click++;

    if (board_rmcount_click > 1)
    {
        if (database.isConnected())
        {
            database.removeUserBoard(board_id);
            label_alert->setHidden(true);
            emit boardDeleted();
            close();
        }

    }
    else if (board_rmcount_click > 0)
    {
       label_alert->setText(tr("Are you sure, that you would like to remove board?\n"
                               "All actions can`t be undone! To remove board click it again!"));
       label_alert->setStyleSheet("background: #F7BAA8;");
       label_alert->setHidden(false);
    }
}

void BoardSettings::removeTasks()
{
    tasks_rm_count_click++;

    if (tasks_available != true)
    {
        label_alert->setText(tr("Nothing to remove! First create a task."));
        label_alert->setStyleSheet("background: #CEE4EA; color:black;");
    }
    else if (tasks_rm_count_click > 1)
    {
        if (database.isConnected())
        {
                // let`s take data and if it`s not empty then we wull remove them
                database.removeBoardAllTasks(user_id, board_id);
                label_alert->setText(tr("Great! All tasks have been removed."));
                label_alert->setStyleSheet("background: #3AA341; color:white;");
                emit allTasksDeleted();
        }
    }
    else if (tasks_rm_count_click > 0)
    {
       label_alert->setText(tr("Are you sure, that you would like to remove all tasks?\n"
                               "All actions can`t be undone! To remove tasks click it again!"));
       label_alert->setStyleSheet("background: #F7BAA8;");
    }
    label_alert->setHidden(false);
}
