#include "randomcolor.h"

#include <QString>
#include <QColor>
#include <QRandomGenerator>

// delete later
#include <QDebug>

RandomColor::RandomColor()
{}

void RandomColor::setRandomColor(QWidget *widget, int value_lighter)
{
    QColor color(generateRandomColor(value_lighter));

    QString random_color = QString("background: rgb(%1, %2, %3);").arg(QString::number(color.red())).
                                                                   arg(QString::number(color.green())).
                                                                   arg(QString::number(color.blue()));

    widget->setStyleSheet(random_color);
}

QString RandomColor::randomColorStylesheet(QString object_name, int value_lighter)
{
     QColor color(generateRandomColor(value_lighter));

     QString random_color = QString(object_name + "{ background: rgb(%1, %2, %3); }").
                                                                  arg(QString::number(color.red())).
                                                                  arg(QString::number(color.green())).
                                                                  arg(QString::number(color.blue()));
     return random_color;
}

QColor RandomColor::generateRandomColor(int value_lighter)
{
    QColor color(qrand() % 200,
                 qrand() % 200,
                 qrand() % 200);

    color = color.lighter(value_lighter);

    return color;
}
