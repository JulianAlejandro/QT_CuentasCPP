#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "frontend/transaccionbasicadialog.h"
#include "frontend/addderivativetransactionsdialog.h"
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
    , transaccionManager(std::make_unique<TransactionsManager>())
    , contadorId(0)  // Inicializar contador de IDs
{
    ui->setupUi(this);
    setWindowTitle("App");

    // Configurar la tabla
    TableUtils::setFieldsTableWidget(ui->tableWidget, TableUtils::vectorString_to_QStringList (transaccionManager->getFieldsTableTransactions()), false);
    TableUtils::setFieldsTableWidget(ui->tableWidget_2, TableUtils::vectorString_to_QStringList(transaccionManager->getFieldsTableDerivativeTransactions()), false);

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
    TableUtils::loadTransactionsTableWidget(ui->tableWidget, transaccionManager->getTransactions(), IdRole);
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

    // TODO: Aqui se crea abre una nueva ventana, esta ventan contiene un QTableWidget en el que se pueden añadir nuevas filas.
    // reglas
    //
/*
    int res;
    addDerivativeTransactionsDialog pd(this);
    pd.setWindowTitle("Add/Edit derivative Transactions");
    QTableWidget* table = pd.getPtrTableWidget();

    setupTableWidget(table, vectorString_to_QStringList (transaccionManager->getFieldsTableTransactions()));

    res = pd.exec();
    if (res == QDialog::Rejected){
        return;
    }
*/

    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Aviso",
                             "Debe seleccionar una transacción primero.");
        return;
    }

    // Obtener ID de la transacción seleccionada
    int id = ui->tableWidget->item(currentRow, 0)->data(IdRole).toInt();

    int res;
    addDerivativeTransactionsDialog pd(this);
    pd.setWindowTitle("Add/Edit derivative Transactions");

    pd.setFieldsTableWidget(
        TableUtils::vectorString_to_QStringList(
            transaccionManager->getFieldsTableDerivativeTransactions()
            ), true);


    pd.loadTransactionsTableWidget(transaccionManager->getDerivativeTransactionsById(id), IdRole);

    res = pd.exec();
    if (res == QDialog::Rejected)
        return;

    //pd obtener las nuevas transacciones.
    //en este punto se consiguen las transacciones, despues de esto se va a comparar con lo que esta cargado en tabla..
    //pd.getNewDerivativeTransactions();


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

    TableUtils::loadTransactionsTableWidget(ui->tableWidget_2, transaccionManager->getDerivativeTransactionsById(id), IdRole);
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


