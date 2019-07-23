#ifndef ALLBOARDS_H
#define ALLBOARDS_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QListWidget>

#include "customwidget/extendedlabel.h"
#include "database.h"
#include "customwidget/animateddialog.h"
#include "customwidget/extendedwidget.h"
#include "boardwidget.h"
#include "appsettings.h"

class AllBoards : public AnimatedDialog
{
    Q_OBJECT
public:
    explicit AllBoards(QWidget *parent = nullptr);

// Methods @set
    void setDataBase(DataBase &database);
    void setUserId(QString user_id);
    void setVecUserBoards(QVector<QStringList> vec_user_boards);
    void setListWidgetItem(QListWidget* listwidget);
    void setStackWidget(QStackedWidget *stackedwidget);

// Methods @get
    QVector<QStringList>& getVecUserBoards();

    void loadWidget();
    void initializeWidgets();

    void showListBoards();

public slots:
    void openClickedBoardByWidget();
    void openClickedBoardByLabel();

protected:
    QString getCountBoards();

private:
    QVBoxLayout *vbox_main, *vbox_frame, *vbox_main_allboards;
    QHBoxLayout *hbox_section_dialog;
    ExtendedLabel *label_close;
    DataBase database;
    QString user_id;
    QLabel *label_total_boards, *label_background_image, *label_dialog_name;
    QFrame *frame;
    QWidget *widget_parent_scroll;
    QScrollArea *scrollarea;
    QVector<QStringList> vec_user_boards;
    QVector<ExtendedWidget *> vec_widget_allboards;
    QVector<QVBoxLayout *> vec_vbox_allboards;
    QVector<QLabel *> vec_date_allboards, vec_name_allboards, vec_label_number_board;
    QVector<QHBoxLayout *> vec_hbox_name_button_allboards, vec_hbox_allboards;
    QVector<ExtendedLabel *> vec_button_allboards;
    QMap<unsigned short, QString> widget_board_id;
    BoardWidget *boardwidget;
    QListWidget *listwidget;
    QStackedWidget *stackedwidget;
    AppSettings appsettings;
};

#endif
