#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "transaccionbasicadialog.h"
#include <QMessageBox>
#include <QTableWidget>
#include <QHeaderView>
#include "backend/gestortransacciones.h"  // Solo para crear la instancia

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , transaccionManager(std::make_unique<gestorTransacciones>())  // Crear instancia a través de interfaz
{
    ui->setupUi(this);
    setWindowTitle("App");

    // Configurar la tabla
    crearTablaTransaccionesBrutas();

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

void MainWindow::cargarTransaccionesEnTabla()
{
    // Limpiar tabla existente
    ui->tableWidget->setRowCount(0);

    // Obtener datos a través de la interfaz - ¡Aquí está la magia!
    auto transacciones = transaccionManager->getTransaccionesBrutas();

    // Llenar la tabla con los datos
    for (const auto& transaccion : transacciones) {
        int newRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(newRow);

        // Agregar los datos a las celdas
        ui->tableWidget->setItem(newRow, AMOUNT,
                                 new QTableWidgetItem(QString::number(transaccion.amount)));
        ui->tableWidget->setItem(newRow, COMMENT,
                                 new QTableWidgetItem(QString::fromStdString(transaccion.comment)));
        ui->tableWidget->setItem(newRow, DATE,
                                 new QTableWidgetItem(QString::fromStdString(transaccion.date)));
        ui->tableWidget->setItem(newRow, CURRENCY,
                                 new QTableWidgetItem(QString::fromStdString(transaccion.currency)));
        ui->tableWidget->setItem(newRow, PROCESSED,
                                 new QTableWidgetItem(transaccion.processed ? "Yes" : "No"));
    }
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

    // Aquí podrías agregar la transacción a través de la interfaz también
    // Por ahora mantenemos la lógica existente de UI

    int newRow = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(newRow);

    // Agregar los datos a las celdas
    ui->tableWidget->setItem(newRow, AMOUNT,
                             new QTableWidgetItem(QString::number(pd.amount())));
    ui->tableWidget->setItem(newRow, COMMENT,
                             new QTableWidgetItem(pd.comment()));
    ui->tableWidget->setItem(newRow, DATE,
                             new QTableWidgetItem(pd.date()));
    ui->tableWidget->setItem(newRow, CURRENCY,
                             new QTableWidgetItem(pd.currency()));
    ui->tableWidget->setItem(newRow, PROCESSED,
                             new QTableWidgetItem("No"));  // Por defecto no procesada
}

// Los demás métodos permanecen igual...
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

    QMessageBox::information(this, "Editar", "Editando fila: " + QString::number(currentRow));
*/
}

void MainWindow::onDeleteRow()
{
    /*
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Eliminar",
                                  "¿Estás seguro de eliminar esta transacción?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        ui->tableWidget->removeRow(currentRow);

        // Aquí también podrías llamar a la interfaz para eliminar del backend
        // transaccionManager->deleteTransaccion(currentRow);
    }
*/
}

void MainWindow::onMarkProcessed()
{
    /*
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    QTableWidgetItem *processedItem = ui->tableWidget->item(currentRow, PROCESSED);
    if (processedItem) {
        processedItem->setText("Yes");

        // Aquí podrías llamar a la interfaz para marcar como procesado en el backend
        // transaccionManager->markAsProcessed(currentRow);
    }
    */



}

void MainWindow::onRowSelected()
{
    // en este punto quiero comunicarme con el gestor para que
    /*
     tengo un problema, quiero seleccionar la fila , esa fila debe tener un id asignado
    ese id asignado se le asigna cada vez que se crea una nueva fila.

    si selecciono una fila, quiero tener acceso a ese id y hacer una operacion con el*/


}
