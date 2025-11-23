#ifndef ADDDERIVATIVETRANSACTIONSDIALOG_H
#define ADDDERIVATIVETRANSACTIONSDIALOG_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class addDerivativeTransactionsDialog;
}

class addDerivativeTransactionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDerivativeTransactionsDialog(QWidget *parent = nullptr);
    ~addDerivativeTransactionsDialog();
    QTableWidget* getPtrTableWidget();

private slots:


    void on_addPushButton_clicked();

    void on_savePushButton_clicked();

    void on_quitPushButton_clicked();

private:
    Ui::addDerivativeTransactionsDialog *ui;
};

#endif // ADDDERIVATIVETRANSACTIONSDIALOG_H
