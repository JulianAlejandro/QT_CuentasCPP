#ifndef ADDDERIVATIVETRANSACTIONSDIALOG_H
#define ADDDERIVATIVETRANSACTIONSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "commonDataTypes.h"

namespace Ui {
class addDerivativeTransactionsDialog;
}

class QStandardItemModel;
class SpinBoxDelegate;
class DoubleSpinBoxDelegate;
class DateEditDelegate;

class addDerivativeTransactionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDerivativeTransactionsDialog(QWidget *parent = nullptr);
    ~addDerivativeTransactionsDialog();

    void setFieldsTableWidget(const QStringList& columnTitles, bool edit);
    void loadTransactionsTableWidget(const std::vector<DT_Structure>& transacciones, int IdRole);
    std::vector<DT_Structure> getDerivativeTransactionsModifications(const int IdRole);
    void setCategoryStructures(std::vector<Category_Structure> c);
    void setParentAmount(const double parent_amount);

private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
    void borrarFilaSeleccionada();
    void on_addPushButton_clicked();
    void onCellDoubleClicked(const QModelIndex &index);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private:
    void ejecutarProcedimientoEspecial(int fila);
    bool validateAmountSum() const;
    void updateSumValidation();
    void showValidationMessage();

    Ui::addDerivativeTransactionsDialog *ui;
    double p_amount;

    QStandardItemModel *m_modelo;
    DoubleSpinBoxDelegate *m_DoubleSpinnerDelegate;
    DateEditDelegate *m_DateEditDelegate;
    std::vector<Category_Structure> cat_struct;
};

#endif // ADDDERIVATIVETRANSACTIONSDIALOG_H
