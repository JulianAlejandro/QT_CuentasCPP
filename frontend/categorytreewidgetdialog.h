
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
        const std::vector<Category_Structure> &categorias
        );

    ~categoryTreeWidgetDialog();

private:
    Ui::categoryTreeWidgetDialog *ui;
    std::vector<Category_Structure> m_categorias;   // <-- miembro real

};

#endif // CATEGORYTREEWIDGETDIALOG_H


