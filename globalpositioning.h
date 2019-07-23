#ifndef GLOBALPOSITIONING_H
#define GLOBALPOSITIONING_H

#include <QObject>
#include <QWidget>

/*
    this class gives information about
    widget positioning
*/

class GlobalPositioning : public QObject
{
    Q_OBJECT
public:
    explicit GlobalPositioning(QObject *parent = nullptr);

    // function that will give us position about the
    // parent widget
    static const QPoint& parentPos(QWidget *object);

};

#endif
