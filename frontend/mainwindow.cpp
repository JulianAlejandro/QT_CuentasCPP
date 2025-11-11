#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "transaccionbasicadialog.h"
#include <QMessageBox>


namespace {

    void inicializarListaSimulacion(QTableWidget* tableWidget) {
        //prueba para añadir una transaccion bruta simple
        QStringList titulos;
        tableWidget->setColumnCount(4);
        titulos << "Amount"  << "Comment"  << "Date" << "category";
        tableWidget->setHorizontalHeaderLabels(titulos);

        // Configurar selección por filas completas
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // o ExtendedSelection para múltiples

        int newRow = tableWidget->rowCount();
        tableWidget->insertRow(newRow);

        // Agregar los datos a las celdas
        tableWidget->setItem(newRow, 0, new QTableWidgetItem("QString::number(-3)"));
        tableWidget->setItem(newRow, 1, new QTableWidgetItem("pd.comment()"));
        tableWidget->setItem(newRow, 2, new QTableWidgetItem("pd.date()"));
        tableWidget->setItem(newRow, 3, new QTableWidgetItem("pd.currency()"));
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Trasnsacciones brutas");

    QStringList titulos;
    ui->tableWidget->setColumnCount(4);
    titulos << "Amount"  << "Comment"  << "Date" << "Currency";
    ui->tableWidget->setHorizontalHeaderLabels(titulos);

    // Configurar selección por filas completas
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // Habilitar el menú contextual
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Conectar la señal del menú contextual
    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested,
            this, &MainWindow::onCustomContextMenuRequested);

    // Conectar la señal de selección de fila
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onRowSelected);
}

MainWindow::~MainWindow()
{
    delete ui;
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

    // Agregar los datos a las celdas
    ui->tableWidget->setItem(newRow, AMOUNT, new QTableWidgetItem(QString::number(pd.amount())));
    ui->tableWidget->setItem(newRow, COMMENT, new QTableWidgetItem(pd.comment()));
    ui->tableWidget->setItem(newRow, DATE, new QTableWidgetItem(pd.date()));
    ui->tableWidget->setItem(newRow, CURRENCY, new QTableWidgetItem(pd.currency()));

    //applyRowColor(newRow, pd.processed());
}

void MainWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    QTableWidgetItem *item = ui->tableWidget->itemAt(pos);
    if (!item) return; // No mostrar menú si no hay item en la posición

    QMenu *contextMenu = new QMenu(this);

    QAction *editAction = contextMenu->addAction("Editar");
    QAction *deleteAction = contextMenu->addAction("Eliminar");
    QAction *markProcessedAction = contextMenu->addAction("Marcar como procesado");

    // Conectar las acciones a sus slots
    connect(editAction, &QAction::triggered, this, &MainWindow::onEditRow);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::onDeleteRow);
    connect(markProcessedAction, &QAction::triggered, this, &MainWindow::onMarkProcessed);

    contextMenu->exec(ui->tableWidget->viewport()->mapToGlobal(pos));
    delete contextMenu;
}

void MainWindow::onEditRow()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    // Aquí implementas la edición de la fila seleccionada
    QMessageBox::information(this, "Editar", "Editando fila: " + QString::number(currentRow));
}

void MainWindow::onDeleteRow()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Eliminar",
                                  "¿Estás seguro de eliminar esta transacción?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        ui->tableWidget->removeRow(currentRow);
    }
}

void MainWindow::onMarkProcessed()
{
    /*int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    QTableWidgetItem *processedItem = ui->tableWidget->item(currentRow, PROCESSED);
    if (processedItem) {
        processedItem->setText("Yes");
    }*/
}

void MainWindow::onRowSelected()
{
    //aquí se ejecuta la accion

    /*
    // Opcional: obtener la fila seleccionada
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow >= 0) {
        // Puedes acceder a los datos de la fila si lo necesitas
        // QTableWidgetItem *item = ui->tableWidget->item(currentRow, 0);
    }
*/
    //inicializarListaSimulacion(ui->tableWidget_2);
}


