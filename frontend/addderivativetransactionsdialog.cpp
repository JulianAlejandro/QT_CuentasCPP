
#include "addderivativetransactionsdialog.h"
#include "ui_addderivativetransactionsdialog.h"
#include "common/DataTypes.h"
#include "frontend/tableutils.h"
#include <QMessageBox>
#include <QDate>
#include <QStyledItemDelegate>


#include "spinboxdelegate.h"
#include "doublespinboxdelegate.h"
#include "dateeditdelegate.h"
#include <QStandardItemModel>
//#include "sliderdelegate.h"
//#include "fontcomboboxdelegate.h"

//void setupTableWidget(QTableWidget* tableWidget, const QStringList& columnTitles, bool edit);

addDerivativeTransactionsDialog::addDerivativeTransactionsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addDerivativeTransactionsDialog)
{
    ui->setupUi(this);


    //ui->setupUi(this);
    m_modelo = new QStandardItemModel(this);
    m_spinnerDelegate_id = new SpinBoxDelegate(this);
    m_DoubleSpinnerDelegate = new DoubleSpinBoxDelegate(this);
    m_spinnerDelegate_id_TB = new SpinBoxDelegate(this);
    m_spinnerDelegate_category_id = new SpinBoxDelegate(this);
    m_DateEditDelegate = new DateEditDelegate(this);

   // m_sliderDelegate = new SliderDelegate(this);
   // m_fcbDelegate = new FontComboBoxDelegate(this);

    m_modelo->setRowCount(7);
    m_modelo->setColumnCount(7);

    QStringList headers;
    headers << "id" << "amount" << "comment" << "date" << "Id_TB" << "Category" << "Category_Id";
    m_modelo->setHorizontalHeaderLabels(headers);

    ui->tableView->setModel(m_modelo);
    ui->tableView->setItemDelegateForColumn(0, m_spinnerDelegate_id);
    ui->tableView->setItemDelegateForColumn(1, m_DoubleSpinnerDelegate);
    ui->tableView->setItemDelegateForColumn(3, m_DateEditDelegate);
    ui->tableView->setItemDelegateForColumn(4, m_spinnerDelegate_id_TB);
    ui->tableView->setItemDelegateForColumn(6, m_spinnerDelegate_category_id);


    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch)   ;opcional pensar si poner.

    // Dentro del constructor o justo después de setFieldsTableWidget(...)
  //  connect(ui->TableWidget, &QTableWidget::itemChanged,
  //          this, &addDerivativeTransactionsDialog::validateCell);

}

addDerivativeTransactionsDialog::~addDerivativeTransactionsDialog()
{
    delete ui;
}

void addDerivativeTransactionsDialog::setFieldsTableWidget(const QStringList& columnTitles, bool edit){

    //TableUtils::setFieldsTableWidget(ui->TableWidget, columnTitles, edit);

}
void addDerivativeTransactionsDialog::loadTransactionsTableWidget(std::vector<std::vector<std::string>> transacciones, int IdRole){

    //TableUtils::loadTransactionsTableWidget(ui->TableWidget, transacciones, IdRole);

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


