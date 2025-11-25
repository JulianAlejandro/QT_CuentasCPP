#include "spinboxdelegate.h"
#include <QSpinBox>

SpinBoxDelegate::SpinBoxDelegate(QObject * parent) :
    QItemDelegate(parent)
{
}

QWidget *SpinBoxDelegate::createEditor(QWidget * parent,
                                const QStyleOptionViewItem & option,
                                const QModelIndex & index) const
{
    QSpinBox *spinner = new QSpinBox(parent);
    spinner->setMinimum(INT_MIN);
    spinner->setMaximum(INT_MAX);
    (void) option;
    (void) index;
    return spinner;
}
void SpinBoxDelegate::setEditorData(QWidget * editor,
                                const QModelIndex & index) const
{
    QSpinBox *spinner = static_cast<QSpinBox *> (editor);
    spinner->setValue(index.data().toInt());
    //spinner->selectAll();

}
void SpinBoxDelegate::setModelData(QWidget * editor,
                                QAbstractItemModel * model,
                                const QModelIndex & index) const
{
    QSpinBox *spinner = static_cast<QSpinBox *> (editor);
    model->setData(index, spinner->value());
}
void SpinBoxDelegate::updateEditorGeometry (QWidget * editor,
                                const QStyleOptionViewItem & option,
                                const QModelIndex & index) const
{
    editor->setGeometry(option.rect);
    (void) index;
}
