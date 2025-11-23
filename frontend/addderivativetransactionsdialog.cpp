
#include "addderivativetransactionsdialog.h"
#include "ui_addderivativetransactionsdialog.h"
#include "common/DataTypes.h"
#include "frontend/tableutils.h"
#include <QMessageBox>
#include <QDate>
#include <QStyledItemDelegate>


//void setupTableWidget(QTableWidget* tableWidget, const QStringList& columnTitles, bool edit);

addDerivativeTransactionsDialog::addDerivativeTransactionsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addDerivativeTransactionsDialog)
{
    ui->setupUi(this);

    // Dentro del constructor o justo después de setFieldsTableWidget(...)
  //  connect(ui->TableWidget, &QTableWidget::itemChanged,
  //          this, &addDerivativeTransactionsDialog::validateCell);

}

addDerivativeTransactionsDialog::~addDerivativeTransactionsDialog()
{
    delete ui;
}

void addDerivativeTransactionsDialog::setFieldsTableWidget(const QStringList& columnTitles, bool edit){

    TableUtils::setFieldsTableWidget(ui->TableWidget, columnTitles, edit);
   // TableUtils::setFieldsTableWidget(ui->TableWidget, columnTitles, edit);

    // Deshabilitar edición en las columnas no editables
   // ui->TableWidget->setItemDelegateForColumn(0, new QStyledItemDelegate(this));  // ID
   // ui->TableWidget->setItemDelegateForColumn(4, new QStyledItemDelegate(this));  // id_TB

}
void addDerivativeTransactionsDialog::loadTransactionsTableWidget(std::vector<std::vector<std::string>> transacciones, int IdRole){

    TableUtils::loadTransactionsTableWidget(ui->TableWidget, transacciones, IdRole);

}


void addDerivativeTransactionsDialog::on_addPushButton_clicked()
{

}


void addDerivativeTransactionsDialog::on_savePushButton_clicked()
{

}

void addDerivativeTransactionsDialog::on_quitPushButton_clicked()
{
    close();
}


