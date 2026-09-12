#include "sqlmanager.h"
#include <qDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QCoreApplication>
#include <vector>
#include <QString>

namespace {

class BDConeccion {
public:
    explicit BDConeccion(QSqlDatabase& bd) : _bd(bd) {
        if(!_bd.isOpen()) {
            if(!_bd.open()) {
                qDebug() << "Error abriendo BD:" << _bd.lastError().text();
            }
        }
    }
    ~BDConeccion() {
        if(_bd.isOpen()) _bd.close();
    }
    bool isOpen() const { return _bd.isOpen(); }
private:
    QSqlDatabase& _bd;
};

estructuraTB extraerTransaccionBruta(QSqlQuery& q) {
    estructuraTB tb;
    tb.id = q.value("id").toInt();
    tb.amount = q.value("amount").toDouble();
    tb.comment = q.value("comment").toString().toStdString();
    tb.date = q.value("date").toString().toStdString();
    tb.currency = q.value("currency").toString().toStdString();
    tb.processed = q.value("processed").toBool();
    return tb;
}

estructuraTN extraerTransaccionNeta(QSqlQuery& q) {
    estructuraTN tn;
    tn.id = q.value("id").toInt();
    tn.amount = q.value("amount").toDouble();
    tn.comment = q.value("comment").toString().toStdString();
    tn.date = q.value("date").toString().toStdString();
    tn.id_TB = q.value("id_TB").toInt();
    tn.category_id = q.value("category_id").toInt();
    tn.category_name = q.value("category_name").toString().toStdString();
    return tn;
}

estructuraCategoria extraerCategoria(QSqlQuery& q) {
    estructuraCategoria cat;
    cat.id = q.value("id").toInt();
    cat.nombre = q.value("nombre").toString().toStdString();
    cat.id_padre = q.value("id_padre").toInt();
    return cat;
}

estructuraDivisa extraerDivisa(QSqlQuery& q) {
    estructuraDivisa div;
    div.codigo = q.value("codigo").toString().toStdString();
    div.nombre = q.value("nombre").toString().toStdString();
    div.simbolo = q.value("simbolo").toString().toStdString();
    return div;
}

void bindearTransaccionBruta(QSqlQuery& q, const estructuraTB& t) {
    q.bindValue(":amount", t.amount);
    q.bindValue(":comment", QString::fromStdString(t.comment));
    q.bindValue(":date", QString::fromStdString(t.date));
    q.bindValue(":currency", QString::fromStdString(t.currency));
    q.bindValue(":processed", t.processed);
}

void bindearTransaccionNeta(QSqlQuery& q, const estructuraTN& t) {
    q.bindValue(":amount", t.amount);
    q.bindValue(":comment", QString::fromStdString(t.comment));
    q.bindValue(":date", QString::fromStdString(t.date));
    q.bindValue(":id_TB", t.id_TB);
    q.bindValue(":category_id", t.category_id);
}

}

SQLManager::SQLManager()
{
    if(!QSqlDatabase::isDriverAvailable("QSQLITE")){
        qDebug() << "Error: QSQLITE no esta disponible";
        return;
    }

    _bd = QSqlDatabase::addDatabase("QSQLITE");
    _bd.setDatabaseName(QCoreApplication::applicationDirPath() + BBDD_NAME);

    BDConeccion conn(_bd);
    if(!conn.isOpen()) return;

    QSqlQuery q;
    bool success = true;

    if(!q.exec("CREATE TABLE IF NOT EXISTS divisa ("
                "codigo TEXT PRIMARY KEY,"
                "nombre TEXT NOT NULL,"
                "simbolo TEXT"
                ");")){
        qDebug() << "Error creando tabla divisas:" << q.lastError().text();
        success = false;
    }

    if(success && !q.exec("CREATE TABLE IF NOT EXISTS transaccion_bruta ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "amount DECIMAL(10,2) NOT NULL,"
                           "comment TEXT NOT NULL,"
                           "date DATE NOT NULL,"
                           "currency TEXT NOT NULL REFERENCES divisa(codigo),"
                           "processed BOOLEAN DEFAULT FALSE"
                           ");")){
        qDebug() << "Error creando tabla transaccion_bruta:" << q.lastError().text();
        success = false;
    }

    if(success && !q.exec("CREATE TABLE IF NOT EXISTS categoria ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "nombre TEXT NOT NULL UNIQUE,"
                           "id_padre INTEGER REFERENCES categoria(id) ON DELETE CASCADE,"
                           "CHECK (id != id_padre)"
                           ");")){
        qDebug() << "Error creando tabla categoria:" << q.lastError().text();
        success = false;
    }

    if(success && !q.exec("CREATE TABLE IF NOT EXISTS transaccion_neta ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "amount DECIMAL(10,2) NOT NULL,"
                           "comment TEXT NOT NULL,"
                           "date DATE NOT NULL,"
                           "id_TB INTEGER NOT NULL,"
                           "category_id INTEGER NOT NULL REFERENCES categoria(id),"
                           "FOREIGN KEY (id_TB) REFERENCES transaccion_bruta(id) "
                           "ON DELETE CASCADE "
                           "ON UPDATE CASCADE"
                           ");")){
        qDebug() << "Error creando tabla transaccion_neta:" << q.lastError().text();
        success = false;
    }

    if(success){
        if(!q.exec("INSERT OR IGNORE INTO divisa (codigo, nombre, simbolo) VALUES "
                    "('EUR', 'Euro', '€'), "
                    "('USD', 'Dólar estadounidense', '$'), "
                    "('GBP', 'Libra esterlina', '£')")){
            qDebug() << "Error insertando divisas básicas:" << q.lastError().text();
            success = false;
        }
    }

    if(success){
        if(!q.exec("INSERT OR IGNORE INTO categoria (id, nombre, id_padre) VALUES (0, 'raiz', NULL)")){
            qDebug() << "Error insertando categoria raiz:" << q.lastError().text();
        }
    }
}

bool SQLManager::abrirBD()
{
    if(!_bd.isOpen()){
        if(!_bd.open()){
            qDebug() << "Error abriendo BD:" << _bd.lastError().text();
            return false;
        }
    }
    return true;
}

void SQLManager::cerrarBD()
{
    if(_bd.isOpen()){
        _bd.close();
    }
}

estructuraTB SQLManager::obtenerTransaccionBrutaPorId(int id)
{
    estructuraTB tb = {0, 0.0, "", "", "", false};
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return tb;

    QSqlQuery q;
    q.prepare("SELECT id, amount, comment, date, currency, processed "
               "FROM transaccion_bruta WHERE id = :id");
    q.bindValue(":id", id);

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        return tb;
    }

    if(q.next()){
        tb = extraerTransaccionBruta(q);
    } else {
        qDebug() << "No se encontró transacción bruta con ID:" << id;
    }

    return tb;
}

std::vector<estructuraTB> SQLManager::obtenerTodasTransaccionesBrutas()
{
    std::vector<estructuraTB> listaTransacciones;
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return listaTransacciones;

    QSqlQuery q;
    if(!q.exec("SELECT id, amount, comment, date, currency, processed "
               "FROM transaccion_bruta ORDER BY date DESC, id DESC")){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        return listaTransacciones;
    }

    while(q.next()){
        listaTransacciones.push_back(extraerTransaccionBruta(q));
    }

    qDebug() << "Transacciones brutas recuperadas:" << listaTransacciones.size();
    return listaTransacciones;
}

std::vector<estructuraTB> SQLManager::obtenerTransaccionesBrutasPorFecha(const std::string& fechaInicio, const std::string& fechaFin)
{
    std::vector<estructuraTB> listaTransacciones;
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return listaTransacciones;

    QSqlQuery q;
    q.prepare("SELECT id, amount, comment, date, currency, processed "
               "FROM transaccion_bruta "
               "WHERE date BETWEEN :fechaInicio AND :fechaFin "
               "ORDER BY date DESC, id DESC");
    q.bindValue(":fechaInicio", QString::fromStdString(fechaInicio));
    q.bindValue(":fechaFin", QString::fromStdString(fechaFin));

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta por fechas:" << q.lastError().text();
        return listaTransacciones;
    }

    while(q.next()){
        listaTransacciones.push_back(extraerTransaccionBruta(q));
    }

    qDebug() << "Transacciones brutas recuperadas en el rango"
             << QString::fromStdString(fechaInicio) << "al" << QString::fromStdString(fechaFin)
             << ":" << listaTransacciones.size();

    return listaTransacciones;
}

std::vector<estructuraTN> SQLManager::obtenerTransaccionesNetasConId_TB(int id_TB)
{
    std::vector<estructuraTN> listaTransacciones;
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return listaTransacciones;

    QSqlQuery q;
    q.prepare("SELECT tn.id, tn.amount, tn.comment, tn.date, tn.id_TB, "
               "tn.category_id, c.nombre as category_name "
               "FROM transaccion_neta tn "
               "LEFT JOIN categoria c ON tn.category_id = c.id "
               "WHERE tn.id_TB = :id_TB ORDER BY tn.id");
    q.bindValue(":id_TB", id_TB);

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        return listaTransacciones;
    }

    while(q.next()){
        listaTransacciones.push_back(extraerTransaccionNeta(q));
    }

    qDebug() << "Transacciones netas para id_TB" << id_TB << ":" << listaTransacciones.size();
    return listaTransacciones;
}

std::vector<estructuraCategoria> SQLManager::obtenerTodasCategorias()
{
    std::vector<estructuraCategoria> listaCategorias;
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return listaCategorias;

    QSqlQuery q;
    if(!q.exec("SELECT id, nombre, id_padre FROM categoria ORDER BY nombre")){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        return listaCategorias;
    }

    while(q.next()){
        listaCategorias.push_back(extraerCategoria(q));
    }

    qDebug() << "Categorías recuperadas:" << listaCategorias.size();
    return listaCategorias;
}

std::string SQLManager::obtenerNombreCategoriaPorId(int id){
    std::string name = "";
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return name;

    QSqlQuery q;
    q.prepare("SELECT nombre FROM categoria WHERE id = :id");
    q.bindValue(":id", id);

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        return name;
    }

    if(q.next()){
        name = q.value("nombre").toString().toStdString();
    } else {
        qDebug() << "No se encontró categoría con ID:" << id;
    }

    return name;
}

std::vector<estructuraDivisa> SQLManager::obtenerTodasDivisas()
{
    std::vector<estructuraDivisa> listaDivisas;
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return listaDivisas;

    QSqlQuery q;
    if(!q.exec("SELECT codigo, nombre, simbolo FROM divisa ORDER BY nombre")){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        return listaDivisas;
    }

    while(q.next()){
        listaDivisas.push_back(extraerDivisa(q));
    }

    qDebug() << "Divisas recuperadas:" << listaDivisas.size();
    return listaDivisas;
}

int SQLManager::insertarTransaccionesBruta(const estructuraTB& transaccion)
{
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return -1;

    QSqlQuery q;
    q.prepare("INSERT INTO transaccion_bruta (amount, comment, date, currency, processed) "
               "VALUES (:amount, :comment, :date, :currency, :processed)");
    bindearTransaccionBruta(q, transaccion);

    if(!q.exec()){
        qDebug() << "Error insertando transacción bruta:" << q.lastError().text();
        return -1;
    }

    int insertedId = q.lastInsertId().toInt();
    qDebug() << "Transacción bruta insertada correctamente, ID:" << insertedId;
    return insertedId;
}

bool SQLManager::eliminarTransaccionBruta(int id)
{
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return false;

    QSqlQuery q;
    q.prepare("DELETE FROM transaccion_bruta WHERE id = :id");
    q.bindValue(":id", id);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error eliminando transacción bruta:" << q.lastError().text();
    } else {
        qDebug() << "Transacción bruta eliminada correctamente, filas afectadas:" << q.numRowsAffected();
    }

    return success;
}

bool SQLManager::insertarTransaccionesNetas(const estructuraTN& transaccion)
{
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return false;

    QSqlQuery q;
    q.prepare("INSERT INTO transaccion_neta (amount, comment, date, id_TB, category_id) "
               "VALUES (:amount, :comment, :date, :id_TB, :category_id)");
    bindearTransaccionNeta(q, transaccion);

    if(!q.exec()){
        qDebug() << "Error insertando transacción neta:" << q.lastError().text();
        return false;
    }

    qDebug() << "Transacción neta insertada correctamente, ID:" << q.lastInsertId().toInt();
    return true;
}

bool SQLManager::actualizarTransaccionNeta(const estructuraTN& transaccion)
{
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return false;

    QSqlQuery q;
    q.prepare("UPDATE transaccion_neta SET amount = :amount, comment = :comment, "
               "date = :date, id_TB = :id_TB, category_id = :category_id "
               "WHERE id = :id");
    bindearTransaccionNeta(q, transaccion);
    q.bindValue(":id", transaccion.id);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error actualizando transacción neta:" << q.lastError().text();
    } else {
        qDebug() << "Transacción neta actualizada correctamente, filas afectadas:" << q.numRowsAffected();
    }

    return success;
}

bool SQLManager::eliminarTransaccionNeta(int id)
{
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return false;

    QSqlQuery q;
    q.prepare("DELETE FROM transaccion_neta WHERE id = :id");
    q.bindValue(":id", id);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error eliminando transacción neta:" << q.lastError().text();
    } else {
        qDebug() << "Transacción neta eliminada correctamente, filas afectadas:" << q.numRowsAffected();
    }

    return success;
}

bool SQLManager::eliminarTransaccionesNetasPorId_TB(int id_TB)
{
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return false;

    QSqlQuery q;
    q.prepare("DELETE FROM transaccion_neta WHERE id_TB = :id_TB");
    q.bindValue(":id_TB", id_TB);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error eliminando transacciones netas por id_TB:" << q.lastError().text();
    } else {
        qDebug() << "Transacciones netas eliminadas para id_TB" << id_TB
                 << "- filas afectadas:" << q.numRowsAffected();
    }

    return success;
}

int SQLManager::obtenerIdCategoriaPorNombre(const std::string& nombre)
{
    BDConeccion conn(_bd);
    if(!conn.isOpen()) return -1;

    QSqlQuery q;
    q.prepare("SELECT id FROM categoria WHERE nombre = :nombre");
    q.bindValue(":nombre", QString::fromStdString(nombre));

    if(!q.exec()){
        qDebug() << "Error obteniendo id de categoria por nombre:"
                 << q.lastError().text();
        return -1;
    }

    if(q.next()){
        return q.value("id").toInt();
    }

    qDebug() << "No existe categoria con nombre:"
             << QString::fromStdString(nombre);

    return -1;
}
