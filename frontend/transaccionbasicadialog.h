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

    void setCategoryStructures(const std::vector<Category_Structure> &cats);
    int getSelectedCategoryId() const;
    std::string getSelectedCategoryName() const;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_categoryPushButton_clicked();
    void on_tipoComboBox_currentIndexChanged(int index);

private:
    Ui::TransaccionBasicaDialog *ui;
    std::vector<Category_Structure> m_categorias;
    int m_selectedCategoryId = -1;
    std::string m_selectedCategoryName;
};

#endif // TRANSACCIONBASICADIALOG_H
