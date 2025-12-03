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

//#include "categorytreewidgetdialog.h"//prueba

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , transactionManager(std::make_unique<TransactionsManager>())
    , contadorId(0)  // Inicializar contador de IDs
{
    ui->setupUi(this);
    setWindowTitle("App");

    // Configurar la tabla
    TableUtils::setFieldsTableWidget(ui->tableWidget, TableUtils::arrayString_to_QStringList(transactionManager->getFieldsTableTransactions()), false);
    TableUtils::setFieldsTableWidget(ui->tableWidget_2, TableUtils::arrayString_to_QStringList(transactionManager->getFieldsTableDerivativeTransactions()), false);

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
    last_transactionsloaded = transactionManager->getTransactions();
    TableUtils::loadTransactionsTableWidget(ui->tableWidget, last_transactionsloaded, IdRole);
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
    std::vector<DT_Structure> current_DT = transactionManager->getDerivativeTransactionsById(id_t);

    // Crear y configurar el diálogo de edición
    addDerivativeTransactionsDialog pd(this);
    pd.setCategoryStructures(transactionManager->getCategoryTable());
    pd.setWindowTitle("Add/Edit derivative Transactions");
    pd.setFieldsTableWidget(
        TableUtils::arrayString_to_QStringList(transactionManager->getFieldsTableDerivativeTransactions()),
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

    //actualizar la tabla
    transactionManager->actualizeDerivativeTransactionsById_T(current_DT, new_DT, id_t);

    // Actualizar la tabla de transacciones derivadas en la interfaz
    last_DerivativeTransactionsLoaded = transactionManager->getDerivativeTransactionsById(id_t);
    TableUtils::loadTransactionsTableWidget(ui->tableWidget_2, last_DerivativeTransactionsLoaded, IdRole);

    QMessageBox::information(this, "Éxito", "Transacciones derivadas actualizadas correctamente.");
}

// Método auxiliar para comparar si dos conjuntos de transacciones son iguales





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
        // transactionManager->eliminarTransaccion(id);

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
        // transactionManager->marcarComoProcesado(id);

        qDebug() << "Transacción ID" << id << "marcada como procesada";
    }

}
*/
void MainWindow::onRowSelected()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    int id = ui->tableWidget->item(currentRow, 0)->data(IdRole).toInt();

    last_DerivativeTransactionsLoaded = transactionManager->getDerivativeTransactionsById(id);
    TableUtils::loadTransactionsTableWidget(ui->tableWidget_2, last_DerivativeTransactionsLoaded, IdRole);

}

void MainWindow::on_actionQuitar_2_triggered()
{
    close();
}


