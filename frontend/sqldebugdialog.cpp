#include "sqldebugdialog.h"
#include "ui_sqldebugdialog.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QHeaderView>

SQLDebugDialog::SQLDebugDialog(QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::SQLDebugDialog)
{
    _ui->setupUi(this);
    setWindowTitle("SQL Debug - Ver Tablas");

    connect(_ui->refreshPushButton, &QPushButton::clicked, this, &SQLDebugDialog::onRefreshClicked);
    connect(_ui->tableComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SQLDebugDialog::onTableSelected);
    connect(_ui->resetCategoriesPushButton, &QPushButton::clicked, this, &SQLDebugDialog::onResetCategoriesClicked);
    connect(_ui->addTestDataPushButton, &QPushButton::clicked, this, &SQLDebugDialog::onAddTestDataClicked);

    loadTables();
}

SQLDebugDialog::~SQLDebugDialog()
{
    delete _ui;
}

void SQLDebugDialog::loadTables()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isOpen()){
        QMessageBox::warning(this, "Error", "Base de datos no disponible");
        return;
    }

    QSqlQuery q;
    if(!q.exec("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name")){
        QMessageBox::warning(this, "Error", "No se pudieron obtener las tablas: " + q.lastError().text());
        return;
    }

    _ui->tableComboBox->clear();
    while(q.next()){
        _ui->tableComboBox->addItem(q.value(0).toString());
    }

    if(_ui->tableComboBox->count() > 0){
        loadTableContent(_ui->tableComboBox->currentText());
    }
}

void SQLDebugDialog::loadTableContent(const QString &tableName)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q;

    if(!q.exec(QString("SELECT * FROM %1").arg(tableName))){
        QMessageBox::warning(this, "Error", "No se pudo cargar la tabla: " + q.lastError().text());
        return;
    }

    QSqlRecord record = q.record();
    int numColumns = record.count();
    int numRows = 0;

    while(q.next()){
        numRows++;
    }
    q.seek(-1);

    _ui->tableWidget->setColumnCount(numColumns);
    _ui->tableWidget->setRowCount(numRows);

    for(int i = 0; i < numColumns; ++i){
        _ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(record.fieldName(i)));
    }

    int row = 0;
    while(q.next()){
        for(int col = 0; col < numColumns; ++col){
            QTableWidgetItem *item = new QTableWidgetItem(q.value(col).toString());
            _ui->tableWidget->setItem(row, col, item);
        }
        row++;
    }

    _ui->tableWidget->resizeColumnsToContents();
}

void SQLDebugDialog::onRefreshClicked()
{
    loadTables();
}

void SQLDebugDialog::onTableSelected(int index)
{
    if(index >= 0){
        loadTableContent(_ui->tableComboBox->itemText(index));
    }
}

bool SQLDebugDialog::deleteAllData()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isOpen()){
        QMessageBox::warning(this, "Error", "Base de datos no disponible");
        return false;
    }

    QSqlQuery q;
    if(!db.transaction()){
        return false;
    }

    if(!q.exec("DELETE FROM transaccion_neta")){
        db.rollback();
        QMessageBox::warning(this, "Error", "No se pudo eliminar transaccion_neta: " + q.lastError().text());
        return false;
    }

    if(!q.exec("DELETE FROM transaccion_bruta")){
        db.rollback();
        QMessageBox::warning(this, "Error", "No se pudo eliminar transaccion_bruta: " + q.lastError().text());
        return false;
    }

    if(!q.exec("DELETE FROM categoria")){
        db.rollback();
        QMessageBox::warning(this, "Error", "No se pudo eliminar categoria: " + q.lastError().text());
        return false;
    }

    return db.commit();
}

bool SQLDebugDialog::insertCategoriesData()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isOpen()){
        QMessageBox::warning(this, "Error", "Base de datos no disponible");
        return false;
    }

    QSqlQuery q;

    std::vector<std::pair<int, QString>> categorias = {
        {0, "raiz"},
        {1, "Ingreso"},
        {2, "Salario"},
        {3, "Bonus"},
        {4, "Gastos"},
        {5, "Casa"},
        {6, "Comida"},
        {7, "Supermercado"},
        {10, "Freelance"},
        {11, "Inversiones"},
        {12, "Dividendos"},
        {13, "Intereses"},
        {14, "Regalos"},
        {15, "Alquiler"},
        {16, "Hipoteca"},
        {17, "Servicios"},
        {18, "Electricidad"},
        {19, "Agua"},
        {20, "Internet"},
        {21, "Mobiliario"},
        {22, "Restaurantes"},
        {23, "Comida llevar"},
        {24, "Transporte"},
        {25, "Coche"},
        {26, "Gasolina"},
        {27, "Mantenimiento"},
        {28, "Seguro"},
        {29, "Transporte Publico"},
        {30, "Bus"},
        {31, "Metro"},
        {32, "Ocio"},
        {33, "Cine"},
        {34, "Videojuegos"},
        {35, "Deportes"},
        {36, "Salud"},
        {37, "Farmacia"},
        {38, "Medico"},
        {39, "Gimnasio"},
        {40, "Educacion"},
        {41, "Libros"},
        {42, "Cursos"},
        {43, "Ropa"},
        {44, "Calzado"},
        {45, "Accesorios"}
    };

    std::vector<std::pair<int, int>> relaciones = {
        {1, 0}, {2, 1}, {3, 2}, {10, 2}, {11, 1}, {12, 11}, {13, 11}, {14, 1},
        {5, 4}, {15, 5}, {16, 5}, {17, 5}, {18, 17}, {19, 17}, {20, 17}, {21, 5},
        {6, 4}, {7, 6}, {22, 6}, {23, 6},
        {24, 4}, {25, 24}, {26, 25}, {27, 25}, {28, 25}, {29, 24}, {30, 29}, {31, 29},
        {32, 4}, {33, 32}, {34, 32}, {35, 32},
        {36, 4}, {37, 36}, {38, 36}, {39, 36},
        {40, 4}, {41, 40}, {42, 40},
        {43, 4}, {44, 43}, {45, 43}
    };

    if(!db.transaction()){
        return false;
    }

    q.prepare("INSERT INTO categoria (id, nombre, id_padre, tipo) VALUES (?, ?, ?, ?)");

    std::map<int, QString> tiposCategorias = {
        {0, "ingreso"},
        {1, "ingreso"},
        {2, "ingreso"},
        {3, "ingreso"},
        {10, "ingreso"},
        {11, "ingreso"},
        {12, "ingreso"},
        {13, "ingreso"},
        {14, "ingreso"},
        {4, "gasto"},
        {5, "gasto"},
        {15, "gasto"},
        {16, "gasto"},
        {17, "gasto"},
        {18, "gasto"},
        {19, "gasto"},
        {20, "gasto"},
        {21, "gasto"},
        {6, "gasto"},
        {7, "gasto"},
        {22, "gasto"},
        {23, "gasto"},
        {24, "gasto"},
        {25, "gasto"},
        {26, "gasto"},
        {27, "gasto"},
        {28, "gasto"},
        {29, "gasto"},
        {30, "gasto"},
        {31, "gasto"},
        {32, "gasto"},
        {33, "gasto"},
        {34, "gasto"},
        {35, "gasto"},
        {36, "gasto"},
        {37, "gasto"},
        {38, "gasto"},
        {39, "gasto"},
        {40, "gasto"},
        {41, "gasto"},
        {42, "gasto"},
        {43, "gasto"},
        {44, "gasto"},
        {45, "gasto"}
    };

    for(const auto& cat : categorias){
        int id = cat.first;
        QString nombre = cat.second;
        int idPadre = -1;

        for(const auto& rel : relaciones){
            if(rel.first == id){
                idPadre = rel.second;
                break;
            }
        }

        q.bindValue(0, id);
        q.bindValue(1, nombre);
        if(idPadre == 0){
            q.bindValue(2, QVariant());
        } else if(idPadre > 0){
            q.bindValue(2, idPadre);
        } else {
            q.bindValue(2, QVariant());
        }
        q.bindValue(3, tiposCategorias[id]);

        if(!q.exec()){
            db.rollback();
            QMessageBox::warning(this, "Error", "No se pudo insertar categoria: " + q.lastError().text());
            return false;
        }
    }

    return db.commit();
}

void SQLDebugDialog::resetAndInsertCategories()
{
    if(deleteAllData()){
        if(insertCategoriesData()){
            QMessageBox::information(this, "Exito", "Categorias reseteadas correctamente.");
            loadTables();
        }
    }
}

void SQLDebugDialog::onResetCategoriesClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmar",
                                  "Se eliminaran todas las categorias y transacciones. Continuar?",
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        resetAndInsertCategories();
    }
}

bool SQLDebugDialog::insertTransactionsData()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isOpen()){
        return false;
    }

    QSqlQuery q;

    if(!db.transaction()){
        return false;
    }

    q.prepare("INSERT INTO transaccion_bruta (amount, comment, date, currency, processed, tipo) VALUES (?, ?, ?, ?, ?, ?)");

    QVariantList amounts = {2500.00, 300.00, 120.50, 45.00, 85.00, 1500.00};
    QStringList comments = {"Salario Enero", "Bonus proyecto", "Compra supermercado", "Cine y cena", "Gasolina", "Salario Febrero"};
    QStringList dates = {"2026-01-31", "2026-01-28", "2026-01-25", "2026-01-20", "2026-01-15", "2026-02-28"};
    QStringList tiposTB = {"ingreso", "ingreso", "gasto", "gasto", "gasto", "ingreso"};

    for(int i = 0; i < amounts.size(); ++i){
        q.bindValue(0, amounts[i]);
        q.bindValue(1, comments[i]);
        q.bindValue(2, dates[i]);
        q.bindValue(3, "EUR");
        q.bindValue(4, false);
        q.bindValue(5, tiposTB[i]);
        if(!q.exec()){
            db.rollback();
            return false;
        }
    }

    q.prepare("INSERT INTO transaccion_neta (amount, comment, date, id_TB, category_id, tipo) VALUES (?, ?, ?, ?, ?, ?)");

    QVariantList tn_amounts = {2000.00, 500.00, 300.00, 80.00, 40.50, 25.00, 20.00, 85.00, 1500.00, 200.00, 150.00, 60.00};
    QStringList tn_comments = {"Salario", "Freelance", "Bonus", "Supermercado", "Restaurantes", "Cine", "Comida llevar", "Gasolina", "Salario", "Dividendos", "Servicios", "Ropa"};
    QStringList tn_dates = {"2026-01-31", "2026-01-31", "2026-01-28", "2026-01-25", "2026-01-25", "2026-01-20", "2026-01-20", "2026-01-15", "2026-02-28", "2026-01-30", "2026-01-26", "2026-01-18"};
    QVariantList tn_idTB = {1, 1, 2, 3, 3, 4, 4, 5, 6, 9, 9, 9};
    QVariantList tn_catId = {2, 10, 3, 7, 22, 33, 23, 26, 2, 12, 17, 43};
    QStringList tiposTN = {"ingreso", "ingreso", "ingreso", "gasto", "gasto", "gasto", "gasto", "gasto", "ingreso", "ingreso", "gasto", "gasto"};

    for(int i = 0; i < tn_amounts.size(); ++i){
        q.bindValue(0, tn_amounts[i]);
        q.bindValue(1, tn_comments[i]);
        q.bindValue(2, tn_dates[i]);
        q.bindValue(3, tn_idTB[i]);
        q.bindValue(4, tn_catId[i]);
        q.bindValue(5, tiposTN[i]);
        if(!q.exec()){
            db.rollback();
            return false;
        }
    }

    return db.commit();
}

void SQLDebugDialog::resetAndInsertTestTransactions()
{
    if(deleteAllData()){
        if(insertCategoriesData()){
            if(insertTransactionsData()){
                QMessageBox::information(this, "Exito", "Datos de prueba insertados correctamente.");
                loadTables();
            } else {
                QMessageBox::warning(this, "Error", "No se pudieron insertar las transacciones de prueba.");
            }
        } else {
            QMessageBox::warning(this, "Error", "No se pudieron insertar las categorias.");
        }
    }
}

void SQLDebugDialog::onAddTestDataClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmar",
                                  "Se eliminaran todas las transacciones actuales. Continuar?",
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        resetAndInsertTestTransactions();
    }
}
