#include "customwidget/extendedcombobox.h"

ExtendedComboBox::ExtendedComboBox(QWidget *parent) : QComboBox(parent)
{}

//function that allows to make in combobox default parameter

// This function set default text for combobox
/// [Please select a country    v] - and this field
/// will be unselectable
/// [...]
/// [...]
void ExtendedComboBox::setComboBoxPlaceHolderText(QComboBox *combobox, QString text)
{
    QString placeholder_text;
    //combobox->setCurrentIndex(0);
   // 0 Index will be unselectable in combobox
   // by this way we will make PlaceHolderText
   placeholder_text.append(text);
   combobox->addItem(placeholder_text);

   // Cast combobox to StandardItemModel
   QStandardItemModel* itemmodel =
           qobject_cast<QStandardItemModel*>(combobox->model());
   QModelIndex modelindex = itemmodel->index(0,
                  combobox->modelColumn(),
                combobox->rootModelIndex());
   QStandardItem* item = itemmodel->itemFromIndex(modelindex);
   item->setSelectable(false);
}


void ExtendedComboBox::disableItem(unsigned short index)
{
    QModelIndex indexdata = this->model()->index(index,0);
    // This is the effective 'disable' flag
    QVariant v(0);
    //the magic
    this->model()->setData(indexdata, v, Qt::UserRole -1);
}

void ExtendedComboBox::enableItem(unsigned short index)
{
    QModelIndex indexdata = this->model()->index(index,0);
    // This is the effective 'disable' flag
    QVariant v(1 | 32);
    //the magic
    this->model()->setData(indexdata, v, Qt::UserRole -1);
}
