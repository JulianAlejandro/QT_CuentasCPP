/*
#ifndef CATEGORYTREEWIDGETDIALOG_H
#define CATEGORYTREEWIDGETDIALOG_H

#include <QDialog>
#include "commonDataTypes.h"

namespace Ui {
class categoryTreeWidgetDialog;
}


class categoryTreeWidgetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit categoryTreeWidgetDialog(
        QWidget *parent,
        const std::vector<estructuraCategoria> &categorias
        );

    ~categoryTreeWidgetDialog();

private:
    Ui::categoryTreeWidgetDialog *ui;
    std::vector<estructuraCategoria> m_categorias;   // <-- miembro real

};

#endif // CATEGORYTREEWIDGETDIALOG_H

*/
