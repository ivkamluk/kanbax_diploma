#ifndef RECENTWIDGET_H
#define RECENTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QVector>
#include <QPushButton>
#include <QGridLayout>
#include <QProgressBar>
#include <QString>
#include <QStackedWidget>
#include <QListWidget>
#include <QMap>
#include <QList>
#include <QMainWindow>

#include "database.h"
#include "customwidget/extendedlabel.h"
#include "customwidget/extendedwidget.h"
#include "createboard.h"
#include "databaseerror.h"
#include "allboards.h"
#include "boardsettings.h"
#include "appsettings.h"

/*
    this class display all recent boards
    and hot tasks that user have
*/

class RecentWidget : public ExtendedWidget
{
    Q_OBJECT
public:
    explicit RecentWidget(QWidget *parent = nullptr);

    void setHtaskNumber(unsigned short htask_number);

    unsigned short getHtaskNumber();

    void setDatabase(DataBase& database);

    void setUserId(QString user_id);

    void initializeBoards(unsigned short boards);

    void initializeCreateBoard();

    void initializeHTasks(unsigned short htasks);

    void maxLengthNameBoard(QString &name);

    void emptyRecentBoards();

    void setStackedWidget(QStackedWidget *stackedwidget);

    void setListWidget(QListWidget *listwidget);

    void setResizing(QMainWindow *widget);

    void clearWidget();

    void loadWidget();

public slots:
    // function will call popup window for creating board
    void createBoard();
    // this function makes deсision what event to make when hover:
    // on enter and leave
    bool eventFilter(QObject *watched, QEvent *event);
    // this function open board with specific board_id
    void openBoardByWidget();
    // this function delete board with specific board_id
    void deleteBoardByLabel();
    // function that open widget that have user all boards
    void showAllBoards();

private:
    unsigned short boards_show_number, htask_number; //it`s controlling by user settings
    QString user_id;
    QVBoxLayout *vbox_main;
    QHBoxLayout *hbox_top_boards, *hbox_boads, *hbox_create_board;
    QLabel *label_last_boards,*label_htask_main;
    ExtendedLabel *label_create_board;
    QPushButton *button_allboards,*button_create_board;
    QList<ExtendedLabel *> vec_label_remove_boards;
    QList<ExtendedWidget *> vec_widget_boards;
    QList<QVBoxLayout *>vec_vbox_boards;
    QList<QLabel *> vec_label_name_boards,
    vec_label_created_boards,vec_label_date_boards,vec_label_htask_images,
    vec_label_htask_names,vec_htask_board_names;
    QVector<QProgressBar *> vec_htask_progressbars;
    QVector<QStringList> user_boards;
    QWidget *widget_htask_table;
    ExtendedWidget *widget_create_board;
    QGridLayout *grid_htask_table;
    DataBase database;
    DataBaseError *databaseerror;
    CreateBoard *createboard;
    QStackedWidget *stacked_widget;
    QListWidget *listwidget;
    BoardWidget *boardwidget;
    QMainWindow *window_main;
    QMap<unsigned short, QString> widget_board_id;
    AllBoards *allboards;
    AppSettings appsettings;
};

#endif
