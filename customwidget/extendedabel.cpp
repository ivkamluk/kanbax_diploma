#include "extendedlabel.h"

ExtendedLabel::ExtendedLabel(QWidget *parent)
    : QLabel(parent)
{}

ExtendedLabel::ExtendedLabel(const QString& text, QWidget *parent)
    : QLabel(parent)
{
    setText(text);
}

// this functions is very useful when i don`t know what label was hovered or clicked
// in QVector< ExtendedLabel *> ...
// if we initialize them with id, we can make qobject_cast<ExtendedLabel *>(object)
// and get id with this label

// Methods @set
void ExtendedLabel::setLabelId(unsigned short id)
{
    label_id = id;
}

// Methods @get
unsigned short ExtendedLabel::getLabelId()
{
    return label_id;
}

// Events

void ExtendedLabel::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
