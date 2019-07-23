#include "databaseerror.h"
#include "stylefont.h"

/*
    this class show dialog box error when user
    doesn`t have connection to the database
*/

DataBaseError::DataBaseError(QWidget *parent) : AnimatedDialog(parent)
{
    // this will make our window transparent
    setAttribute(Qt::WA_TranslucentBackground);

    vbox_main = new QVBoxLayout;

    // we need this QFrame because only with this we can make
    // qdialog with rounded corners without pixeling
    // so, we create vbox_main then addwidget(frame) and then frame->setLayout(vbox_frame)
    // after that we add in code  setAttribute(Qt::WA_TranslucentBackground)
    // and write in stylesheet border-radius
    // so with this we will have main frame, that can be rounded
    vbox_frame = new QVBoxLayout;

    frame = new QFrame(this);
    frame->setLayout(vbox_frame);
    frame->setObjectName("frame");

    vbox_main->addWidget(frame);
    /////// for rounding corners

    label_icon = new QLabel(this);
    label_icon->setPixmap(QPixmap(":/image/Image/connection_fail_databaseerror.png"));

    label_text_error = new QLabel(this);
    label_text_error->setText(tr("Connection error"));
    label_text_error->setFont(StyleFont::loadFontSourceCodeVariable());

    label_close_button = new ExtendedLabel(this);
    label_close_button->setPixmap(QPixmap(":/image/Image/close_animateddialog.png"));

    // we will not create stylesheet file for this small task
    // because file will make size for app bigger
    // so there we have our stylesheet for this window
    setStyleSheet("background-color:lavender; color:black; font-size: 15px; border-radius: 5px;"
                  "QFrame#frame { border:1px solid transparent; }");

    vbox_frame->addWidget(label_close_button);
    vbox_frame->addWidget(label_icon);
    vbox_frame->addWidget(label_text_error);

    vbox_frame->setAlignment(label_close_button, Qt::AlignRight);
    vbox_frame->setAlignment(label_icon, Qt::AlignHCenter);
    vbox_frame->setAlignment(label_text_error, Qt::AlignHCenter);

    setLayout(vbox_main);
    resize(250,180);

    connect(label_close_button, SIGNAL(clicked()), this, SLOT(close()));
}
