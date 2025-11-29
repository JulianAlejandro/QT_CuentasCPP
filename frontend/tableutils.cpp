
//#include "tableutils.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QStringList>

 namespace TableUtils {

    void setFieldsTableWidget(QTableWidget* tableWidget,
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
/*
    QStringList vectorString_to_QStringList( std::vector<std::string> v){
        QStringList result;

        for (const auto& i : v) {
            result << QString::fromStdString(i);
        }
        return result;
    }
    */
    /*
    void loadTransactionsTableWidget(QTableWidget* table, std::vector<std::vector<std::string>> transacciones, int IdRole) //TODO
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
    }
    */



    bool compareTransactions(const std::vector<std::vector<std::string>>& v1,
                 const std::vector<std::vector<std::string>>& v2) {
        // Verificar tamaño del vector externo
        if (v1.size() != v2.size()) {
            return false;
        }

        // Comparar cada vector interno
        for (size_t i = 0; i < v1.size(); ++i) {
            // Verificar tamaño del vector interno
            if (v1[i].size() != v2[i].size()) {
                return false;
            }

            // Comparar cada string
            for (size_t j = 0; j < v1[i].size(); ++j) {
                 if (v1[i][j] != v2[i][j]) {
                     return false;
                 }
            }
        }

        return true;
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
