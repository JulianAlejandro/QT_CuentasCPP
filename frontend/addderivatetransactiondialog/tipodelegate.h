#ifndef TIPODELEGATE_H
#define TIPODELEGATE_H

#include <QItemDelegate>

class TipoDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit TipoDelegate(QObject *parent = nullptr);
signals:
    void tipoChanged(int row, const QString &newTipo) const;
protected:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif
