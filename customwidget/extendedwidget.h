#ifndef EXTENDEDWIDGET_H
#define EXTENDEDWIDGET_H

#include <QWidget>

class ExtendedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExtendedWidget(QWidget *parent = nullptr);

    // this functions is very useful when i don`t know what widget was hovered or clicked
    // in QVector<ExtendedWidget *> ...
    // if we initialize them with id, we can make qobject_cast<ExtendedWidget *>(object)
    // and get id with this widget

  //Methods @set
        void setWidgetId(unsigned short id);

  //Methods @get
        unsigned short getWidgetId();

    // function that with recursion deletes all layouts and widgets inside this widget
        void reconstructWidget(QLayout* layout);

signals:
    void clicked();
protected:
   // Events
    void mousePressEvent(QMouseEvent * event) override;

    //it`s very important, that custom widgets must reimplement paintEvent
    // without it i can`t build styles for the - QSS
    void paintEvent(QPaintEvent *pevent) override;

private:
    unsigned short widget_id;
};

#endif
