#include "transaccionbasicadialog.h"
#include "ui_transaccionbasicadialog.h"
#include "categorytreewidgetdialog.h"
#include <QDebug>

TransaccionBasicaDialog::TransaccionBasicaDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TransaccionBasicaDialog)
    , _selectedCategoryId(0)
    , _selectedTipo("gasto")
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->amountDubleSpinBox->setValue(0.0);

    _selectedCategoryName = "";
    ui->categoryLineEdit->setText(_selectedCategoryName);

    ui->tipoCombo->addItem("Gasto", "gasto");
    ui->tipoCombo->addItem("Ingreso", "ingreso");

    ui->tipoCombo->setCurrentIndex(0);
}

TransaccionBasicaDialog::~TransaccionBasicaDialog()
{
    delete ui;
}

T_Structure TransaccionBasicaDialog::getNewTransaction()
{
    T_Structure result;
    result.id = -1;

    _selectedTipo = ui->tipoCombo->currentData().toString().toStdString();

    double amount = ui->amountDubleSpinBox->value();
    if (_selectedTipo == "gasto") {
        amount = -std::abs(amount);
    } else {
        amount = +std::abs(amount);
    }

    result.values[t_AMOUNT] = std::to_string(amount);

    result.values[t_DATE] = ui->dateEdit->date().toString("yyyy-MM-dd").toStdString();
    result.values[t_CONCEPT] = ui->conceptLineEdit->text().toStdString();

    QString selectedCurrency = ui->currencyComboBox->currentText();
    if (selectedCurrency.contains(" - ")) {
        selectedCurrency = selectedCurrency.split(" - ").first();
    }

    result.values[t_CURRENCY] = selectedCurrency.toStdString();

    result.processed = false;
    result.category_id = _selectedCategoryId;
    result.tipo = _selectedTipo;

    qDebug() << "Nueva transacción creada:";
    qDebug() << "  Tipo:" << QString::fromStdString(_selectedTipo);
    qDebug() << "  Monto:" << amount;
    qDebug() << "  Divisa:" << selectedCurrency;

    return result;
}

void TransaccionBasicaDialog::setListCurrencies(const std::vector<std::string> &list)
{
    // Limpiar el combobox primero
    ui->currencyComboBox->clear();

    // Verificar si la lista no está vacía
    if (list.empty()) {
        qWarning() << "Lista de divisas vacía proporcionada a setListCurrencies";

        // Añadir valores por defecto si la lista está vacía
        ui->currencyComboBox->addItem("EUR");
        ui->currencyComboBox->addItem("USD");
        ui->currencyComboBox->addItem("COP");
        return;
    }

    // Añadir todas las divisas de la lista
    for (const std::string &currencyStr : list) {
        QString currency = QString::fromStdString(currencyStr);

        // Puedes formatear el texto si es necesario
        // Por ejemplo: si recibes "EUR", mostrar "EUR - Euro"
        QString displayText = currency;

        // Opcional: Si quieres formatear automáticamente
        if (currency.length() == 3) {
            // Es probablemente un código de divisa ISO
            QString currencyName;
            if (currency == "EUR") currencyName = "Euro";
            else if (currency == "USD") currencyName = "Dólar estadounidense";
            else if (currency == "GBP") currencyName = "Libra esterlina";
            else if (currency == "JPY") currencyName = "Yen japonés";
            else if (currency == "MXN") currencyName = "Peso mexicano";
            else if (currency == "COP") currencyName = "Peso colombiano";
            else currencyName = currency;

            displayText = QString("%1 - %2").arg(currency, currencyName);
        }

        // Añadir al combobox con el código como dato asociado
        ui->currencyComboBox->addItem(displayText, currency);
    }

    // Establecer la primera divisa como seleccionada por defecto
    if (ui->currencyComboBox->count() > 0) {
        ui->currencyComboBox->setCurrentIndex(0);
    }

    qDebug() << "Cargadas" << list.size() << "divisas en el combobox";
}

void TransaccionBasicaDialog::setListCategories(const std::vector<Category_Structure> &list)
{
    _categories = list;

    if (list.empty()) {
        qWarning() << "Lista de categorías vacía proporcionada a setListCategories";
        return;
    }

    qDebug() << "Cargadas" << list.size() << "categorías para el tree dialog";
}

int TransaccionBasicaDialog::getSelectedCategoryId() const
{
    return _selectedCategoryId;
}

void TransaccionBasicaDialog::on_buttonBox_accepted()
{
    // Validación básica antes de aceptar
    if (ui->conceptLineEdit->text().trimmed().isEmpty()) {
        qWarning() << "Concepto vacío, no se puede aceptar";
        // Podrías mostrar un mensaje al usuario aquí
        return;
    }

    if (ui->amountDubleSpinBox->value() == 0.0) {
        qWarning() << "Monto cero, no se puede aceptar";
        // Podrías mostrar un mensaje al usuario aquí
        return;
    }

    accept();
}

void TransaccionBasicaDialog::on_buttonBox_rejected()
{
    reject();
}

void TransaccionBasicaDialog::on_categorySelectButton_clicked()
{
    std::string tipo = ui->tipoCombo->currentData().toString().toStdString();
    categoryTreeWidgetDialog dialog(this, _categories, tipo);
    if (dialog.exec() == QDialog::Accepted) {
        _selectedCategoryId = dialog.getSelectedCategoryId();
        _selectedCategoryName = dialog.getSelectedCategoryName();
        ui->categoryLineEdit->setText(_selectedCategoryName);
        qDebug() << "Categoría seleccionada:" << _selectedCategoryName << "ID:" << _selectedCategoryId;
    }
}
