#ifndef HTASKWIDGET_H
#define HTASKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

/*
    this class show for user his
    current hot tasks, which user must
    finish as soon as possible
*/

class HTaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HTaskWidget(QWidget *parent = nullptr);

signals:

public slots:
};

#endif
