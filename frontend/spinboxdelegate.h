#ifndef SPINBOXDELEGATE_H
#define SPINBOXDELEGATE_H

#include <QItemDelegate>

class SpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SpinBoxDelegate(QObject *parent = 0);
protected:
    virtual QWidget *createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    virtual void setEditorData(QWidget * editor, const QModelIndex & index) const;
    virtual void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
    virtual void updateEditorGeometry (QWidget * editor , const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // SPINBOXDELEGATE_H
