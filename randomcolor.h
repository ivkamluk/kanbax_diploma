#ifndef RANDOMCOLOR_H
#define RANDOMCOLOR_H

#include <QWidget>

class RandomColor
{
public:
    RandomColor();

    void setRandomColor(QWidget *widget, int value_lighter = 200);
    QString randomColorStylesheet(QString object_name, int value_lighter = 200);


protected:
        QColor generateRandomColor(int value_lighter = 200);
};

#endif
