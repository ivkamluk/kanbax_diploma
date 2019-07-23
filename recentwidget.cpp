#include "recentwidget.h"
#include "styleqss.h"
#include "stylefont.h"
#include "globalpositioning.h"
#include "confirmdeleteboard.h"

#include <QDateTime>

/*
    this class display all recent boards
    and hot tasks that user have
*/

//delete
#include <QDebug>
#include <QTimer>
#include <QGraphicsBlurEffect>


//constructor
RecentWidget::RecentWidget(QWidget *parent) :
    ExtendedWidget(parent), boards_show_number(0), htask_number(0)
{
    databaseerror = new DataBaseError(this);

    vbox_main = new QVBoxLayout;
    setLayout(vbox_main);

    // we don`t initialize here in constructor all widgets
    // because first we need to get from database their number
    // so for initializing them we have functions
    // : initializeBoards() and : initializeHTasks()
}

//function set/get that display number of hot tasks
void RecentWidget::setHtaskNumber(unsigned short htask_number)
{
    this->htask_number= htask_number;
}

unsigned short RecentWidget::getHtaskNumber()
{
    return htask_number;
}

//function that set`s database to this widget
void RecentWidget::setDatabase(DataBase &database)
{
    this->database = database;
}

//function set user_id, with it`s value we will get board
void RecentWidget::setUserId(QString user_id)
{
    this->user_id = user_id;
}

//this function will set up all functionality to this widget
void RecentWidget::loadWidget()
{

    if (appsettings.getRecentBoardsCount() == 0) // boards-count 2
        boards_show_number = 2;
    else if (appsettings.getRecentBoardsCount() == 1) // boards count 3
        boards_show_number = 3;
    else if (appsettings.getRecentBoardsCount() == 2) // boards coutn 4
        boards_show_number = 4;

    if (database.isConnected())
    {
        user_boards = database.getUserDataTBoard(user_id, boards_show_number);

        // now we retrieve data about user boards3

        if (user_boards.isEmpty())
        {
            emptyRecentBoards();
            initializeCreateBoard();
        }
        else
        {
            setHtaskNumber(3);
            // all this variables later we will need to save in QSettings!!!!

            // here we compare how many boards we have
            // if in settings we have saved -> display 3 boards
            // but we don`t have no one
            //  we set number of boards that we have in database (2)
            // so there will be displayed 2 boards -> not 3
            // later if user create new board -> will be displayed 3 boards
            if (user_boards.size() < boards_show_number)
                initializeBoards(user_boards.size());
            else
                initializeBoards(boards_show_number);

//            initializeHTasks(getHtaskNumber());
        }
    }
    else
        databaseerror->exec();

    if (appsettings.getThemeColor() == 1) // Dark
         setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_recentwindow_dark.qss"));
    else
        setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_recentwindow_light.qss"));


}

//functions that initializes from database user boards and htasks
void RecentWidget::initializeBoards(unsigned short boards)
{

 // BUTTON AND LABEL RECENT BOARDS

    hbox_top_boards = new QHBoxLayout;

    label_last_boards = new QLabel(this);
    button_allboards = new QPushButton(this);

    label_last_boards->setObjectName("labelLastBoards");
    button_allboards->setObjectName("buttonAllBoards");

    //Set correct size for icon to see all boards
    button_allboards->setMinimumSize(50,50);

    hbox_top_boards->addWidget(button_allboards);
    hbox_top_boards->addWidget(label_last_boards);
    hbox_top_boards->addStretch(1);

    hbox_top_boards->setAlignment(label_last_boards, Qt::AlignBaseline);

    QString display_board_text;

    if (boards > 1)
        // if user have more than 1 board (2-3-4) boards
          label_last_boards->setText(tr("Your last %1 boards").
                                     arg(QString::number(user_boards.size())));
    else
      // if user have only one board (1 board)
        label_last_boards->setText(tr("Your last %1 board").
                                   arg(QString::number(user_boards.size())));

    vbox_main->addLayout(hbox_top_boards);

 // SHOW RECENT BOARDS
    hbox_boads = new QHBoxLayout;
    vbox_main->addLayout(hbox_boads);
   // space between boards and button create new board
   vbox_main->addSpacing(50);

    // Initialize widgets in vector, because we don`t know
    // how many we will have
    for (unsigned short i=0; i<boards; i++)
       {
        // dynamically allocate memory for widgets inside vector
           vec_widget_boards.append(new ExtendedWidget(this));
           vec_vbox_boards.append(new QVBoxLayout(this));
           vec_label_remove_boards.append(new ExtendedLabel(this));
           vec_label_name_boards.append(new QLabel(this));
           vec_label_created_boards.append((new QLabel(this)));
           vec_label_date_boards.append((new QLabel(this)));

           vec_widget_boards.at(i)->setWidgetId(i);
           vec_label_remove_boards.at(i)->setLabelId(i);

           vec_widget_boards.at(i)->setObjectName("vecWidgetBoards");
           vec_label_remove_boards.at(i)->setObjectName("vecLabelRemoveBoards");
           vec_label_name_boards.at(i)->setObjectName("vecLabelNameBoards");
           vec_label_created_boards.at(i)->setObjectName("vecLabelCreatedBoards");
           vec_label_date_boards.at(i)->setObjectName("vecLabelDateBoards");

           //make for all widgets horizontal alligment
           hbox_boads->addWidget(vec_widget_boards.at(i));

           //give specific size for widget to make it look
           // like rectangle
           vec_widget_boards.at(i)->setMinimumSize(250,250);
           vec_widget_boards.at(i)->setMaximumSize(350,350);

           // for each widget, we have each vertical layout
           vec_widget_boards.at(i)->setLayout(vec_vbox_boards.at(i));

           vec_label_remove_boards.at(i)->setCursor(Qt::PointingHandCursor);
           vec_label_remove_boards.at(i)->setHidden(true);
           vec_label_remove_boards.at(i)->setFixedSize(QSize(15,15));

           // now let`s make connection between widget id and board id
           widget_board_id.insert(i, (user_boards.at(i).at(0)));

           // we check if board name is good for displaying
           // if it`s length is smaller than we have in function
            QString board_name = user_boards.at(i).at(2);
            maxLengthNameBoard(board_name);

           vec_label_name_boards.at(i)->setText(board_name);

           vec_label_created_boards.at(i)->setText(tr("Date of creation: "));

           // now we convert date
           QDateTime date = QDateTime::fromString(user_boards.at(i).at(3), "yyyy-MM-ddTHH:mm:ss");
           vec_label_date_boards.at(i)->setText(date.toString("dd/MM/yyyy hh:mm"));

           vec_vbox_boards.at(i)->addWidget(vec_label_remove_boards.at(i));
           vec_vbox_boards.at(i)->setAlignment(vec_label_remove_boards.at(i), Qt::AlignRight | Qt::AlignLeading);
           vec_vbox_boards.at(i)->addStretch(1);
           vec_vbox_boards.at(i)->addSpacing(-11);
           vec_vbox_boards.at(i)->addWidget(vec_label_name_boards.at(i));
           vec_vbox_boards.at(i)->addWidget(vec_label_created_boards.at(i));
           vec_vbox_boards.at(i)->addWidget(vec_label_date_boards.at(i));
           vec_vbox_boards.at(i)->addStretch(1);


           //set for widgets alligment
           vec_vbox_boards.at(i)->setAlignment(vec_label_name_boards.at(i), Qt::AlignHCenter);
           vec_vbox_boards.at(i)->setAlignment(vec_label_date_boards.at(i), Qt::AlignHCenter);
           vec_vbox_boards.at(i)->setAlignment(vec_label_created_boards.at(i), Qt::AlignHCenter);

           hbox_boads->setAlignment(vec_widget_boards.at(i), Qt::AlignVCenter);

           vec_vbox_boards.at(i)->addStretch(1);
       }

    for (unsigned short i=0; i<boards; i++)
    {
        vec_widget_boards.at(i)->installEventFilter(this);
        vec_widget_boards.at(i)->setMouseTracking(true);
        vec_widget_boards.at(i)->setAttribute(Qt::WA_Hover);
    }

    for (unsigned short i=0; i<boards; i++)
    {
        connect(vec_widget_boards.at(i), SIGNAL(clicked()), this, SLOT(openBoardByWidget()));
        connect(vec_label_remove_boards.at(i), SIGNAL(clicked()), this, SLOT(deleteBoardByLabel()));
    }

    // signals and slots
    connect(button_allboards, SIGNAL(clicked(bool)), this, SLOT(showAllBoards()));

    initializeCreateBoard();
}

// function that initializes creation of board
void RecentWidget::initializeCreateBoard()
{
    // CREATE BOARD
       hbox_create_board = new QHBoxLayout;
       widget_create_board = new ExtendedWidget;
       button_create_board = new QPushButton;
       label_create_board = new ExtendedLabel(this);

       widget_create_board->setObjectName("widgetCreateBoard");
       button_create_board->setObjectName("buttonCreateBoard");
       label_create_board->setObjectName("labelCreateBoard");

       button_create_board->setFixedSize(40,40);
       label_create_board->setText(tr("Create new board"));

       widget_create_board->setCursor(Qt::PointingHandCursor);
       button_create_board->setCursor(Qt::PointingHandCursor);
       label_create_board->setCursor(Qt::PointingHandCursor);

       // make layout inside widgets
       // in layout -> button and label
       widget_create_board->setLayout(hbox_create_board);

       hbox_create_board->addSpacing(3);
       hbox_create_board->addWidget(button_create_board);
       hbox_create_board->setSpacing(11);
       hbox_create_board->addWidget(label_create_board);    
       // add new widget to the main layout of all widget
       vbox_main->addSpacing(50);
       vbox_main->addWidget(widget_create_board);

       // make it look in center
       vbox_main->setAlignment(widget_create_board, Qt::AlignCenter);
       vbox_main->addStretch(1);

    //  connect signals with widgets

           // this signals only for @Create new board@
           // without label or button, we can only click on widget
           // so we need to connect with them also
           connect(label_create_board, SIGNAL(clicked()), this, SLOT(createBoard()));
           connect(button_create_board, SIGNAL(clicked()), this, SLOT(createBoard()));
           connect(widget_create_board, SIGNAL(clicked()), this, SLOT(createBoard()));

}

//functions that initializes from database user boards and htasks
void RecentWidget::initializeHTasks(unsigned short htasks)
{

 // Adding Hot Tasks for now

      label_htask_main = new QLabel(this);
      label_htask_main->setObjectName("labelHtaskMain");

      label_htask_main->setText(tr("Hot tasks for now"));
       //load font for main label hot tasks
      label_htask_main->setFont(StyleFont::loadFontSourceCodeVariable());

    // add label_htas_main to main hbox to all recent widget
    vbox_main->addWidget(label_htask_main);

// add main widget for hot tasks

     grid_htask_table = new QGridLayout;

     widget_htask_table = new QWidget(this);
     widget_htask_table->setObjectName("widgetHtaskTable");

     widget_htask_table->setLayout(grid_htask_table);

     // we need this to make widget htask for all free space
     // because i we don`t do this, after it we will
     // have enough space and widgets wouldn`t be scretched
     vbox_main->setStretchFactor(widget_htask_table,1);

     vbox_main->addWidget(widget_htask_table);

    for(unsigned short i=0; i<htasks;i++)
    {
        //dynamically allocate memory for widgets
        vec_label_htask_images.append(new QLabel(this));
        vec_label_htask_names.append(new QLabel(this));
        vec_htask_progressbars.append(new QProgressBar(this));
        vec_htask_board_names.append(new QLabel(this));

        vec_label_htask_images.at(i)->setObjectName("vecLabelHtaskImages");
        vec_label_htask_names.at(i)->setObjectName("vecLabelHtaskNames");
        vec_htask_progressbars.at(i)->setObjectName("vecHtaskProgressbars");
        vec_htask_board_names.at(i)->setObjectName("vecHtaskBoardNames");

        //load image for htask
        vec_label_htask_images.at(i)->setPixmap
                (QPixmap(":/image/Image/important_htask_table.png"));

//        vec_label_htask_names.at(i)->setText(tr("Smile everyday!"));
//        vec_htask_board_names.at(i)->setText(tr(":Ice Cream"));

        //make cursor like a hand for a link
        vec_label_htask_names.at(i)->setCursor(Qt::PointingHandCursor);
        vec_htask_board_names.at(i)->setCursor(Qt::PointingHandCursor);

        // set value for making stylesheet for progressbar
        // for next use remove it
        // and make for rest time it
        // look different
        // if have a time - green, medium - orange,yellow
        // and if we have not much time - red
        vec_htask_progressbars.at(i)->setValue(40);

        // setFormat we need to set time that we have for rest of task
        vec_htask_progressbars.at(i)->setFormat("5 days 14h 3m");

        // add all this widgets to layout
        grid_htask_table->addWidget(vec_label_htask_images.at(i),i,0,1,1);
        grid_htask_table->addWidget(vec_label_htask_names.at(i),i,1,1,1);
        grid_htask_table->addWidget(vec_htask_progressbars.at(i),i,2,1,1);
        grid_htask_table->addWidget(vec_htask_board_names.at(i),i,3,1,1);

    }
}


// LATER ADD TO class UserCorrectnessInput
// ???????????
// function that controls size of max length in name board
void RecentWidget::maxLengthNameBoard(QString &name)
{
    // if board name have more than 20 symbols
    // than we end this board with dots (example: -animal...)
    const  unsigned short max_length = 20;

   if (name.length() > max_length)
   {
       name.insert(max_length-3, ".");
       name.insert(max_length-2, ".");
       name.insert(max_length-1, ".");
       name.insert(max_length, " ");
   }
}

// ???????????

void RecentWidget::emptyRecentBoards()
{
    QLabel *label_empty_boards_text_1 = new QLabel(this);
    label_empty_boards_text_1->setObjectName("labelEmptyBoards1");
    label_empty_boards_text_1->setText(tr("No boards yet?"));

    QLabel *label_empty_boards_text_2 = new QLabel(this);
    label_empty_boards_text_2->setObjectName("labelEmptyBoards2");
    label_empty_boards_text_2->setText(tr("Make your first trip to board world"));

    QLabel *label_image_empty_board = new QLabel(this);
    label_image_empty_board->setPixmap(QPixmap(":/image/Image/empty_recentwidget.png"));

    vbox_main->addWidget(label_empty_boards_text_1);
    vbox_main->addSpacing(-5);
    vbox_main->addWidget(label_empty_boards_text_2);
    vbox_main->addSpacing(10);
    vbox_main->addWidget(label_image_empty_board);
    vbox_main->setStretch(0, 1);

    vbox_main->setAlignment(label_image_empty_board, Qt::AlignCenter);
    vbox_main->setAlignment(label_empty_boards_text_1, Qt::AlignCenter);
    vbox_main->setAlignment(label_empty_boards_text_2, Qt::AlignCenter);
}

void RecentWidget::setStackedWidget(QStackedWidget *stackedwidget)
{
    stacked_widget = stackedwidget;
}

void RecentWidget::setListWidget(QListWidget *listwidget)
{
    this->listwidget = listwidget;
}

void RecentWidget::setResizing(QMainWindow *widget)
{
    window_main = widget;
}

// function will call popup window for creating board
void RecentWidget::createBoard()
{ 
    createboard = new CreateBoard(this);
    createboard->setDatabase(database);
    createboard->setUserId(user_id);
    createboard->setStackedWidget(stacked_widget);
    createboard->setListWidget(listwidget);
    createboard->move(GlobalPositioning::parentPos(window_main).x()-
                      createboard->divideDialogWidth(), window_main->y());
   //  now we create effect if we need to make new board
    QGraphicsOpacityEffect *geffect = new QGraphicsOpacityEffect(this);
    geffect->setOpacity(0.2);
    this->setGraphicsEffect(geffect);

    createboard->exec();
    // anyway is we have create it or close dialog
    // we need to hide this effect
    geffect->setEnabled(false);
}

// this function makes deсision what event to make when hover:
// on enter and leave
bool RecentWidget::eventFilter(QObject *watched, QEvent *event)
{
   ExtendedWidget *widget = qobject_cast<ExtendedWidget *>(watched);
    if (event->type() == QEvent::Enter)
    {
         vec_label_remove_boards.at(widget->getWidgetId())->setHidden(false);
            return true;
    }
    else if (event->type() == QEvent::Leave)
    {
         vec_label_remove_boards.at(widget->getWidgetId())->setHidden(true);
            return true;
    }
        return false;
}

 // this function open board with specific board_id
void RecentWidget::openBoardByWidget()
{
   ExtendedWidget *widget = qobject_cast<ExtendedWidget *>(sender());
   // now indicate that we open boardwidget
   listwidget->item(1)->setSelected(true);
   // and let`s send board id to the boardwidget
   boardwidget = new BoardWidget(this);
   boardwidget->setDataBase(database);
   boardwidget->setBoardId(widget_board_id.value(widget->getWidgetId()));
   boardwidget->setResizing(window_main);
   boardwidget->setUserId(user_id);
   boardwidget->loadWidget();
   stacked_widget->addWidget(boardwidget);
   stacked_widget->setCurrentWidget(boardwidget);

}

 // this function delete board with specific board_id
void RecentWidget::deleteBoardByLabel()
{
    ExtendedLabel *label = qobject_cast<ExtendedLabel *>(sender());
    ConfirmDeleteBoard *confirmdeleteboard = new ConfirmDeleteBoard(this);
    confirmdeleteboard->setDataBase(database);
    confirmdeleteboard->setBoardId(widget_board_id.value(label->getLabelId()));
    confirmdeleteboard->setUserId(user_id);

    // there if we multiple by 3, we will set confirmdeleteboard to the center of window
    confirmdeleteboard->move(GlobalPositioning::parentPos(window_main).x()
                             -confirmdeleteboard->divideDialogWidth()*3,  window_main->y());

    // now we are making effect when we have board deletion
    QGraphicsOpacityEffect *geffect = new QGraphicsOpacityEffect(this);
    geffect->setOpacity(0.3);
    this->setGraphicsEffect(geffect);

    vec_widget_boards.at(label->getLabelId())->setGraphicsEffect(geffect);
    confirmdeleteboard->exec();
    // anyway if board deleted or dialog was closed
    // we hide this effect
    geffect->setEnabled(false);

    if(confirmdeleteboard->isBoardDeleted())
    {
          clearWidget();
          loadWidget();
    }
}

// function that open widget that have user all boards
void RecentWidget::showAllBoards()
{
   allboards = new AllBoards(this);
   allboards->setDataBase(database);
   allboards->setUserId(user_id);
   allboards->loadWidget();
   allboards->setListWidgetItem(listwidget);
   allboards->setStackWidget(stacked_widget);
   QGraphicsOpacityEffect *geffect = new QGraphicsOpacityEffect(this);
   geffect->setOpacity(0.3);
   this->setGraphicsEffect(geffect);
   allboards->exec();
   geffect->setEnabled(false);
}

 // function that removes all widgets from recentwidget
void RecentWidget::clearWidget()
{
    reconstructWidget(vbox_main);
    vec_widget_boards.clear();
    vec_vbox_boards.clear();
    vec_label_remove_boards.clear();
    vec_label_name_boards.clear();
    vec_label_created_boards.clear();
    vec_label_date_boards.clear();
    vec_label_htask_images.clear();
    vec_label_htask_names.clear();
    vec_htask_progressbars.clear();
    vec_htask_board_names.clear();
}
