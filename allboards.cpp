#include "allboards.h"
#include "styleqss.h"
#include "databaseerror.h"
#include "stylefont.h"
#include "usercorrectnessinput.h"
#include "randomcolor.h"

#include <QFrame>
#include <QDateTime>
#include <QScrollBar>

AllBoards::AllBoards(QWidget *parent) : AnimatedDialog(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);

    frame = new QFrame(this);
    frame->setObjectName("frame");

    vbox_main = new QVBoxLayout(this);
    vbox_main->addWidget(frame);

    vbox_frame = new QVBoxLayout;
    frame->setLayout(vbox_frame);

    setLayout(vbox_main);

    setFixedSize(500,600);
}

// Methods @set

void AllBoards::setDataBase(DataBase &database)
{
    this->database = database;
}

void AllBoards::setUserId(QString user_id)
{
    this->user_id = user_id;
}

void AllBoards::setVecUserBoards(QVector<QStringList> vec_user_boards)
{
    this->vec_user_boards = vec_user_boards;
}

void AllBoards::setListWidgetItem(QListWidget *listwidget)
{
    this->listwidget = listwidget;
}

void AllBoards::setStackWidget(QStackedWidget *stackedwidget)
{
    this->stackedwidget = stackedwidget;
}

// Methods @get

QVector<QStringList>& AllBoards::getVecUserBoards()
{
    return vec_user_boards;
}

void AllBoards::loadWidget()
{
    if (database.isConnected())
    {
        // let`s take all user data from database about his boards
       setVecUserBoards(database.getUserDataTBoardFCreated(user_id));
       initializeWidgets();
    }
    else
    {
        DataBaseError *databaseerror = new DataBaseError(this);
        databaseerror->exec();
    }

    if (appsettings.getThemeColor() == 1) // Dark
         setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_allboards_dark.qss"));
    else
         setStyleSheet(StyleQSS::loadQSSfromFile(":/styles/QSS/MAC_allboards_light.qss"));

}

// function that initialize all widgets in this window
void AllBoards::initializeWidgets()
{
    hbox_section_dialog = new QHBoxLayout;

    label_dialog_name = new QLabel(this);
    label_dialog_name->setObjectName("labelDialogName");
    label_dialog_name->setText(tr("My boards"));

    label_close = new ExtendedLabel(this);
    label_close->setObjectName("labelClose");
    label_close->setPixmap(QPixmap(":/image/Image/close_animateddialog.png"));

    hbox_section_dialog->addWidget(label_dialog_name);
    hbox_section_dialog->addWidget(label_close);

    hbox_section_dialog->setAlignment(label_dialog_name, Qt::AlignLeft);
    hbox_section_dialog->setAlignment(label_close, Qt::AlignRight);

    label_background_image = new QLabel(this);
    label_background_image->setObjectName("labelBackgroundImage");
    label_background_image->setPixmap(QPixmap(":/image/Image/background_allboards.png"));
    // the maximum size for perfect background
    label_background_image->setMaximumSize(500,150);

    label_total_boards = new QLabel(this);
    label_total_boards->setObjectName("labelTotalBoards");
    // set how many boards we have
    label_total_boards->setText(UserCorrectnessInput::getCountBoards
                                (getVecUserBoards().size()));

    vbox_frame->setMargin(0);
    vbox_frame->addLayout(hbox_section_dialog);
    vbox_frame->addSpacing(-5);
    vbox_frame->addWidget(label_background_image);
    vbox_frame->addSpacing(-20);
    vbox_frame->addWidget(label_total_boards);


    vbox_frame->setAlignment(label_background_image, Qt::AlignHCenter);
    vbox_frame->setAlignment(label_total_boards, Qt::AlignCenter);

    showListBoards();

    connect(label_close, SIGNAL(clicked()), this, SLOT(reject()));
}

// this function will construct vector widget allbords
void AllBoards::showListBoards()
{
    vbox_main_allboards = new QVBoxLayout;

    scrollarea = new QScrollArea(this);

    // this widget we need for making correct view inside scrollarea
    widget_parent_scroll = new QWidget(this);
    widget_parent_scroll->setLayout(vbox_main_allboards);

    for (unsigned long long i=0; i<getVecUserBoards().size(); i++)
    {

        vec_widget_allboards.append(new ExtendedWidget(this));
        vec_widget_allboards.at(i)->setObjectName("vecWidgetAllBoards");
        vec_widget_allboards.at(i)->setWidgetId(i);
        // max width for perfect dialog view
        vec_widget_allboards.at(i)->setFixedWidth(400);

        vec_widget_allboards.at(i)->setStyleSheet("#vecWidgetAllBoards{ border-radius:5px; padding-bottom: 10px; padding-top: 10px;"
                                                  "} #vecWidgetAllBoards:hover { background:yellow; }");

        RandomColor randomcolor;
        vec_widget_allboards.at(i)->setStyleSheet(vec_widget_allboards.at(i)->styleSheet().
                                                  append("#vecWidgetAllBoards { background:#fffae6; }"));

        // and again let`s create map about our widget id and board id
        widget_board_id.insert(i, getVecUserBoards().at(i).at(0));

        vec_vbox_allboards.append(new QVBoxLayout(this));
        vbox_main_allboards->addLayout(vec_vbox_allboards.at(i));
        vec_vbox_allboards.at(i)->setMargin(0);

        vec_date_allboards.append(new QLabel(this));
        vec_date_allboards.at(i)->setObjectName("vecDateAllBoards");

        // now we convert date
        QDateTime date = QDateTime::fromString(getVecUserBoards().at(i).at(3), "yyyy-MM-ddTHH:mm:ss");
        vec_date_allboards.at(i)->setText(date.toString("dd/MM/yyyy hh:mm"));

        vec_name_allboards.append(new QLabel(this));
        vec_name_allboards.at(i)->setObjectName("vecNameAllBoards");
        vec_name_allboards.at(i)->setText(getVecUserBoards().at(i).at(2));
        vec_name_allboards.at(i)->setFont(StyleFont::loadFontSourceCodeVariable());
        vec_name_allboards.at(i)->setAlignment(Qt::AlignHCenter);

        vec_name_allboards.at(i)->setStyleSheet("font-size:15px;");

        vec_button_allboards.append(new ExtendedLabel(this));
        vec_button_allboards.at(i)->setObjectName("vecButtonAllBoards");
        vec_button_allboards.at(i)->setLabelId(i);
        // max size for perfect image (icon)
        vec_button_allboards.at(i)->setFixedSize(25,25);

        vec_button_allboards.at(i)->setStyleSheet("border-image: url(':/image/Image/button_open_board.png');");

        vec_label_number_board.append(new QLabel(this));
        vec_label_number_board.at(i)->setObjectName("vecLabelNumberBoard");
        vec_label_number_board.at(i)->setText(QString::number(i+1));

        vec_hbox_allboards.append(new QHBoxLayout(this));

        vec_hbox_name_button_allboards.append(new QHBoxLayout(this));
        vec_widget_allboards.at(i)->setLayout(vec_hbox_name_button_allboards.at(i));

        vec_hbox_name_button_allboards.at(i)->addWidget(vec_name_allboards.at(i));
        vec_hbox_name_button_allboards.at(i)->addWidget(vec_button_allboards.at(i));

        vec_hbox_allboards.at(i)->addWidget(vec_label_number_board.at(i));
        vec_hbox_allboards.at(i)->addStretch(1);
        vec_hbox_allboards.at(i)->addWidget(vec_widget_allboards.at(i));
        vec_hbox_allboards.at(i)->setMargin(0);

        vec_vbox_allboards.at(i)->addWidget(vec_date_allboards.at(i));
        vec_vbox_allboards.at(i)->addLayout(vec_hbox_allboards.at(i));
        vec_vbox_allboards.at(i)->addSpacing(10);
        vec_vbox_allboards.at(i)->setMargin(0);

        // style for dark and light

        if (appsettings.getThemeColor() == 1) // Dark
        {
            vec_date_allboards.at(i)->setStyleSheet("font-size:11px; font-weight:bold; color:white;");
            vec_label_number_board.at(i)->setStyleSheet("font-size:10px; border-bottom:1px solid black;"
                                                        " margin-left:10px; color: white;");
        }
        else // Light
        {
            vec_date_allboards.at(i)->setStyleSheet("font-size:11px; font-weight:bold;");
            vec_label_number_board.at(i)->setStyleSheet("font-size:10px; border-bottom:1px solid black;"
                                                        " margin-left:10px;");

        }
    }

        scrollarea->setWidget(widget_parent_scroll);
        vbox_frame->addWidget(scrollarea);

        scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollarea->horizontalScrollBar()->setEnabled(false);

    for (unsigned long long i=0; i<getVecUserBoards().size(); i++)
    {
        connect(vec_widget_allboards.at(i), SIGNAL(clicked()), this, SLOT(openClickedBoardByWidget()));
        connect(vec_button_allboards.at(i), SIGNAL(clicked()), this, SLOT(openClickedBoardByLabel()));
    }
}

void AllBoards::openClickedBoardByWidget()
{
    ExtendedWidget *object = qobject_cast<ExtendedWidget *>(sender());
    // now indicate that we open boardwidget
    listwidget->item(1)->setSelected(true);
    // and let`s send board id to the boardwidget
    boardwidget = new BoardWidget(this);
    boardwidget->setDataBase(database);
    boardwidget->setBoardId(widget_board_id.value(object->getWidgetId()));
    boardwidget->setUserId(user_id);
    stackedwidget->addWidget(boardwidget);
    stackedwidget->setCurrentWidget(boardwidget);
    boardwidget->loadWidget();
    // and now let`s close allboards window
    reject();
}

void AllBoards::openClickedBoardByLabel()
{
    ExtendedLabel *object = qobject_cast<ExtendedLabel *>(sender());
    // now indicate that we open boardwidget
    listwidget->item(1)->setSelected(true);
    // and let`s send board id to the boardwidget
    boardwidget = new BoardWidget(this);
    boardwidget->setDataBase(database);
    boardwidget->setBoardId(widget_board_id.value(object->getLabelId()));
    boardwidget->setUserId(user_id);
    stackedwidget->addWidget(boardwidget);
    stackedwidget->setCurrentWidget(boardwidget);
    boardwidget->loadWidget();
    // and now let`s close allboards window
    reject();
}
