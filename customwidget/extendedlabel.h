#ifndef EXTENDEDLABEL_H
#define EXTENDEDLABEL_H

#include <QLabel>

class ExtendedLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ExtendedLabel(QWidget *parent = nullptr);
    explicit ExtendedLabel(const QString& text="", QWidget *parent = nullptr);

    // this functions is very useful when i don`t know what label was hovered or clicked
    // in QVector< ExtendedLabel *> ...
    // if we initialize them with id, we can make qobject_cast<ExtendedLabel *>(object)
    // and get id with this label

    // Methods @set
        void setLabelId(unsigned short id);
    // Methods @get
        unsigned short getLabelId();

signals:
    void clicked();

protected:
    // Events
    void mousePressEvent(QMouseEvent * event) override;

private:
    unsigned short label_id;
};

#endif
