#include "globalpositioning.h"

#include <QRect>

/*
    this class gives information about
    widget positioning
*/

GlobalPositioning::GlobalPositioning(QObject *parent) : QObject(parent)
{}

// function that will give us position about the
// parent widget
const QPoint& GlobalPositioning::parentPos(QWidget *object)
{
    return object->mapToGlobal(object->rect().center());
}


