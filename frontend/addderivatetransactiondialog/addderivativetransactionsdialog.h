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
    //QTableWidget* getPtrTableWidget();
    void setFieldsTableWidget(const QStringList& columnTitles, bool edit);
    void loadTransactionsTableWidget(const std::vector<DT_Structure>& transacciones, int IdRole);
    std::vector<DT_Structure> getDerivativeTransactionsModifications(const int IdRole);

private slots:

    void onCustomContextMenuRequested(const QPoint &pos);
    void borrarFilaSeleccionada();

    void on_addPushButton_clicked();

    // En addderivativetransactionsdialog.h
    void onCellDoubleClicked(const QModelIndex &index);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::addDerivativeTransactionsDialog *ui;
    QStandardItemModel *m_modelo;
    //SpinBoxDelegate *m_spinnerDelegate_id;
    DoubleSpinBoxDelegate *m_DoubleSpinnerDelegate;
    //SpinBoxDelegate *m_spinnerDelegate_id_TB;
    //SpinBoxDelegate *m_spinnerDelegate_category_id;
    DateEditDelegate *m_DateEditDelegate;
};

#endif // ADDDERIVATIVETRANSACTIONSDIALOG_H
