#ifndef TRANSACCIONBASICADIALOG_H
#define TRANSACCIONBASICADIALOG_H

#include <QDialog>
#include "commonDataTypes.h"
#include <vector>
#include <string>

namespace Ui {
class TransaccionBasicaDialog;
}

class TransaccionBasicaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransaccionBasicaDialog(QWidget *parent = nullptr);
    ~TransaccionBasicaDialog();

    T_Structure getNewTransaction();

    // Cambiado: ahora recibe vector<string> en lugar de QStringList
    void setListCurrencies(const std::vector<std::string> &list);
    void setListCategories(const std::vector<Category_Structure> &list);
    int getSelectedCategoryId() const;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_categorySelectButton_clicked();

private:
    Ui::TransaccionBasicaDialog *ui;
    std::vector<Category_Structure> _categories;
    int _selectedCategoryId;
    QString _selectedCategoryName;
    std::string _selectedTipo;
};

#endif // TRANSACCIONBASICADIALOG_H
