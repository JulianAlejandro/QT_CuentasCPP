#include "addderivativetransactionsdialog.h"
#include "ui_addderivativetransactionsdialog.h"
//#include "common/DataTypes.h"
//#include "frontend/tableutils.h"
#include <QMessageBox>
#include <QDate>

#include <QStyledItemDelegate>


//#include "spinboxdelegate.h"
#include "doublespinboxdelegate.h"
#include "dateeditdelegate.h"
#include <QStandardItemModel>

#include "commonDataTypes.h"
//#include "sliderdelegate.h"
//#include "fontcomboboxdelegate.h"

//void setupTableWidget(QTableWidget* tableWidget, const QStringList& columnTitles, bool edit);
#include <QMenu>


#include "../categorytreewidgetdialog.h"

addDerivativeTransactionsDialog::addDerivativeTransactionsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addDerivativeTransactionsDialog)
{
    ui->setupUi(this);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView, &QTableView::customContextMenuRequested,
            this, &addDerivativeTransactionsDialog::onCustomContextMenuRequested);


    m_modelo = new QStandardItemModel(this);
    //m_spinnerDelegate_id = new SpinBoxDelegate(this);
    m_DoubleSpinnerDelegate = new DoubleSpinBoxDelegate(this);
    //m_spinnerDelegate_id_TB = new SpinBoxDelegate(this);
    //m_spinnerDelegate_category_id = new SpinBoxDelegate(this);
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
        ui->tableView->setItemDelegateForColumn(0, m_DateEditDelegate);
        ui->tableView->setItemDelegateForColumn(2, m_DoubleSpinnerDelegate);
        //ui->tableView->setItemDelegateForColumn(1, m_DoubleSpinnerDelegate);
        //ui->tableView->setItemDelegateForColumn(3, m_DateEditDelegate);
        //ui->tableView->setItemDelegateForColumn(4, m_spinnerDelegate_id_TB);
        //ui->tableView->setItemDelegateForColumn(6, m_spinnerDelegate_category_id);

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

void addDerivativeTransactionsDialog::loadTransactionsTableWidget(
    const std::vector<DT_Structure>& transacciones,
    int IdRole)
{
    // Limpiar el modelo actual
    m_modelo->removeRows(0, m_modelo->rowCount());

    // Establecer el número de filas según las transacciones
    m_modelo->setRowCount(static_cast<int>(transacciones.size()));

    // Llenar el modelo con los datos del vector de estructuras
    for (size_t row = 0; row < transacciones.size(); ++row) {

        const auto& fila = transacciones[row].values;  // <-- ahora usamos el array interno

        for (size_t col = 0;
             col < fila.size() && col < static_cast<size_t>(m_modelo->columnCount());
             ++col)
        {
            QString valor = QString::fromStdString(fila[col]);

            QModelIndex idx = m_modelo->index(static_cast<int>(row),
                                              static_cast<int>(col));

            m_modelo->setData(idx, valor);
        }

        // Si deseas guardar el ID en un rol, por ejemplo:
        // (Opcional)
        m_modelo->setData(
            m_modelo->index(static_cast<int>(row), 0),
            transacciones[row].id,
            IdRole
            );
    }

    // Opcional: ajustar columnas
    // ui->tableView->resizeColumnsToContents();
}



void addDerivativeTransactionsDialog::on_addPushButton_clicked()
{
    int newRow = m_modelo->rowCount();           // Número de la nueva fila
    m_modelo->insertRow(newRow);                 // Añadir fila vacía

    // Opcional: inicializar todas las columnas con cadenas vacías
    for (int c = 0; c < m_modelo->columnCount(); ++c) {
        m_modelo->setData(m_modelo->index(newRow, c), "");
    }

    // Opcional: mover selección a la nueva fila
    ui->tableView->selectRow(newRow);
}




void addDerivativeTransactionsDialog::onCellDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int fila = index.row();
    int columna = index.column();

    if (columna == 3) {
        // Llamar directamente al procedimiento para la cuarta columna
        ejecutarProcedimientoEspecial(fila);
        return;
    }

    // Para las demás columnas, mantener el comportamiento original
    m_modelo->setData(m_modelo->index(fila, columna), "", Qt::DisplayRole);
}

void addDerivativeTransactionsDialog::ejecutarProcedimientoEspecial(int fila)
{
    // Verificar que tenemos categorías disponibles
    if (cat_struct.empty()) {
        QMessageBox::warning(this, "Advertencia",
                             "No hay categorías disponibles para seleccionar.");
        return;
    }

    // Crear y mostrar el diálogo de categorías
    categoryTreeWidgetDialog cd(this, cat_struct);
    cd.setWindowTitle("Seleccionar Categoría");

    int res = cd.exec();

    if (res == QDialog::Accepted) {
        // Obtener la categoría seleccionada
        QString nombreCategoria = cd.getSelectedCategoryName();
        int idCategoria = cd.getSelectedCategoryId();

        if (!nombreCategoria.isEmpty()) {
            // Añadir el nombre de la categoría a la columna 3 (cuarta columna)
            QModelIndex index = m_modelo->index(fila, 3); // Columna 3

            // Establecer el nombre de la categoría como valor
            m_modelo->setData(index, nombreCategoria, Qt::EditRole);

            // Opcional: también guardar el ID en una propiedad de la celda
            // Puedes usar Qt::UserRole + algún número
            m_modelo->setData(index, idCategoria, Qt::UserRole + 1);

            // Opcional: si quieres mostrar un mensaje de confirmación
            // qDebug() << "Categoría seleccionada: " << nombreCategoria
            //          << " (ID: " << idCategoria << ")";
        } else {
            QMessageBox::information(this, "Información",
                                     "No se seleccionó ninguna categoría.");
        }
    } else {
        // Usuario canceló o cerró el diálogo sin seleccionar
        // qDebug() << "Selección de categoría cancelada";
    }
}

std::vector<DT_Structure> addDerivativeTransactionsDialog::getDerivativeTransactionsModifications(const int IdRole)
{
    std::vector<DT_Structure> result;

    if (!m_modelo) return result;

    for (int fila = 0; fila < m_modelo->rowCount(); ++fila) {
        DT_Structure filaDatos;

        // Obtener el ID almacenado en el rol personalizado (IdRole)
        QVariant idVariant = m_modelo->data(m_modelo->index(fila, 0), IdRole);

        if (idVariant.isValid()) {
            // Si hay un ID válido en el rol, usarlo
            filaDatos.id = idVariant.toInt();
        } else {
            // Si no hay ID en el rol, intentar obtenerlo del contenido de la celda
            QVariant cellData = m_modelo->data(m_modelo->index(fila, 0), Qt::EditRole);
            bool ok;
            int idFromCell = cellData.toInt(&ok);
            filaDatos.id = ok ? idFromCell : -1; // -1 o 0 para indicar que no tiene ID
        }

        // Obtener los valores de las demás columnas
        for (int columna = 0; columna < m_modelo->columnCount(); ++columna) {
            QVariant dato = m_modelo->data(m_modelo->index(fila, columna), Qt::EditRole);
            QString valor = dato.isValid() ? dato.toString() : "";
            filaDatos.values[columna] = valor.toStdString();
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
    reject();
}


void addDerivativeTransactionsDialog::onCustomContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);
    if (!index.isValid()) return;

    // Seleccionar la fila completa
    ui->tableView->selectRow(index.row());

    // Crear menú
    QMenu menu(this);
    QAction *deleteAction = menu.addAction("Borrar transacción");

    // Conectar acción
    connect(deleteAction, &QAction::triggered, this, &addDerivativeTransactionsDialog::borrarFilaSeleccionada);

    // Mostrar menú en la posición del cursor
    menu.exec(ui->tableView->viewport()->mapToGlobal(pos));
}


void addDerivativeTransactionsDialog::borrarFilaSeleccionada()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) return;

    int fila = index.row();
    m_modelo->removeRow(fila);
}


void addDerivativeTransactionsDialog::setCategoryStructures(std::vector<Category_Structure> c){

    cat_struct = c;
}
