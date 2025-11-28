
#ifndef DATEEDITDELEGATE_H
#define DATEEDITDELEGATE_H

#include <QItemDelegate>
#include <QPainter>

class DateEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit DateEditDelegate(QObject *parent = nullptr);

protected:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const override;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
};

#endif // DATEEDITDELEGATE_H


/*
#ifndef DATEEDITDELEGATE_H
#define DATEEDITDELEGATE_H

#include <QItemDelegate>

class DateEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit DateEditDelegate(QObject *parent = nullptr);

protected:
    virtual QWidget *createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    virtual void setEditorData(QWidget * editor, const QModelIndex & index) const;
    virtual void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
    virtual void updateEditorGeometry (QWidget * editor , const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // DATEEDITDELEGATE_H

*/
