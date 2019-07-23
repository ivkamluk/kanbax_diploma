#ifndef EXTENDEDCOMBOBOX_H
#define EXTENDEDCOMBOBOX_H

#include <QWidget>
#include <QComboBox>
#include <QStandardItemModel>

class ExtendedComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ExtendedComboBox(QWidget *parent = nullptr);

    //function that allows to make in combobox default parameter
    void setComboBoxPlaceHolderText(QComboBox *combobox, QString text);

    void disableItem(unsigned short index);
    void enableItem(unsigned short index);
};

#endif
