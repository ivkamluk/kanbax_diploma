#include "createboard.h"
#include "stylefont.h"
#include "databaseerror.h"
#include "usercorrectnessinput.h"

#include <QRegExp>
#include <QDateTime>

/*
    this class show dialog box when user
    user click @Create new board@
*/

//delete later
#include <QDebug>

//constructor
CreateBoard::CreateBoard(QWidget *parent) : AnimatedDialog(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    vbox_frame = new QVBoxLayout;
    frame = new QFrame(this);

    vbox_main = new QVBoxLayout;
    hbox_buttons = new QHBoxLayout;

    frame->setObjectName("frame");

    vbox_main->addWidget(frame);
    frame->setLayout(vbox_frame);

    label_title_create = new QLabel(this);
    label_title_create->setObjectName("labelTitleCreate");
    label_title_create->setText(tr("Your new board"));

    editline_create = new QLineEdit(this);
    editline_create->setObjectName("editlineCreate");
   // editline_create->setFont(StyleFont::loadFontSourceCodeVariable());
    //remove blue border for editline on MAC
    editline_create->setAttribute(Qt::WA_MacShowFocusRect,0);

    button_cancel = new QPushButton(this);
    button_cancel->setObjectName("buttonCancel");
    button_cancel->setText(tr("Decline"));

    button_ok = new QPushButton(this);
    button_ok->setObjectName("buttonOk");
    button_ok->setText(tr("Save"));
    // when we @setDefault@ we call @Enter@ key event
    button_ok->setDefault(true);
    button_ok->setDisabled(true);

    hbox_buttons->setSpacing(20);
    hbox_buttons->addWidget(button_cancel);
    hbox_buttons->addWidget(button_ok);

    vbox_frame->setSpacing(25);
    vbox_frame->addWidget(label_title_create);
    vbox_frame->addWidget(editline_create);
    vbox_frame->addLayout(hbox_buttons);
    vbox_frame->setAlignment(hbox_buttons, Qt::AlignRight);

    setLayout(vbox_main);

    resize(500,40);

    connect(button_ok, SIGNAL(clicked(bool)), this, SLOT(saveBoard()));
    connect(button_cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(editline_create, SIGNAL(textEdited(QString)), this, SLOT(enableButtonSave()));

    if (appsettings.getThemeColor() == 1) // Dark
        setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_createboard_dark.qss"));
    else
        setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_createboard_light.qss"));
}

 //Methods @set
void CreateBoard::setDatabase(DataBase &database)
{
    this->database = database;
}

void CreateBoard::setUserId(QString user_id)
{
   this->user_id = user_id;
}

void CreateBoard::setStackedWidget(QStackedWidget *stackedwidget)
{
    stacked_widget = stackedwidget;
}

void CreateBoard::setListWidget(QListWidget *listwidget)
{
    this->listwidget = listwidget;
}

void CreateBoard::saveBoard()
{
    if (database.isConnected())
    {
        // we get current datetime
        // and it will be the same as for creation and modification
        QString datetime_board_creation = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

         // at the default we have sound notification for board creation
        database.writeUserDataTBoard(user_id,editline_create->text(), datetime_board_creation,
                                        datetime_board_creation, true);
        boardwidget = new BoardWidget(this);
        boardwidget->setDataBase(database);
        boardwidget->setBoardId(database.getUserCreatedBoardId(user_id)); // last created board
        boardwidget->setUserId(user_id);
        boardwidget->loadWidget();
        stacked_widget->addWidget(boardwidget);
        stacked_widget->setCurrentWidget(boardwidget);
        // now indicate that we open boardwidget
        listwidget->item(1)->setSelected(true);
    }
    else
    {
        DataBaseError *databaseerror = new DataBaseError(this);
        databaseerror->exec();
    }
    this->accept();
}

void CreateBoard::enableButtonSave()
{
    UserCorrectnessInput *correctinput = new UserCorrectnessInput(this);
    correctinput->inputCheckCorrectLine(editline_create->text(), button_ok);
}
