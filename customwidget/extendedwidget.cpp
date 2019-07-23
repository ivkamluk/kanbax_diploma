#include "extendedwidget.h"

#include <QStyleOption>
#include <QPainter>
#include <QStyle>
#include <QLayoutItem>
#include <QLayout>

// delete later
#include <QDebug>

ExtendedWidget::ExtendedWidget(QWidget *parent) :QWidget(parent)
{}

// this functions is very useful when i don`t know what widget was hovered or clicked
// in QVector<ExtendedWidget *> ...
// if we initialize them with id, we can make qobject_cast<ExtendedWidget *>(object)
// and get id with this widget

//Methods @set
void ExtendedWidget::setWidgetId(unsigned short id)
{
    widget_id = id;
}

//Methods @get
unsigned short ExtendedWidget::getWidgetId()
{
    return widget_id;
}

// function that with recursion deletes all layouts and widgets inside this widget
void ExtendedWidget::reconstructWidget(QLayout* layout)
{
    QLayoutItem* child;
       while (layout->count() != 0)
       {
           child = layout->takeAt (0);

           if (child->layout() != 0)
               reconstructWidget(child->layout());
             else if (child->widget() != 0)
               delete child->widget();

           delete child;
       }
       update();
}

// Events

void ExtendedWidget::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}

//it`s very important, that custom widgets must reimplement paintEvent
// without it i can`t build styles for the - QSS

// Addition!!!
// Quote: "Direct subclasses of QWidget still need that paintEvent()
// override even if you provide Q_OBJECT"
void ExtendedWidget::paintEvent(QPaintEvent *pevent)
{
    QStyleOption option;
    option.initFrom(this);
    QPainter paint(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &paint, this);
}
