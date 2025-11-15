#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "transaccionbasicadialog.h"
#include <QMessageBox>
#include <QTableWidget>
#include <QHeaderView>
#include "backend/transactionsmanager.h"

    MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , transaccionManager(std::make_unique<TransactionsManager>())
    , contadorId(0)  // Inicializar contador de IDs
{
    ui->setupUi(this);
    setWindowTitle("App");

    // Configurar la tabla
    crearTablaTransaccionesBrutas();
    crearTablaTransaccionesNetas();

    // Conectar las señales
    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested,
            this, &MainWindow::onCustomContextMenuRequested);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onRowSelected);

    // Cargar datos iniciales usando la interfaz
    cargarTransaccionesEnTabla();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::crearTablaTransaccionesBrutas()
{
    QStringList titulos;
    ui->tableWidget->setColumnCount(5);
    titulos << "Amount" << "Comment" << "Date" << "Currency" << "Processed";
    ui->tableWidget->setHorizontalHeaderLabels(titulos);

    // Configurar selección por filas completas
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // Ajustar el ancho de las columnas
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    // Habilitar el menú contextual
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}


void MainWindow::crearTablaTransaccionesNetas(){

    QStringList titulos;

    ui->tableWidget_2->setColumnCount(5);
    titulos << "Amount" << "Comment" << "Date" << "id_TB" << "Category";
    ui->tableWidget_2->setHorizontalHeaderLabels(titulos);

    // Configurar selección por filas completas
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);

    // Ajustar el ancho de las columnas
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);

    // Habilitar el menú contextual
    //ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

}

void MainWindow::cargarTransaccionesEnTabla()
{
    // Limpiar tabla existente
    ui->tableWidget->setRowCount(0);

    // Obtener datos a través de la interfaz
    auto transacciones = transaccionManager->getTransactions();

    // Llenar la tabla con los datos
    for (const auto& transaccion : transacciones) {
        int newRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(newRow);

        // Generar un ID único para esta fila
        int nuevoId = transaccion.id;

        // Crear items y asignarles el ID
        QTableWidgetItem* amountItem = new QTableWidgetItem(QString::number(transaccion.amount));
        QTableWidgetItem* commentItem = new QTableWidgetItem(QString::fromStdString(transaccion.comment));
        QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(transaccion.date));
        QTableWidgetItem* currencyItem = new QTableWidgetItem(QString::fromStdString(transaccion.currency));
        QTableWidgetItem* processedItem = new QTableWidgetItem(transaccion.processed ? "Yes" : "No");

        // Asignar el ID a TODOS los items de la fila
        amountItem->setData(IdRole, nuevoId);
        commentItem->setData(IdRole, nuevoId);
        dateItem->setData(IdRole, nuevoId);
        currencyItem->setData(IdRole, nuevoId);
        processedItem->setData(IdRole, nuevoId);
        processedItem->setData(ProcessedRole, transaccion.processed);

        // Agregar a la tabla
        ui->tableWidget->setItem(newRow, AMOUNT, amountItem);
        ui->tableWidget->setItem(newRow, COMMENT, commentItem);
        ui->tableWidget->setItem(newRow, DATE, dateItem);
        ui->tableWidget->setItem(newRow, CURRENCY, currencyItem);
        ui->tableWidget->setItem(newRow, PROCESSED, processedItem);

        qDebug() << "Fila" << newRow << "asignada ID:" << nuevoId;
    }
}

int MainWindow::obtenerIdDeFila(int fila) const
{
    if (fila < 0 || fila >= ui->tableWidget->rowCount()) {
        return -1;
    }

    // Obtener el ID desde cualquier item de la fila (usamos la columna 0)
    QTableWidgetItem* item = ui->tableWidget->item(fila, 0);
    if (item) {
        return item->data(IdRole).toInt();
    }
    return -1;
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

void MainWindow::on_actionA_adir_transaccion_Basica_triggered()
{
    int res;
    TransaccionBasicaDialog pd(this);
    pd.setWindowTitle("Transacciones brutas");
    res = pd.exec();
    if (res == QDialog::Rejected){
        return;
    }

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
    processedItem->setData(ProcessedRole, false);

    // Agregar a la tabla
    ui->tableWidget->setItem(newRow, AMOUNT, amountItem);
    ui->tableWidget->setItem(newRow, COMMENT, commentItem);
    ui->tableWidget->setItem(newRow, DATE, dateItem);
    ui->tableWidget->setItem(newRow, CURRENCY, currencyItem);
    ui->tableWidget->setItem(newRow, PROCESSED, processedItem);

    qDebug() << "Nueva transacción agregada con ID:" << nuevoId;
}

void MainWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem *item = ui->tableWidget->itemAt(pos);
    if (!item) return;

    QMenu *contextMenu = new QMenu(this);
    QAction *editAction = contextMenu->addAction("Editar");
    QAction *deleteAction = contextMenu->addAction("Eliminar");
    QAction *markProcessedAction = contextMenu->addAction("Marcar como procesado");

    connect(editAction, &QAction::triggered, this, &MainWindow::onEditRow);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::onDeleteRow);
    connect(markProcessedAction, &QAction::triggered, this, &MainWindow::onMarkProcessed);

    contextMenu->exec(ui->tableWidget->viewport()->mapToGlobal(pos));
    delete contextMenu;
}

void MainWindow::onEditRow()
{
    /*
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    int id = obtenerIdDeFila(currentRow);
    QMessageBox::information(this, "Editar",
                             QString("Editando transacción - Fila: %1, ID: %2")
                                 .arg(currentRow).arg(id));

    // Aquí puedes implementar la lógica de edición usando el ID
    // Por ejemplo: transaccionManager->editarTransaccion(id, nuevosDatos);
*/
}

void MainWindow::onDeleteRow()
{
    /*
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
*/
}

void MainWindow::onMarkProcessed()
{
    /*
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
*/
}

void MainWindow::onRowSelected()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    int id = obtenerIdDeFila(currentRow);


    // ¡Ahora tienes acceso al ID! Puedes hacer cualquier operación con él
    //qDebug() <<  "ID:" << id;

    // Ejemplo: Obtener más información usando el ID
    // auto infoTransaccion = transaccionManager->obtenerTransaccionPorId(id);

    // Ejemplo: Actualizar otra parte de la UI basada en este ID
    // actualizarDetallesTransaccion(id);

    // Ejemplo: Filtrar otras tablas basado en este ID
    // filtrarTransaccionesRelacionadas(id);

    /*
    QMessageBox::information(this, "Fila Seleccionada",
                             QString("Has seleccionado la transacción con ID: %1")
                                 .arg(id));
    */

    // Limpiar tabla existente
    ui->tableWidget_2->setRowCount(0);

    // Obtener datos a través de la interfaz
    auto transacciones = transaccionManager->getDerivativeTransaccionesById(id);

    // Llenar la tabla con los datos
    for (const auto& transaccion : transacciones) {
        int newRow = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(newRow);

        ui->tableWidget_2->setItem(newRow, 0, new QTableWidgetItem(QString::number(transaccion.amount)));
        ui->tableWidget_2->setItem(newRow, 1, new QTableWidgetItem(QString::fromStdString(transaccion.comment)));
        ui->tableWidget_2->setItem(newRow, 2, new QTableWidgetItem(QString::fromStdString(transaccion.date)));
        ui->tableWidget_2->setItem(newRow, 3, new QTableWidgetItem(QString::number(transaccion.id_TB)));
        ui->tableWidget_2->setItem(newRow, 4, new QTableWidgetItem(QString::fromStdString(transaccion.category)));

    }

}

void MainWindow::on_actionQuitar_2_triggered()
{
    close();
}

