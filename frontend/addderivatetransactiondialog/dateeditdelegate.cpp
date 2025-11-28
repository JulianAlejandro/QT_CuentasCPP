
#include "dateeditdelegate.h"
#include <QDateEdit>
#include <QDate>

DateEditDelegate::DateEditDelegate(QObject *parent)
    : QItemDelegate{parent}
{}

QWidget *DateEditDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    QDateEdit *dateEdit = new QDateEdit(parent);
    dateEdit->setCalendarPopup(true);  // ¡IMPORTANTE! Para que tenga el calendario desplegable
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setMinimumDate(QDate(1900, 1, 1));
    dateEdit->setMaximumDate(QDate(2100, 12, 31));

    (void)option;
    (void)index;
    return dateEdit;
}

void DateEditDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    QDateEdit *dateEdit = static_cast<QDateEdit *>(editor);

    // Obtener los datos del modelo
    QVariant data = index.data(Qt::EditRole);

    if (data.isValid() && data.canConvert<QDate>()) {
        // Si el modelo ya tiene un QDate
        dateEdit->setDate(data.toDate());
    }
    else if (data.isValid() && data.canConvert<QString>()) {
        // Si el modelo tiene un string, intentar convertirlo a fecha
        QString dateString = data.toString();
        QDate date = QDate::fromString(dateString, "dd/MM/yyyy");

        if (date.isValid()) {
            dateEdit->setDate(date);
        } else {
            // Si no se puede convertir, usar fecha actual
            dateEdit->setDate(QDate::currentDate());
        }
    }
    else {
        // Si no hay datos válidos, usar fecha actual
        dateEdit->setDate(QDate::currentDate());
    }
}

void DateEditDelegate::setModelData(QWidget *editor,
                                    QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QDateEdit *dateEdit = static_cast<QDateEdit *>(editor);

    // Opción 1: Guardar como QDate (recomendado)
    model->setData(index, dateEdit->date(), Qt::EditRole);

    // Opción 2: Si prefieres guardar como string:
    // QString dateString = dateEdit->date().toString("dd/MM/yyyy");
    // model->setData(index, dateString, Qt::EditRole);
}

void DateEditDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
    (void)index;
}


// ¡NO OLVIDES IMPLEMENTAR LA FUNCIÓN paint()!
void DateEditDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);
}


/*
#include "dateeditdelegate.h"
#include <QDateEdit>

DateEditDelegate::DateEditDelegate(QObject *parent)
    : QItemDelegate{parent}
{}


QWidget *DateEditDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const{
    QDateEdit *date = new QDateEdit(parent);
    date->setMinimumDate(QDate(1900, 1, 1));
    date->setMaximumDate(QDate(2100, 12, 31));

    (void) option;
    (void) index;
    return date;
}
void DateEditDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const{
    QDateEdit *date = static_cast<QDateEdit *> (editor);
    QDate d
    date->setDate(d)
}
void DateEditDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const{

}
void DateEditDelegate::updateEditorGeometry (QWidget * editor , const QStyleOptionViewItem & option, const QModelIndex & index) const{

}

*/
