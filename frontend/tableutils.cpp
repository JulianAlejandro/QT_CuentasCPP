
//#include "tableutils.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QStringList>

 namespace TableUtils {

    void setupTableWidget(QTableWidget* tableWidget,
                          const QStringList& columnTitles,
                          bool edit)
    {
        tableWidget->setColumnCount(columnTitles.size());
        tableWidget->setHorizontalHeaderLabels(columnTitles);

        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        tableWidget->horizontalHeader()->setStretchLastSection(true);

        if (!edit) {
            tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        }
    }

    QStringList vectorString_to_QStringList( std::vector<std::string> v){
        QStringList result;

        for (const auto& i : v) {
            result << QString::fromStdString(i);
        }
        return result;
    }



 void loadTableTransactions(QTableWidget* table, std::vector<std::vector<std::string>> transacciones, int IdRole)
 {

     table->setRowCount(0);

     for (const auto& transaccion : transacciones){
         int newRow = table->rowCount();
         table->insertRow(newRow);

         if (transaccion.empty()) {
            continue; // Saltar transacciones vacías
         }

         int nuevoId = std::stoi(transaccion[0]);

        for (size_t  i = 0; i < transaccion.size(); i++) { //añade para cada transacción
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(transaccion[i]));
            item->setData(IdRole, nuevoId);
            table->setItem(newRow, i, item);
        }
     }

     // Obtener datos a través de la interfaz
    // std::vector<std::vector<std::string>> transacciones = transaccionManager->getTransactions();

    // for (const auto& transaccion : transacciones) { //para cada transaccion
    //      int newRow = ui->tableWidget->rowCount();
    //     ui->tableWidget->insertRow(newRow);

         // Verificar que el vector no esté vacío antes de acceder a [0]
    //     if (transaccion.empty()) {
    //         continue; // Saltar transacciones vacías
    //     }

    //     int nuevoId = std::stoi(transaccion[0]); // almacena id (primer elemento)


    //     for (size_t  i = 0; i < transaccion.size(); i++) { //añade para cada transacción
    //         QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(transaccion[i]));
    //         item->setData(IdRole, nuevoId);
    //         ui->tableWidget->setItem(newRow, i, item);
    //     }
    // }
 }

} // namespace TableUtils

/*
 void MainWindow::onRowSelected()
 {
     int currentRow = ui->tableWidget->currentRow();
     if (currentRow < 0) return;

     int id = ui->tableWidget->item(currentRow, 0)->data(IdRole).toInt();

     // Limpiar tabla existente
     ui->tableWidget_2->setRowCount(0);

     // Obtener datos a través de la interfaz
     std::vector<std::vector<std::string>> transacciones = transaccionManager->getDerivativeTransactionsById(id);

     for (const auto& transaccion : transacciones) {
         int newRow = ui->tableWidget_2->rowCount();
         ui->tableWidget_2->insertRow(newRow);

         // Verificar que el vector no esté vacío antes de acceder a [0]
         if (transaccion.empty()) {
             continue; // Saltar transacciones vacías
         }

         // Usar size() en lugar de lenght() - length() no existe en std::vector
         for (size_t  i = 0; i < transaccion.size(); i++) {
             ui->tableWidget_2->setItem(newRow, i, new QTableWidgetItem(QString::fromStdString(transaccion[i])));
         }
     }
 }
*/
