#include "tipodelegate.h"
#include <QComboBox>

TipoDelegate::TipoDelegate(QObject* parent)
    : QItemDelegate(parent)
{
}

QWidget* TipoDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox* editor = new QComboBox(parent);
    editor->addItem("Gasto", "gasto");
    editor->addItem("Ingreso", "ingreso");
    editor->setFocusPolicy(Qt::StrongFocus);
    return editor;
}

void TipoDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QComboBox* combo = qobject_cast<QComboBox*>(editor);
    if (!combo) return;

    QString value = index.model()->data(index, Qt::EditRole).toString();
    int idx = combo->findData(value);
    if (idx >= 0) {
        combo->setCurrentIndex(idx);
    }
}

void TipoDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* combo = qobject_cast<QComboBox*>(editor);
    if (!combo) return;

    QString value = combo->currentData().toString();
    QString displayText = combo->currentText();

    QString oldValue = index.data(Qt::EditRole).toString();

    model->setData(index, value, Qt::EditRole);
    model->setData(index, displayText, Qt::DisplayRole);

    if (value != oldValue) {
        emit tipoChanged(index.row(), value);
    }
}

void TipoDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
