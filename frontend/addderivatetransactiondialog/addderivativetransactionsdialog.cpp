#include "addderivativetransactionsdialog.h"
#include "ui_addderivativetransactionsdialog.h"
//#include "common/DataTypes.h"
//#include "frontend/tableutils.h"
#include <QMessageBox>
#include <QDate>
#include <QStyledItemDelegate>


#include "spinboxdelegate.h"
#include "doublespinboxdelegate.h"
#include "dateeditdelegate.h"
#include <QStandardItemModel>
//#include "sliderdelegate.h"
//#include "fontcomboboxdelegate.h"

//void setupTableWidget(QTableWidget* tableWidget, const QStringList& columnTitles, bool edit);


addDerivativeTransactionsDialog::addDerivativeTransactionsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addDerivativeTransactionsDialog)
{
    ui->setupUi(this);

    m_modelo = new QStandardItemModel(this);
    m_spinnerDelegate_id = new SpinBoxDelegate(this);
    m_DoubleSpinnerDelegate = new DoubleSpinBoxDelegate(this);
    m_spinnerDelegate_id_TB = new SpinBoxDelegate(this);
    m_spinnerDelegate_category_id = new SpinBoxDelegate(this);
    m_DateEditDelegate = new DateEditDelegate(this);

    m_modelo->setRowCount(7);

    // CONECTAR LA SEÑAL DE DOBLE CLICK
    connect(ui->tableView, &QTableView::doubleClicked,
            this, &addDerivativeTransactionsDialog::onCellDoubleClicked);
}

/*
addDerivativeTransactionsDialog::addDerivativeTransactionsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addDerivativeTransactionsDialog)
{
    ui->setupUi(this);


    //ui->setupUi(this);
    m_modelo = new QStandardItemModel(this);
    m_spinnerDelegate_id = new SpinBoxDelegate(this);
    m_DoubleSpinnerDelegate = new DoubleSpinBoxDelegate(this);
    m_spinnerDelegate_id_TB = new SpinBoxDelegate(this);
    m_spinnerDelegate_category_id = new SpinBoxDelegate(this);
    m_DateEditDelegate = new DateEditDelegate(this);

   // m_sliderDelegate = new SliderDelegate(this);
   // m_fcbDelegate = new FontComboBoxDelegate(this);

    m_modelo->setRowCount(7);
   // m_modelo->setColumnCount(7);


    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch)   ;opcional pensar si poner.

    // Dentro del constructor o justo después de setFieldsTableWidget(...)
  //  connect(ui->TableWidget, &QTableWidget::itemChanged,
  //          this, &addDerivativeTransactionsDialog::validateCell);

}
*/
addDerivativeTransactionsDialog::~addDerivativeTransactionsDialog()
{
    delete ui;
}

void addDerivativeTransactionsDialog::setFieldsTableWidget(const QStringList& columnTitles, bool edit){
    m_modelo->setColumnCount(columnTitles.size());
    m_modelo->setHorizontalHeaderLabels(columnTitles);

    ui->tableView->setModel(m_modelo);

    if (edit) {
        // MODO EDICIÓN HABILITADO
        ui->tableView->setItemDelegateForColumn(0, m_spinnerDelegate_id);
        ui->tableView->setItemDelegateForColumn(1, m_DoubleSpinnerDelegate);
        ui->tableView->setItemDelegateForColumn(3, m_DateEditDelegate);
        ui->tableView->setItemDelegateForColumn(4, m_spinnerDelegate_id_TB);
        ui->tableView->setItemDelegateForColumn(6, m_spinnerDelegate_category_id);

        // SOLUCIÓN: Usar solo DoubleClicked y EditKeyPressed
        // Esto evita que se concatene al presionar teclas
        ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked |
                                       QAbstractItemView::EditKeyPressed);

        // Permitir selección de items
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

    } else {
        // MODO SOLO LECTURA
        ui->tableView->setItemDelegate(nullptr);

        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setFocusPolicy(Qt::NoFocus);
    }
}

/*
void addDerivativeTransactionsDialog::setFieldsTableWidget(const QStringList& columnTitles, bool edit){

    //TableUtils::setFieldsTableWidget(ui->TableWidget, columnTitles, edit);

    m_modelo->setColumnCount(columnTitles.size());
    m_modelo->setHorizontalHeaderLabels(columnTitles);

    ui->tableView->setModel(m_modelo);
    ui->tableView->setItemDelegateForColumn(0, m_spinnerDelegate_id);
    ui->tableView->setItemDelegateForColumn(1, m_DoubleSpinnerDelegate);
    ui->tableView->setItemDelegateForColumn(3, m_DateEditDelegate);
    ui->tableView->setItemDelegateForColumn(4, m_spinnerDelegate_id_TB);
    ui->tableView->setItemDelegateForColumn(6, m_spinnerDelegate_category_id);

}
*/

void addDerivativeTransactionsDialog::loadTransactionsTableWidget(std::vector<std::vector<std::string>> transacciones, int IdRole)
{
    /*
    // Limpiar el modelo actual
    m_modelo->removeRows(0, m_modelo->rowCount());

    // Establecer el número de filas según las transacciones
    m_modelo->setRowCount(transacciones.size());

    // Llenar el modelo con los datos del vector
    for (size_t row = 0; row < transacciones.size(); ++row) {
        const std::vector<std::string>& fila = transacciones[row];

        for (size_t col = 0; col < fila.size() && col < static_cast<size_t>(m_modelo->columnCount()); ++col) {
            // Convertir std::string to QString
            QString valor = QString::fromStdString(fila[col]);

            // Establecer el valor en el modelo
            m_modelo->setData(m_modelo->index(row, col), valor);
        }
    }

    // Ajustar el tamaño de las columnas al contenido
    //ui->tableView->resizeColumnsToContents();
    */
}


void addDerivativeTransactionsDialog::on_addPushButton_clicked()
{

}



void addDerivativeTransactionsDialog::onCellDoubleClicked(const QModelIndex &index)
{
    // m_modelo->setData(m_modelo->index(0, 0), "", Qt::DisplayRole);

    if (!index.isValid()) return;

    int fila = index.row();
    int columna = index.column();

    m_modelo->setData(m_modelo->index(fila, columna), "", Qt::DisplayRole); // ocultar info caundo se le da 2 click para editar

    /* ESTO ES LO NUEVO

    if (!index.isValid()) return;

    int columna = index.column();

    // Si es la primera columna, no permitir edición
    if (columna == 0) {
        return; // Salir sin hacer nada
    }

    int fila = index.row();
    m_modelo->setData(m_modelo->index(fila, columna), "", Qt::DisplayRole);
    */

    /*
    // Obtener el valor de la celda
    QVariant dato = m_modelo->data(index, Qt::DisplayRole);
    QString valor = dato.toString();

    // Obtener el nombre de la columna
    QString nombreColumna = m_modelo->headerData(columna, Qt::Horizontal).toString();

    // Mostrar información (puedes cambiar esto por tu lógica)
    QMessageBox::information(this,
                             "Celda seleccionada",
                             QString("Doble click en:\n"
                                     "Fila: %1\n"
                                     "Columna: %2 (%3)\n"
                                     "Valor: %4")
                                 .arg(fila + 1)
                                 .arg(columna)
                                 .arg(nombreColumna)
                                 .arg(valor));

    // Aquí puedes agregar tu lógica específica
    ejecutarAccionPersonalizada(fila, columna, valor);
*/
}

std::vector<std::vector<std::string>> addDerivativeTransactionsDialog::getNewDerivativeTransactions()
{
    std::vector<std::vector<std::string>> result;

    if (!m_modelo) return result;

    for (int fila = 0; fila < m_modelo->rowCount(); ++fila) {
        std::vector<std::string> filaDatos;

        for (int columna = 0; columna < m_modelo->columnCount(); ++columna) {
            QVariant dato = m_modelo->data(m_modelo->index(fila, columna), Qt::EditRole);
            QString valor = dato.isValid() ? dato.toString() : "";
            filaDatos.push_back(valor.toStdString());
        }

        result.push_back(filaDatos);
    }

    return result;
}

void addDerivativeTransactionsDialog::on_buttonBox_accepted()
{
    accept();
}


void addDerivativeTransactionsDialog::on_buttonBox_rejected()
{
    rejected();
}
