#include "transaccionbasicadialog.h"
#include "ui_transaccionbasicadialog.h"
#include <QDebug>

TransaccionBasicaDialog::TransaccionBasicaDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TransaccionBasicaDialog)
{
    ui->setupUi(this);

    // Configurar valores por defecto
    ui->dateEdit->setDate(QDate::currentDate());
    ui->amountDubleSpinBox->setValue(0.0);

    // Configurar formato de fecha si lo deseas
    // ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
}

TransaccionBasicaDialog::~TransaccionBasicaDialog()
{
    delete ui;
}

T_Structure TransaccionBasicaDialog::getNewTransaction()
{
    T_Structure result;
    result.id = -1;

    // Para el amount
    result.values[t_AMOUNT] = ui->amountDubleSpinBox->text().toStdString();

    // Fecha en formato ISO: "yyyy-MM-dd"
    result.values[t_DATE] = ui->dateEdit->date().toString("yyyy-MM-dd").toStdString();

    // Concepto
    result.values[t_CONCEPT] = ui->conceptLineEdit->text().toStdString();

    // Divisa seleccionada del combobox
    // Obtener el código de divisa (podría ser solo el código o código + nombre)
    QString selectedCurrency = ui->currencyComboBox->currentText();

    // Si el texto tiene formato "EUR - Euro", extraer solo "EUR"
    if (selectedCurrency.contains(" - ")) {
        selectedCurrency = selectedCurrency.split(" - ").first();
    }

    result.values[t_CURRENCY] = selectedCurrency.toStdString();

    result.processed = false;

    // Debug opcional
    qDebug() << "Nueva transacción creada:";
    qDebug() << "  Divisa:" << selectedCurrency;
    qDebug() << "  Monto:" << QString::fromStdString(result.values[t_AMOUNT]);
    qDebug() << "  Fecha:" << QString::fromStdString(result.values[t_DATE]);

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
