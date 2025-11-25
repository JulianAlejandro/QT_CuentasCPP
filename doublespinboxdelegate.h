#ifndef DOUBLESPINBOXDELEGATE_H
#define DOUBLESPINBOXDELEGATE_H

#include <QItemDelegate>

class DoubleSpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit DoubleSpinBoxDelegate(QObject *parent = nullptr);
protected:
    virtual QWidget *createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    virtual void setEditorData(QWidget * editor, const QModelIndex & index) const;
    virtual void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
    virtual void updateEditorGeometry (QWidget * editor , const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // DOUBLESPINBOXDELEGATE_H
