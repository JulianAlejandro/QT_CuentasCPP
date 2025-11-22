#include "addderivativetransactionsdialog.h"
#include "ui_addderivativetransactionsdialog.h"

addDerivativeTransactionsDialog::addDerivativeTransactionsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addDerivativeTransactionsDialog)
{
    ui->setupUi(this);
}

addDerivativeTransactionsDialog::~addDerivativeTransactionsDialog()
{
    delete ui;
}

QTableWidget* addDerivativeTransactionsDialog::getPtrTableWidget(){
    return ui->addDerTranTableWidget;
}
