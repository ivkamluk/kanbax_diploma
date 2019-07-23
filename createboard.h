#ifndef CREATEBOARD_H
#define CREATEBOARD_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QKeyEvent>
#include <QString>
#include <QStackedWidget>
#include <QListWidget>
#include <QFrame>

#include "styleqss.h"
#include "customwidget/extendedlabel.h"
#include "database.h"
#include "appsettings.h"
#include "boardwidget.h"
#include "customwidget/animateddialog.h"

/*
    this class show dialog box when user
    user click @Create new board@
*/

class CreateBoard : public AnimatedDialog
{
    Q_OBJECT
public:

    explicit CreateBoard(QWidget *parent = nullptr);

//Methods @set
    void setDatabase(DataBase &database);
    void setUserId(QString user_id);
    void setStackedWidget(QStackedWidget *stackedwidget);
    void setListWidget(QListWidget *listwidget);

public slots:
    void saveBoard();
    void enableButtonSave();

private:
    QVBoxLayout *vbox_main, *vbox_frame;
    QFrame *frame;
    QHBoxLayout *hbox_buttons;
    QLabel *label_title_create;
    QLineEdit *editline_create;
    QPushButton *button_cancel, *button_ok;
    QString user_id;
    DataBase database;
    AppSettings appsettings;
    QStackedWidget *stacked_widget;
    BoardWidget *boardwidget;
    QListWidget *listwidget;
};

#endif
