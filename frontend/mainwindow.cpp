#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "frontend/transaccionbasicadialog.h"
#include "frontend/addderivatetransactiondialog/addderivativetransactionsdialog.h"
#include <QMessageBox>
#include <QTableWidget>
#include <QHeaderView>
#include "backend/transactionsmanager.h"
#include "frontend/tableutils.h"

#include <QDebug>
#include <set>

//#include "categorytreewidgetdialog.h"//prueba

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , transaccionManager(std::make_unique<TransactionsManager>())
    , contadorId(0)  // Inicializar contador de IDs
{
    ui->setupUi(this);
    setWindowTitle("App");

    // Configurar la tabla
    TableUtils::setFieldsTableWidget(ui->tableWidget, TableUtils::arrayString_to_QStringList(transaccionManager->getFieldsTableTransactions()), false);
    TableUtils::setFieldsTableWidget(ui->tableWidget_2, TableUtils::arrayString_to_QStringList(transaccionManager->getFieldsTableDerivativeTransactions()), false);

    // ✅ HABILITAR MENÚ CONTEXTUAL EN AMBAS TABLAS
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //ui->tableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);

    // Conectar las señales de tablas
    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested,
            this, &MainWindow::onCustomContextMenuRequested);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onRowSelected);

    // ✅ CONECTAR TAMBIÉN LA SEGUNDA TABLA
    //connect(ui->tableWidget_2, &QTableWidget::customContextMenuRequested,
    //        this, &MainWindow::onCustomContextMenuRequested);

    // Cargar datos iniciales usando la interfaz
    last_transactionsloaded = transaccionManager->getTransactions();
    TableUtils::loadTransactionsTableWidget(ui->tableWidget, last_transactionsloaded, IdRole);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
void MainWindow::loadTransactionsInTable() //TODO. MODIFICAR CAMPOS
{

    ui->tableWidget->setRowCount(0);

    // Obtener datos a través de la interfaz
    std::vector<std::vector<std::string>> transacciones = transaccionManager->getTransactions();

    for (const auto& transaccion : transacciones) {
        int newRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(newRow);

        // Verificar que el vector no esté vacío antes de acceder a [0]
        if (transaccion.empty()) {
            continue; // Saltar transacciones vacías
        }

        int nuevoId = std::stoi(transaccion[0]); // almacena id (primer elemento)

        // Usar size() en lugar de lenght() - length() no existe en std::vector
        for (size_t  i = 0; i < transaccion.size(); i++) {
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(transaccion[i]));
            item->setData(IdRole, nuevoId);
            ui->tableWidget->setItem(newRow, i, item);
        }
    }
}
*/
void MainWindow::establecerIdEnFila(int fila, int id)
{
    if (fila < 0 || fila >= ui->tableWidget->rowCount()) {
        return;
    }

    // Establecer el ID en todos los items de la fila
    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
        QTableWidgetItem* item = ui->tableWidget->item(fila, col);
        if (item) {
            item->setData(IdRole, id);
        }
    }
}

int MainWindow::generarNuevoId()
{
    // Incrementar el contador y devolver el nuevo ID
    return ++contadorId;
}

void MainWindow::on_actionA_adir_transaccion_Basica_triggered() //Esto no se puede hacer asi a la ligera
{
    int res;
    TransaccionBasicaDialog pd(this);
    pd.setWindowTitle("Transacciones brutas");
    res = pd.exec();

    if (res == QDialog::Rejected){
        return;
    }

    //TODO: Aqui hay que pensar en como añadir al BBDD cada transacción.

    /*
    int newRow = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(newRow);

    // Generar nuevo ID para esta transacción
    int nuevoId = generarNuevoId();

    // Crear items y asignarles el ID
    QTableWidgetItem* amountItem = new QTableWidgetItem(QString::number(pd.amount()));
    QTableWidgetItem* commentItem = new QTableWidgetItem(pd.comment());
    QTableWidgetItem* dateItem = new QTableWidgetItem(pd.date());
    QTableWidgetItem* currencyItem = new QTableWidgetItem(pd.currency());
    QTableWidgetItem* processedItem = new QTableWidgetItem("No");

    // Asignar el ID a todos los items
    amountItem->setData(IdRole, nuevoId);
    commentItem->setData(IdRole, nuevoId);
    dateItem->setData(IdRole, nuevoId);
    currencyItem->setData(IdRole, nuevoId);
    processedItem->setData(IdRole, nuevoId);
    //processedItem->setData(ProcessedRole, false);

    // Agregar a la tabla
    ui->tableWidget->setItem(newRow, AMOUNT, amountItem);
    ui->tableWidget->setItem(newRow, COMMENT, commentItem);
    ui->tableWidget->setItem(newRow, DATE, dateItem);
    ui->tableWidget->setItem(newRow, CURRENCY, currencyItem);
    ui->tableWidget->setItem(newRow, PROCESSED, processedItem);

    qDebug() << "Nueva transacción agregada con ID:" << nuevoId;
*/

}

void MainWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    QTableWidget* table = qobject_cast<QTableWidget*>(sender());
    if (!table) return;

    QTableWidgetItem *item = table->itemAt(pos);
    if (!item) return;

    QMenu *contextMenu = new QMenu(this);

    // Acciones comunes para ambas tablas
    QAction *addDerivativeTransactionAction = contextMenu->addAction("Añadir o editar transacciones derivadas");
    //QAction *deleteAction = contextMenu->addAction("Eliminar");


    // Acción específica solo para la primera tabla
/*
    if (table == ui->tableWidget) {
        QAction *markProcessedAction = contextMenu->addAction("Marcar como procesado");
        connect(markProcessedAction, &QAction::triggered, this, &MainWindow::onMarkProcessed);
    }
*/
    connect(addDerivativeTransactionAction, &QAction::triggered, this, &MainWindow::onAddDerivativeTransaction);
    //connect(deleteAction, &QAction::triggered, this, &MainWindow::onDeleteRow);

    contextMenu->exec(table->viewport()->mapToGlobal(pos));
    delete contextMenu;
}

void MainWindow::onAddDerivativeTransaction()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Aviso",
                             "Debe seleccionar una transacción primero.");
        return;
    }

    // Obtener ID de la transacción padre seleccionada
    int id_t = ui->tableWidget->item(currentRow, 0)->data(IdRole).toInt();

    // Obtener transacciones derivadas actuales para esta transacción padre
    std::vector<DT_Structure> current_DT = transaccionManager->getDerivativeTransactionsById(id_t);

    // Crear y configurar el diálogo de edición
    addDerivativeTransactionsDialog pd(this);
    pd.setWindowTitle("Add/Edit derivative Transactions");
    pd.setFieldsTableWidget(
        TableUtils::arrayString_to_QStringList(transaccionManager->getFieldsTableDerivativeTransactions()),
        true);
    pd.loadTransactionsTableWidget(current_DT, IdRole);

    int res = pd.exec();
    if (res == QDialog::Rejected) {
        return; // Usuario canceló
    }

    // Obtener las transacciones modificadas del diálogo
    std::vector<DT_Structure> new_DT = pd.getDerivativeTransactionsModifications(IdRole);

    // Validar que no hay valores vacíos
    for (const auto& dt_n : new_DT) {
        for (size_t idx = 0; idx < dt_n.values.size(); ++idx) {
            if (dt_n.values[idx].empty()) {
                QMessageBox::warning(this, "Error",
                                     "No se permiten valores vacíos en las transacciones.");
                return;
            }
        }
    }

    // Si no hay cambios, no hacer nada
    if (transactionsAreEqual(current_DT, new_DT, id_t)) {
        return;
    }

    // Procesar los cambios
    processDerivativeTransactionsChanges(current_DT, new_DT, id_t);

    // Actualizar la tabla de transacciones derivadas en la interfaz
    last_DerivativeTransactionsLoaded = transaccionManager->getDerivativeTransactionsById(id_t);
    TableUtils::loadTransactionsTableWidget(ui->tableWidget_2, last_DerivativeTransactionsLoaded, IdRole);

    QMessageBox::information(this, "Éxito", "Transacciones derivadas actualizadas correctamente.");
}

// Método auxiliar para comparar si dos conjuntos de transacciones son iguales
bool MainWindow::transactionsAreEqual(const std::vector<DT_Structure>& oldTransactions,
                                      const std::vector<DT_Structure>& newTransactions,
                                      int parentId)
{
    // Primero comparar tamaños
    if (oldTransactions.size() != newTransactions.size()) {
        return false;
    }

    // Crear mapas para comparación más eficiente
    std::map<int, DT_Structure> oldMap, newMap;

    for (const auto& dt : oldTransactions) {
        oldMap[dt.id] = dt;
    }

    for (const auto& dt : newTransactions) {
        // Para transacciones nuevas (id = -1), las consideramos diferentes
        if (dt.id == -1) {
            return false;
        }
        newMap[dt.id] = dt;
    }

    // Comparar cada transacción
    for (const auto& [id, oldDT] : oldMap) {
        auto it = newMap.find(id);
        if (it == newMap.end()) {
            return false; // Transacción eliminada
        }

        const auto& newDT = it->second;

        // Comparar todos los valores
        for (size_t i = 0; i < oldDT.values.size(); ++i) {
            if (oldDT.values[i] != newDT.values[i]) {
                return false; // Valor modificado
            }
        }
    }

    return true;
}

// Método para procesar todos los cambios
void MainWindow::processDerivativeTransactionsChanges(
    const std::vector<DT_Structure>& oldTransactions,
    const std::vector<DT_Structure>& newTransactions,
    int parentId)
{
    // Crear mapas para las transacciones
    std::map<int, DT_Structure> oldMap;
    std::map<int, DT_Structure> newMap;

    for (const auto& dt : oldTransactions) {
        oldMap[dt.id] = dt;
    }

    // Crear una copia modificable de newTransactions para establecer id_T
    std::vector<DT_Structure> modifiedNewTransactions = newTransactions;

    for (auto& dt : modifiedNewTransactions) {
        dt.id_T = parentId; // Establecer el ID padre
        if (dt.id != -1) {
            newMap[dt.id] = dt;
        }
    }

    // 1. Encontrar transacciones eliminadas (están en oldMap pero no en newMap)
    for (const auto& [id, oldDT] : oldMap) {
        if (newMap.find(id) == newMap.end()) {
            // Transacción eliminada
            //transaccionManager->eliminarDerivativeTransaction(id);
            transaccionManager->deleteDerivativeTransactionsById(id);
            qDebug() << "Transacción derivada ID" << id << "eliminada";
        }
    }

    // 2. Encontrar transacciones modificadas
    for (const auto& [id, newDT] : newMap) {
        auto oldIt = oldMap.find(id);
        if (oldIt != oldMap.end()) {
            // La transacción existe, verificar si fue modificada
            const auto& oldDT = oldIt->second;
            bool modified = false;

            // Comparar todos los valores
            for (size_t i = 0; i < oldDT.values.size(); ++i) {
                if (oldDT.values[i] != newDT.values[i]) {
                    modified = true;
                    break;
                }
            }

            if (modified) {
                // Transacción modificada
                transaccionManager->actualizeDerivativeTransaction(newDT);
                qDebug() << "Transacción derivada ID" << id << "actualizada";
            }
        }
    }

    // 3. Encontrar transacciones nuevas (id = -1)
    for (const auto& newDT : modifiedNewTransactions) {
        if (newDT.id == -1) {
            // Transacción nueva
            transaccionManager->insertDerivativeTransaction(newDT);
            qDebug() << "Nueva transacción derivada añadida para transacción padre ID" << parentId;
        }
    }
}

// Método auxiliar para obtener transacciones eliminadas
std::vector<int> MainWindow::findDeletedTransactions(
    const std::vector<DT_Structure>& oldTransactions,
    const std::vector<DT_Structure>& newTransactions)
{
    std::vector<int> deletedIds;
    std::set<int> newIds;

    // Recoger todos los IDs de las nuevas transacciones (excluyendo -1)
    for (const auto& dt : newTransactions) {
        if (dt.id != -1) {
            newIds.insert(dt.id);
        }
    }

    // Buscar IDs que están en las viejas pero no en las nuevas
    for (const auto& dt : oldTransactions) {
        if (newIds.find(dt.id) == newIds.end()) {
            deletedIds.push_back(dt.id);
        }
    }

    return deletedIds;
}

// Método auxiliar para obtener transacciones modificadas
std::vector<DT_Structure> MainWindow::findModifiedTransactions(
    const std::vector<DT_Structure>& oldTransactions,
    const std::vector<DT_Structure>& newTransactions)
{
    std::vector<DT_Structure> modifiedTransactions;
    std::map<int, DT_Structure> oldMap;

    // Crear mapa de transacciones antiguas
    for (const auto& dt : oldTransactions) {
        oldMap[dt.id] = dt;
    }

    // Comparar con las nuevas
    for (const auto& newDT : newTransactions) {
        if (newDT.id == -1) continue; // Saltar nuevas

        auto it = oldMap.find(newDT.id);
        if (it != oldMap.end()) {
            const auto& oldDT = it->second;

            // Verificar si hay cambios
            bool changed = false;
            for (size_t i = 0; i < oldDT.values.size(); ++i) {
                if (oldDT.values[i] != newDT.values[i]) {
                    changed = true;
                    break;
                }
            }

            if (changed) {
                modifiedTransactions.push_back(newDT);
            }
        }
    }

    return modifiedTransactions;
}

// Método auxiliar para obtener transacciones nuevas
std::vector<DT_Structure> MainWindow::findNewTransactions(
    const std::vector<DT_Structure>& newTransactions)
{
    std::vector<DT_Structure> newTrans;

    for (const auto& dt : newTransactions) {
        if (dt.id == -1) {
            newTrans.push_back(dt);
        }
    }

    return newTrans;
}

/*
void MainWindow::onDeleteRow()
{

    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    int id = obtenerIdDeFila(currentRow);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Eliminar",
                                  QString("¿Estás seguro de eliminar la transacción ID %1?")
                                      .arg(id),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Aquí llamarías al backend para eliminar por ID
        // transaccionManager->eliminarTransaccion(id);

        ui->tableWidget->removeRow(currentRow);
        qDebug() << "Transacción ID" << id << "eliminada";
    }

}
*/
/*
void MainWindow::onMarkProcessed()
{

    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    int id = obtenerIdDeFila(currentRow);

    QTableWidgetItem *processedItem = ui->tableWidget->item(currentRow, PROCESSED);
    if (processedItem) {
        processedItem->setText("Yes");
        processedItem->setData(ProcessedRole, true);

        // Aquí llamarías al backend para marcar como procesado
        // transaccionManager->marcarComoProcesado(id);

        qDebug() << "Transacción ID" << id << "marcada como procesada";
    }

}
*/
void MainWindow::onRowSelected()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    int id = ui->tableWidget->item(currentRow, 0)->data(IdRole).toInt();

    last_DerivativeTransactionsLoaded = transaccionManager->getDerivativeTransactionsById(id);
    TableUtils::loadTransactionsTableWidget(ui->tableWidget_2, last_DerivativeTransactionsLoaded, IdRole);


    /*
    // ¡Ahora tienes acceso al ID! Puedes hacer cualquier operación con él
    //qDebug() <<  "ID:" << id;


    // TODO. esta funcionalidad de cargar la tabla se añadira en tableutils
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
    */
}

void MainWindow::on_actionQuitar_2_triggered()
{
    close();
}


