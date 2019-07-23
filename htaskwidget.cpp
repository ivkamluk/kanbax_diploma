#include "htaskwidget.h"

/*
    this class show for user his
    current hot tasks, which user must
    finish as soon as possible
*/

HTaskWidget::HTaskWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vbox_main = new QVBoxLayout(this);
    QLabel *label_test = new QLabel(this);

    label_test->setText("Hot tasks widget");

    vbox_main->addWidget(label_test);

    setLayout(vbox_main);
}
