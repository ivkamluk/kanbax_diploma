#include "confirmdeleteboard.h"
#include "databaseerror.h"
#include "styleqss.h"
#include "stylefont.h"

/*
    this class show dialog box for confirmation
     when user want to delete a board
*/

//delete later
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

ConfirmDeleteBoard::ConfirmDeleteBoard(QWidget *parent) : AnimatedDialog(parent),
    board_deleted(false)
{
    setAttribute(Qt::WA_TranslucentBackground);

    vbox_main = new QVBoxLayout(this);

    frame = new QFrame(this);
    vbox_frame = new QVBoxLayout;

    // we need this QFrame because only with this we can make
    // qdialog with rounded corners without pixeling
    // so, we create vbox_main then addwidget(frame) and then frame->setLayout(vbox_frame)
    // after that we add in code  setAttribute(Qt::WA_TranslucentBackground)
    // and write in stylesheet border-radius
    // so with this we will have main frame, that can be rounded
    frame->setObjectName("frame");

    vbox_main->addWidget(frame);
    frame->setLayout(vbox_frame);

    label_confirmation_text = new QLabel(this);
    label_confirmation_text->setObjectName("labelConfirmationText");
    label_confirmation_text->setText(tr("Are you sure you want to delete this board?"));

    hbox_buttons = new QHBoxLayout(this);

    button_cancel = new QPushButton (this);
    button_cancel->setObjectName("buttonCancel");
    button_cancel->setText(tr("Cancel"));

    button_delete = new QPushButton(this);
    button_delete->setObjectName("buttonDelete");
    button_delete->setText(tr("Delete"));

    //ATTENTION
        // app is crashing when this is on and
        // when user click SpaceBar
        // button_delete->setDefault(true);

    hbox_buttons->addWidget(button_cancel);
    hbox_buttons->addWidget(button_delete);
    hbox_buttons->setSpacing(20);

    vbox_frame->addWidget(label_confirmation_text);
    vbox_frame->addLayout(hbox_buttons);

    setLayout(vbox_main);

    connect(button_cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(button_delete, SIGNAL(clicked(bool)), this, SLOT(deleteUserBoard()));

    if (appsettings.getThemeColor() == 1) // Dark
           setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_confirmationdeleteboard_dark.qss"));
    else
            setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_confirmationdeleteboard_light.qss"));
}

//Methods @set
void ConfirmDeleteBoard::setDataBase(DataBase &database)
{
    this->database = database;
}

void ConfirmDeleteBoard::setBoardId(QString board_id)
{
    this->board_id = board_id;
}

void ConfirmDeleteBoard::setUserId(QString user_id)
{
    this->user_id = user_id;
}

bool ConfirmDeleteBoard::isBoardDeleted()
{
    return board_deleted;
}

void ConfirmDeleteBoard::deleteUserBoard()
{
    if (database.isConnected())
    {
        qDebug()<<user_id;
        qDebug()<<board_id;
        database.removeBoardAllTasks(user_id, board_id);
        database.removeUserBoard(board_id);

        // this variable give a signal that board was deleted
        board_deleted = true;
    }
    else
    {
        DataBaseError *databaseerror = new DataBaseError(this);
        databaseerror->exec();
    }
    this->accept();
}
