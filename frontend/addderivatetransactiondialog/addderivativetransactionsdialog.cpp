#include "addderivativetransactionsdialog.h"
#include "ui_addderivativetransactionsdialog.h"
#include <QMessageBox>
#include <QDate>
#include <QStyledItemDelegate>
#include "doublespinboxdelegate.h"
#include "dateeditdelegate.h"
#include <QStandardItemModel>
#include "commonDataTypes.h"
#include <QMenu>
#include "../categorytreewidgetdialog.h"
#include <QDebug>
#include <QDialogButtonBox>

addDerivativeTransactionsDialog::addDerivativeTransactionsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addDerivativeTransactionsDialog)
    , p_amount(0.0)
{
    ui->setupUi(this);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView, &QTableView::customContextMenuRequested,
            this, &addDerivativeTransactionsDialog::onCustomContextMenuRequested);

    m_modelo = new QStandardItemModel(this);
    m_DoubleSpinnerDelegate = new DoubleSpinBoxDelegate(this);
    m_DateEditDelegate = new DateEditDelegate(this);

    // Inicializar con 0 filas en lugar de 7
    m_modelo->setRowCount(0);

    // Conectar señal de datos cambiados para validación en tiempo real
    connect(m_modelo, &QStandardItemModel::dataChanged,
            this, &addDerivativeTransactionsDialog::onDataChanged);

    // Conectar doble click
    connect(ui->tableView, &QTableView::doubleClicked,
            this, &addDerivativeTransactionsDialog::onCellDoubleClicked);
}

addDerivativeTransactionsDialog::~addDerivativeTransactionsDialog()
{
    delete ui;
}

void addDerivativeTransactionsDialog::setFieldsTableWidget(const QStringList& columnTitles, bool edit)
{
    m_modelo->setColumnCount(columnTitles.size());
    m_modelo->setHorizontalHeaderLabels(columnTitles);
    ui->tableView->setModel(m_modelo);

    if (edit) {
        ui->tableView->setItemDelegateForColumn(0, m_DateEditDelegate);
        ui->tableView->setItemDelegateForColumn(2, m_DoubleSpinnerDelegate);

        ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked |
                                       QAbstractItemView::EditKeyPressed);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    } else {
        ui->tableView->setItemDelegate(nullptr);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setFocusPolicy(Qt::NoFocus);
    }
}

void addDerivativeTransactionsDialog::loadTransactionsTableWidget(
    const std::vector<DT_Structure>& transacciones,
    int IdRole)
{
    m_modelo->removeRows(0, m_modelo->rowCount());
    m_modelo->setRowCount(static_cast<int>(transacciones.size()));

    for (size_t row = 0; row < transacciones.size(); ++row) {
        const auto& fila = transacciones[row].values;

        for (size_t col = 0; col < fila.size() && col < static_cast<size_t>(m_modelo->columnCount()); ++col) {
            QString valor = QString::fromStdString(fila[col]);
            QModelIndex idx = m_modelo->index(static_cast<int>(row), static_cast<int>(col));
            m_modelo->setData(idx, valor);
        }

        m_modelo->setData(
            m_modelo->index(static_cast<int>(row), 0),
            transacciones[row].id,
            IdRole
            );
    }

    // Actualizar validación después de cargar datos
    updateSumValidation();
}

void addDerivativeTransactionsDialog::on_addPushButton_clicked()
{
    int newRow = m_modelo->rowCount();
    m_modelo->insertRow(newRow);

    // Inicializar todas las columnas con cadenas vacías
    for (int c = 0; c < m_modelo->columnCount(); ++c) {
        m_modelo->setData(m_modelo->index(newRow, c), "");
    }

    // Para la columna de fecha, establecer fecha actual por defecto
    QModelIndex dateIndex = m_modelo->index(newRow, 0);
    m_modelo->setData(dateIndex, QDate::currentDate().toString("yyyy-MM-dd"));

    ui->tableView->selectRow(newRow);

    // Actualizar validación
    updateSumValidation();
}

void addDerivativeTransactionsDialog::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)
    updateSumValidation();
}

bool addDerivativeTransactionsDialog::validateAmountSum() const
{
    if (p_amount <= 0.0) {
        qDebug() << "Monto padre no establecido o inválido:" << p_amount;
        return false;
    }

    double sum = 0.0;
    const double EPSILON = 0.001; // Margen de error para números de punto flotante

    for (int row = 0; row < m_modelo->rowCount(); ++row) {
        QModelIndex index = m_modelo->index(row, 2); // Columna 2 = amount (dt_AMOUNT)
        QVariant data = m_modelo->data(index, Qt::EditRole);

        if (!data.isValid() || data.toString().trimmed().isEmpty()) {
            // Celda vacía, consideramos como 0.0
            continue;
        }

        bool ok = false;
        double amount = data.toDouble(&ok);

        if (!ok) {
            qDebug() << "Valor no numérico en fila" << row << ":" << data.toString();
            return false;
        }

        sum += amount;
    }

    bool isValid = std::abs(sum - p_amount) <= EPSILON;

    qDebug() << "Suma validación:" << sum << ", Monto padre:" << p_amount
             << ", Diferencia:" << (sum - p_amount) << ", Válido:" << isValid;

    return isValid;
}

void addDerivativeTransactionsDialog::updateSumValidation()
{
    bool isValid = validateAmountSum();

    // Habilitar o deshabilitar el botón OK
    QPushButton* okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (okButton) {
        okButton->setEnabled(isValid);
    }
    /*
    // Actualizar etiqueta de estado si existe
    if (ui->labelStatus) {
        if (isValid) {
            ui->labelStatus->setText("✓ Suma correcta");
            ui->labelStatus->setStyleSheet("color: green; font-weight: bold;");
        } else {
            double currentSum = 0.0;
            for (int row = 0; row < m_modelo->rowCount(); ++row) {
                QModelIndex index = m_modelo->index(row, 2);
                QVariant data = m_modelo->data(index, Qt::EditRole);
                if (data.isValid() && !data.toString().isEmpty()) {
                    bool ok;
                    currentSum += data.toDouble(&ok);
                }
            }

            double difference = currentSum - p_amount;
            QString statusText = QString("Suma: %1 | Requerido: %2 | Diferencia: %3")
                                     .arg(currentSum, 0, 'f', 2)
                                     .arg(p_amount, 0, 'f', 2)
                                     .arg(difference, 0, 'f', 2);

            if (difference > 0) {
                statusText += " (demasiado alto)";
            } else if (difference < 0) {
                statusText += " (demasiado bajo)";
            }

            ui->labelStatus->setText(statusText);
            ui->labelStatus->setStyleSheet("color: red; font-weight: bold;");
        }
    }*/
}

void addDerivativeTransactionsDialog::showValidationMessage()
{
    double currentSum = 0.0;
    int validRows = 0;

    for (int row = 0; row < m_modelo->rowCount(); ++row) {
        QModelIndex index = m_modelo->index(row, 2);
        QVariant data = m_modelo->data(index, Qt::EditRole);
        if (data.isValid() && !data.toString().isEmpty()) {
            bool ok;
            double amount = data.toDouble(&ok);
            if (ok) {
                currentSum += amount;
                validRows++;
            }
        }
    }

    QMessageBox::warning(this, "Error de validación",
                         QString("La suma de los montos (%1) no coincide con el monto de la transacción padre (%2).\n\n"
                                 "Transacciones: %3\n"
                                 "Monto total requerido: %4\n"
                                 "Diferencia: %5\n\n"
                                 "Por favor, ajuste los valores antes de continuar.")
                             .arg(currentSum, 0, 'f', 2)
                             .arg(p_amount, 0, 'f', 2)
                             .arg(validRows)
                             .arg(p_amount, 0, 'f', 2)
                             .arg(currentSum - p_amount, 0, 'f', 2));
}

void addDerivativeTransactionsDialog::on_buttonBox_accepted()
{
    if (!validateAmountSum()) {
        showValidationMessage();
        return; // No cerrar el diálogo
    }
    accept();
}

void addDerivativeTransactionsDialog::on_buttonBox_rejected()
{
    reject();
}

// Resto de métodos sin cambios...
void addDerivativeTransactionsDialog::onCellDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int fila = index.row();
    int columna = index.column();

    if (columna == 3) {
        ejecutarProcedimientoEspecial(fila);
        return;
    }

    m_modelo->setData(m_modelo->index(fila, columna), "", Qt::DisplayRole);
}

void addDerivativeTransactionsDialog::ejecutarProcedimientoEspecial(int fila)
{
    if (cat_struct.empty()) {
        QMessageBox::warning(this, "Advertencia",
                             "No hay categorías disponibles para seleccionar.");
        return;
    }

    categoryTreeWidgetDialog cd(this, cat_struct);
    cd.setWindowTitle("Seleccionar Categoría");

    int res = cd.exec();

    if (res == QDialog::Accepted) {
        QString nombreCategoria = cd.getSelectedCategoryName();
        int idCategoria = cd.getSelectedCategoryId();

        if (!nombreCategoria.isEmpty()) {
            QModelIndex index = m_modelo->index(fila, 3);
            m_modelo->setData(index, nombreCategoria, Qt::EditRole);
            m_modelo->setData(index, idCategoria, Qt::UserRole + 1);
        }
    }
}

std::vector<DT_Structure> addDerivativeTransactionsDialog::getDerivativeTransactionsModifications(const int IdRole)
{
    std::vector<DT_Structure> result;

    if (!m_modelo) return result;

    for (int fila = 0; fila < m_modelo->rowCount(); ++fila) {
        DT_Structure filaDatos;

        QVariant idVariant = m_modelo->data(m_modelo->index(fila, 0), IdRole);
        if (idVariant.isValid()) {
            filaDatos.id = idVariant.toInt();
        } else {
            QVariant cellData = m_modelo->data(m_modelo->index(fila, 0), Qt::EditRole);
            bool ok;
            int idFromCell = cellData.toInt(&ok);
            filaDatos.id = ok ? idFromCell : -1;
        }

        for (int columna = 0; columna < m_modelo->columnCount(); ++columna) {
            QVariant dato = m_modelo->data(m_modelo->index(fila, columna), Qt::EditRole);
            QString valor = dato.isValid() ? dato.toString() : "";
            filaDatos.values[columna] = valor.toStdString();
        }

        result.push_back(filaDatos);
    }

    return result;
}

void addDerivativeTransactionsDialog::onCustomContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);
    if (!index.isValid()) return;

    ui->tableView->selectRow(index.row());

    QMenu menu(this);
    QAction *deleteAction = menu.addAction("Borrar transacción");
    connect(deleteAction, &QAction::triggered, this, &addDerivativeTransactionsDialog::borrarFilaSeleccionada);

    menu.exec(ui->tableView->viewport()->mapToGlobal(pos));
}

void addDerivativeTransactionsDialog::borrarFilaSeleccionada()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) return;

    int fila = index.row();
    m_modelo->removeRow(fila);
    updateSumValidation();
}

void addDerivativeTransactionsDialog::setCategoryStructures(std::vector<Category_Structure> c)
{
    cat_struct = c;
}

void addDerivativeTransactionsDialog::setParentAmount(const double parent_amount)
{
    p_amount = parent_amount;
    qDebug() << "Monto padre establecido:" << p_amount;
    updateSumValidation();
}
