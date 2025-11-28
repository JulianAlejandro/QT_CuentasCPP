#ifndef ADDDERIVATIVETRANSACTIONSDIALOG_H
#define ADDDERIVATIVETRANSACTIONSDIALOG_H

#include <QDialog>
#include <QTableWidget>

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
    void loadTransactionsTableWidget(std::vector<std::vector<std::string>> transacciones, int IdRole);
    std::vector<std::vector<std::string>> getNewDerivativeTransactions();

private slots:

    void on_addPushButton_clicked();

    // En addderivativetransactionsdialog.h
    void onCellDoubleClicked(const QModelIndex &index);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::addDerivativeTransactionsDialog *ui;
    QStandardItemModel *m_modelo;
    SpinBoxDelegate *m_spinnerDelegate_id;
    DoubleSpinBoxDelegate *m_DoubleSpinnerDelegate;
    SpinBoxDelegate *m_spinnerDelegate_id_TB;
    SpinBoxDelegate *m_spinnerDelegate_category_id;
    DateEditDelegate *m_DateEditDelegate;
};

#endif // ADDDERIVATIVETRANSACTIONSDIALOG_H
