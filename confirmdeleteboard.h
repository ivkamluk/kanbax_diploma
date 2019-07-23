#ifndef CONFIRMDELETEBOARD_H
#define CONFIRMDELETEBOARD_H

#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QListWidget>

#include "database.h"
#include "customwidget/animateddialog.h"
#include "appsettings.h"


/*
    this class show dialog box for confirmation
     when user want to delete a board
*/

class ConfirmDeleteBoard : public AnimatedDialog
{
    Q_OBJECT
public:
    explicit ConfirmDeleteBoard(QWidget *parent = nullptr);

  //Method @set
    void setDataBase(DataBase &database);
    void setBoardId(QString board_id);
    void setUserId(QString user_id);

    bool isBoardDeleted();

public slots:
    void deleteUserBoard();

private:
     bool board_deleted;
     QVBoxLayout *vbox_main, *vbox_frame;
     QHBoxLayout *hbox_buttons;
     QFrame *frame;
     DataBase database;
     QString board_id, user_id;
     QStackedWidget *stacked_widget;
     QListWidget *list_widget;
     QPushButton *button_cancel, *button_delete;
     QLabel *label_confirmation_text;
     AppSettings appsettings;
};

#endif
