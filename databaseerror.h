#ifndef DATABASEERROR_H
#define DATABASEERROR_H

#include "customwidget/animateddialog.h"
#include "customwidget/extendedlabel.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>

/*
    this class show dialog box error when user
    doesn`t have connection to the database
*/

class DataBaseError : public AnimatedDialog
{
    Q_OBJECT
public:
    explicit DataBaseError(QWidget *parent = nullptr);

private:
    QVBoxLayout *vbox_main, *vbox_frame;
    QFrame *frame;
    QLabel *label_icon, *label_text_error;
    ExtendedLabel *label_close_button;
};

#endif
