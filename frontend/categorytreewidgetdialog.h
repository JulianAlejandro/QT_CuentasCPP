
#ifndef CATEGORYTREEWIDGETDIALOG_H
#define CATEGORYTREEWIDGETDIALOG_H

#include <QDialog>
#include "commonDataTypes.h"
#include <QTreeWidgetItem>

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

    // Método para obtener la categoría seleccionada
    QString getSelectedCategoryName() const;
    int getSelectedCategoryId() const;

    ~categoryTreeWidgetDialog();

private slots:
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::categoryTreeWidgetDialog *ui;
    std::vector<Category_Structure> m_categorias;

    // Variables para almacenar la selección
    QString m_selectedName;
    int m_selectedId;
};

#endif // CATEGORYTREEWIDGETDIALOG_H
