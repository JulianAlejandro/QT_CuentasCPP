#include "addderivativetransactionsdialog.h"
#include "ui_addderivativetransactionsdialog.h"
#include "common/DataTypes.h"


//void setupTableWidget(QTableWidget* tableWidget, const QStringList& columnTitles, bool edit);

addDerivativeTransactionsDialog::addDerivativeTransactionsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addDerivativeTransactionsDialog)
{
    ui->setupUi(this);

    /*
    setupTableWidget

    table->setRowCount(0);

    std::vector<std::vector<std::string>> derivadas =
        transaccionManager->getDerivativeTransactionsById(id);

    for (const auto& transaccion : derivadas)
    {
        int newRow = table->rowCount();
        table->insertRow(newRow);

        if (transaccion.empty())
            continue;

        for (size_t i = 0; i < transaccion.size(); i++)
        {
            table->setItem(newRow, i,
                           new QTableWidgetItem(QString::fromStdString(transaccion[i])));
        }
    }
    */
}

addDerivativeTransactionsDialog::~addDerivativeTransactionsDialog()
{
    delete ui;
}

QTableWidget* addDerivativeTransactionsDialog::getPtrTableWidget(){
    return ui->TableWidget;
}



void addDerivativeTransactionsDialog::on_addPushButton_clicked()
{
    //ui->addDerTranTableWidget
}


void addDerivativeTransactionsDialog::on_savePushButton_clicked()
{

}


void addDerivativeTransactionsDialog::on_quitPushButton_clicked()
{
    close();
}


/*
// funcion de apoyo
void setupTableWidget(QTableWidget* tableWidget, const QStringList& columnTitles, bool edit)
{
    // Configurar número de columnas y títulos
    tableWidget->setColumnCount(columnTitles.size());
    tableWidget->setHorizontalHeaderLabels(columnTitles);

    // Configurar selección por filas completas
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    tableWidget->horizontalHeader()->setStretchLastSection(true);

    if(edit == false){
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}
*/
