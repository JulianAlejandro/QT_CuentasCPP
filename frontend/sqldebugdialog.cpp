#include "sqldebugdialog.h"
#include "ui_sqldebugdialog.h"
#include <QTableWidget>

SQLDebugDialog::SQLDebugDialog(SQLManager* sqlManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SQLDebugDialog)
    , _sqlManager(sqlManager)
{
    ui->setupUi(this);
    cargarTablas();
}

SQLDebugDialog::~SQLDebugDialog()
{
    delete ui;
}

void SQLDebugDialog::on_refreshButton_clicked()
{
    cargarTablas();
}

void SQLDebugDialog::cargarTablas()
{
    while (ui->tabWidget->count() > 0) {
        QWidget* w = ui->tabWidget->widget(0);
        ui->tabWidget->removeTab(0);
        delete w;
    }

    for (const TablaSQL& tabla : _sqlManager->obtenerTodasLasTablas()) {
        QTableWidget* table = new QTableWidget(ui->tabWidget);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setColumnCount(tabla.columnas.size());
        table->setHorizontalHeaderLabels(tabla.columnas);
        table->setRowCount(static_cast<int>(tabla.filas.size()));

        for (int r = 0; r < static_cast<int>(tabla.filas.size()); ++r) {
            const QStringList& fila = tabla.filas[r];
            for (int c = 0; c < fila.size(); ++c)
                table->setItem(r, c, new QTableWidgetItem(fila[c]));
        }

        table->resizeColumnsToContents();
        ui->tabWidget->addTab(table, tabla.nombre);
    }
}
