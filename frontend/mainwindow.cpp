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
#include "commonDataTypes.h"
//#include "categorytreewidgetdialog.h"//prueba

MainWindow::MainWindow(std::shared_ptr<ITransactionsManager> backend, QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
    , _transactionManager(backend)
    , _contadorId(0)  // Inicializar contador de IDs
{
    _ui->setupUi(this);
    setWindowTitle("App");

    // Configurar la tabla
    TableUtils::setFieldsTableWidget(_ui->tableWidget, TableUtils::arrayString_to_QStringList(_transactionManager->getFieldsTableTransactions()), false);
    TableUtils::setFieldsTableWidget(_ui->tableWidget_2, TableUtils::arrayString_to_QStringList(_transactionManager->getFieldsTableDerivativeTransactions()), false);

    // Establecer el selector de fecha para que sea solo meses.
    _ui->dateEdit->setDisplayFormat("MMMM yyyy");
    _ui->dateEdit->setDate(QDate(2026, 6, 1));

    QDate fechaSeleccionada = _ui->dateEdit->date();
    QString fechaMes = fechaSeleccionada.toString("yyyy-MM");

    // ✅ HABILITAR MENÚ CONTEXTUAL EN AMBAS TABLAS
    _ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //_ui->tableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);

    // Conectar las señales de tablas
    connect(_ui->tableWidget, &QTableWidget::customContextMenuRequested,
            this, &MainWindow::onCustomContextMenuRequested);
    connect(_ui->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onRowSelected);

    // ✅ CONECTAR TAMBIÉN LA SEGUNDA TABLA
    //connect(ui->tableWidget_2, &QTableWidget::customContextMenuRequested,
    //        this, &MainWindow::onCustomContextMenuRequested);

    // Cargar datos iniciales usando la interfaz
    //_last_transactionsloaded = _transactionManager->getTransactions();

    _last_transactionsloaded = _transactionManager->getTransactionsByMonth(fechaMes.toStdString());
    TableUtils::loadTransactionsTableWidget(_ui->tableWidget, _last_transactionsloaded, IdRole);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::establecerIdEnFila(int fila, int id)
{
    if (fila < 0 || fila >= _ui->tableWidget->rowCount()) {
        return;
    }

    // Establecer el ID en todos los items de la fila
    for (int col = 0; col < _ui->tableWidget->columnCount(); ++col) {
        QTableWidgetItem* item = _ui->tableWidget->item(fila, col);
        if (item) {
            item->setData(IdRole, id);
        }
    }
}

int MainWindow::generarNuevoId()
{
    // Incrementar el contador y devolver el nuevo ID
    return ++_contadorId;
}

void MainWindow::on_actionA_adir_transaccion_Basica_triggered()
{
    TransaccionBasicaDialog pd(this);
    pd.setWindowTitle("Transacciones brutas");
    pd.setListCurrencies(_transactionManager->getCurrencies());

    // Usar QDialog::Accepted para mayor claridad
    if (pd.exec() == QDialog::Accepted) {
        // Solo aquí se ejecuta si el usuario hizo clic en OK/Aceptar
        _transactionManager->insertNewTransaction(pd.getNewTransaction());
    }
    // Si fue Rejected, no hacer nada (implícitamente)
    _last_transactionsloaded = _transactionManager->getTransactions();
    TableUtils::loadTransactionsTableWidget(_ui->tableWidget, _last_transactionsloaded, IdRole);
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
    QAction *deleteAction = contextMenu->addAction("Eliminar");


    // Acción específica solo para la primera tabla
/*
    if (table == ui->tableWidget) {
        QAction *markProcessedAction = contextMenu->addAction("Marcar como procesado");
        connect(markProcessedAction, &QAction::triggered, this, &MainWindow::onMarkProcessed);
    }
*/
    connect(addDerivativeTransactionAction, &QAction::triggered, this, &MainWindow::onAddDerivativeTransaction);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::onDeleteRow);

    contextMenu->exec(table->viewport()->mapToGlobal(pos));
    delete contextMenu;
}

void MainWindow::onAddDerivativeTransaction()
{
    int currentRow = _ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Aviso",
                             "Debe seleccionar una transacción primero.");
        return;
    }

    // Obtener ID de la transacción padre seleccionada
    int id_t = _ui->tableWidget->item(currentRow, 0)->data(IdRole).toInt();

    // Obtener transacciones derivadas actuales para esta transacción padre
    std::vector<DT_Structure> current_DT = _transactionManager->getDerivativeTransactionsById(id_t);

    // Crear y configurar el diálogo de edición
    addDerivativeTransactionsDialog addDialog(this);
    addDialog.setCategoryStructures(_transactionManager->getCategoryTable());
    addDialog.setWindowTitle("Add/Edit derivative Transactions");
    addDialog.setFieldsTableWidget(
        TableUtils::arrayString_to_QStringList(_transactionManager->getFieldsTableDerivativeTransactions()),
        true);
    addDialog.loadTransactionsTableWidget(current_DT, IdRole);

    for(const auto& ltl : _last_transactionsloaded){
        if (ltl.id == id_t){
            addDialog.setParentAmount(stod(ltl.values[t_AMOUNT]));
        }
    }

    int res = addDialog.exec();
    if (res == QDialog::Rejected) {
        return; // Usuario canceló
    }

    // Obtener las transacciones modificadas del diálogo
    std::vector<DT_Structure> new_DT = addDialog.getDerivativeTransactionsModifications(IdRole);

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

    UpdateResult r = _transactionManager->actualizeDerivativeTransactionsWithId_T(new_DT, id_t); // es necesario pasar el Id_t porque desde el dialogo no se añade.
    if(r == UpdateResult::SumMismatch){
        QMessageBox::warning(this, "Error",
                             "Valores incorrectos de amount");
                             return;
    }

    // Actualizar la tabla de transacciones derivadas en la interfaz
    _last_DerivativeTransactionsLoaded = _transactionManager->getDerivativeTransactionsById(id_t);
    TableUtils::loadTransactionsTableWidget(_ui->tableWidget_2, _last_DerivativeTransactionsLoaded, IdRole);

    QMessageBox::information(this, "Éxito", "Transacciones derivadas actualizadas correctamente.");
}

// Método auxiliar para comparar si dos conjuntos de transacciones son iguales


void MainWindow::onDeleteRow()
{
   // QMessageBox::information(this, "nada", "prueba");

    int currentRow = _ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    int id = _ui->tableWidget->item(currentRow, 0)->data(IdRole).toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Eliminar",
                                  QString("¿Estás seguro de eliminar la transacción ID %1?")
                                      .arg(id),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Aquí llamarías al backend para eliminar por ID
        // transactionManager->eliminarTransaccion(id);

        _transactionManager->deleteTransactionById(id);
        _transactionManager->deleteDerivativeTransactionsBYId_T(id);

        _last_transactionsloaded = _transactionManager->getTransactions();
        TableUtils::loadTransactionsTableWidget(_ui->tableWidget, _last_transactionsloaded, IdRole);
        //ui->tableWidget->removeRow(currentRow);
        qDebug() << "Transacción ID" << id << "eliminada";
    }

}

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
    int currentRow = _ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    int id = _ui->tableWidget->item(currentRow, 0)->data(IdRole).toInt();

    _last_DerivativeTransactionsLoaded = _transactionManager->getDerivativeTransactionsById(id);

    TableUtils::loadTransactionsTableWidget(_ui->tableWidget_2, _last_DerivativeTransactionsLoaded, IdRole);

}

void MainWindow::on_actionQuitar_2_triggered()
{
    close();
}



void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{
    // 1. Convertimos el QDate al formato "yyyy-MM" que tu backend sabe procesar (ej: "2026-06")
    QString fechaMes = date.toString("yyyy-MM");

    // 2. Pedimos al backend las transacciones asociadas a ese mes
    _last_transactionsloaded = _transactionManager->getTransactionsByMonth(fechaMes.toStdString());

    // 3. Cargamos los datos limpios en la primera tabla
    TableUtils::loadTransactionsTableWidget(_ui->tableWidget, _last_transactionsloaded, IdRole);
}

