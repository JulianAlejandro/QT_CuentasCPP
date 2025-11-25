#include "doublespinboxdelegate.h"
#include <QDoubleSpinBox>

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent)
    : QItemDelegate{parent}
{}


QWidget *DoubleSpinBoxDelegate::createEditor(QWidget * parent,
                                       const QStyleOptionViewItem & option,
                                       const QModelIndex & index) const
{
    QDoubleSpinBox *spinner = new QDoubleSpinBox(parent);
    spinner->setMinimum(0.0);
    spinner->setMaximum(1000000.0);  // ✅ Usar valor double apropiado
    spinner->setDecimals(2);         // ✅ Establecer decimales
    spinner->setSingleStep(0.1);     // ✅ Paso incremental decimal

    (void) option;
    (void) index;
    return spinner;
}
void DoubleSpinBoxDelegate::setEditorData(QWidget * editor,
                                    const QModelIndex & index) const
{
    QDoubleSpinBox *spinner = static_cast<QDoubleSpinBox *> (editor);
    double value = index.data(Qt::EditRole).toDouble();  // ✅ Usar toDouble()
    spinner->setValue(value);

}
void DoubleSpinBoxDelegate::setModelData(QWidget * editor,
                                   QAbstractItemModel * model,
                                   const QModelIndex & index) const
{
    QDoubleSpinBox *spinner = static_cast<QDoubleSpinBox *> (editor);
    model->setData(index, spinner->value(), Qt::EditRole);
}
void DoubleSpinBoxDelegate::updateEditorGeometry (QWidget * editor,
                                           const QStyleOptionViewItem & option,
                                           const QModelIndex & index) const
{
    editor->setGeometry(option.rect);
    (void)index;
}
