#include "sqlmanager.h"
#include <qDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QCoreApplication>
#include <vector>

SQLManager::SQLManager()
{
    if(!QSqlDatabase::isDriverAvailable("QSQLITE")){
        qDebug() << "Error: QSQLITE no esta disponible";
        return;
    }

    bd = QSqlDatabase::addDatabase("QSQLITE");
    bd.setDatabaseName(QCoreApplication::applicationDirPath() + "/datos_cuentas_cpp.db");

    if(!bd.open()){
        qDebug() << "Error abriendo BD:" << bd.lastError().text();
        return;
    }

    QSqlQuery q;
    bool success = true;

    // Crear tabla divisa
    if(!q.exec("CREATE TABLE IF NOT EXISTS divisa ("
                "codigo TEXT PRIMARY KEY,"
                "nombre TEXT NOT NULL,"
                "simbolo TEXT"
                ");")){
        qDebug() << "Error creando tabla divisas:" << q.lastError().text();
        success = false;
    }

    // Crear tabla transaccion_bruta
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

    // Crear tabla categoria con jerarquía (CORREGIDO)
    if(success && !q.exec("CREATE TABLE IF NOT EXISTS categoria ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "nombre TEXT NOT NULL UNIQUE,"
                           "id_padre INTEGER REFERENCES categoria(id) ON DELETE CASCADE,"
                           "CHECK (id != id_padre)"
                           ");")){
        qDebug() << "Error creando tabla categoria:" << q.lastError().text();
        success = false;
    }

    // Crear tabla transaccion_neta (ACTUALIZADO con category_id)
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

    // Insertar datos básicos de divisas
    if(success){
        if(!q.exec("INSERT OR IGNORE INTO divisa (codigo, nombre, simbolo) VALUES "
                    "('EUR', 'Euro', '€'), "
                    "('USD', 'Dólar estadounidense', '$'), "
                    "('GBP', 'Libra esterlina', '£')")){
            qDebug() << "Error insertando divisas básicas:" << q.lastError().text();
            success = false;
        }
    }

    // Insertar categoría raíz si no existe (CORREGIDO - usar 'nombre' en lugar de 'categoria')
    if(success){
        if(!q.exec("INSERT OR IGNORE INTO categoria (id, nombre, id_padre) VALUES (0, 'raiz', NULL)")){
            qDebug() << "Error insertando categoria raiz:" << q.lastError().text();
            success = false;
        }
    }

    bd.close();
}

bool SQLManager::abrirBD()
{
    if(!bd.isOpen()){
        if(!bd.open()){
            qDebug() << "Error abriendo BD:" << bd.lastError().text();
            return false;
        }
    }
    return true;
}

void SQLManager::cerrarBD()
{
    if(bd.isOpen()){
        bd.close();
    }
}

// Implementación para Transacciones Brutas
estructuraTB SQLManager::obtenerTransaccionBrutaPorId(int id)
{
    estructuraTB tb = {0, 0.0, "", "", "", false};

    if(!abrirBD()) return tb;

    QSqlQuery q;
    QString queryStr = "SELECT id, amount, comment, date, currency, processed "
                       "FROM transaccion_bruta WHERE id = :id";

    q.prepare(queryStr);
    q.bindValue(":id", id);

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        cerrarBD();
        return tb;
    }

    if(q.next()){
        tb.id = q.value("id").toInt();
        tb.amount = q.value("amount").toDouble();
        tb.comment = q.value("comment").toString().toStdString();
        tb.date = q.value("date").toString().toStdString();
        tb.currency = q.value("currency").toString().toStdString();
        tb.processed = q.value("processed").toBool();
    } else {
        qDebug() << "No se encontró transacción bruta con ID:" << id;
    }

    cerrarBD();
    return tb;
}

std::vector<estructuraTB> SQLManager::obtenerTodasTransaccionesBrutas()
{
    std::vector<estructuraTB> listaTransacciones;

    if(!abrirBD()) return listaTransacciones;

    QSqlQuery q;
    QString queryStr = "SELECT id, amount, comment, date, currency, processed "
                       "FROM transaccion_bruta ORDER BY date DESC, id DESC";

    if(!q.exec(queryStr)){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        cerrarBD();
        return listaTransacciones;
    }

    while(q.next()){
        estructuraTB tb;
        tb.id = q.value("id").toInt();
        tb.amount = q.value("amount").toDouble();
        tb.comment = q.value("comment").toString().toStdString();
        tb.date = q.value("date").toString().toStdString();
        tb.currency = q.value("currency").toString().toStdString();
        tb.processed = q.value("processed").toBool();

        listaTransacciones.push_back(tb);
    }

    qDebug() << "Transacciones brutas recuperadas:" << listaTransacciones.size();
    cerrarBD();
    return listaTransacciones;
}

std::vector<estructuraTB> SQLManager::obtenerTransaccionesBrutasPorEstado(bool processed)
{
    std::vector<estructuraTB> listaTransacciones;

    if(!abrirBD()) return listaTransacciones;

    QSqlQuery q;
    QString queryStr = "SELECT id, amount, comment, date, currency, processed "
                       "FROM transaccion_bruta WHERE processed = :processed "
                       "ORDER BY date DESC, id DESC";

    q.prepare(queryStr);
    q.bindValue(":processed", processed);

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        cerrarBD();
        return listaTransacciones;
    }

    while(q.next()){
        estructuraTB tb;
        tb.id = q.value("id").toInt();
        tb.amount = q.value("amount").toDouble();
        tb.comment = q.value("comment").toString().toStdString();
        tb.date = q.value("date").toString().toStdString();
        tb.currency = q.value("currency").toString().toStdString();
        tb.processed = q.value("processed").toBool();

        listaTransacciones.push_back(tb);
    }

    qDebug() << "Transacciones brutas con processed =" << processed << ":" << listaTransacciones.size();
    cerrarBD();
    return listaTransacciones;
}

// Implementación para Transacciones Netas
std::vector<estructuraTN> SQLManager::obtenerTransaccionesNetasConId_TB(int id_TB)
{
    std::vector<estructuraTN> listaTransacciones;

    if(!abrirBD()) return listaTransacciones;

    QSqlQuery q;
    QString queryStr = "SELECT tn.id, tn.amount, tn.comment, tn.date, tn.id_TB, "
                       "tn.category_id, c.nombre as category_name "
                       "FROM transaccion_neta tn "
                       "LEFT JOIN categoria c ON tn.category_id = c.id "
                       "WHERE tn.id_TB = :id_TB ORDER BY tn.id";

    q.prepare(queryStr);
    q.bindValue(":id_TB", id_TB);

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        cerrarBD();
        return listaTransacciones;
    }

    while(q.next()){
        estructuraTN tn;
        tn.id = q.value("id").toInt();
        tn.amount = q.value("amount").toDouble();
        tn.comment = q.value("comment").toString().toStdString();
        tn.date = q.value("date").toString().toStdString();
        tn.id_TB = q.value("id_TB").toInt();
        tn.category_id = q.value("category_id").toInt();
        tn.category_name = q.value("category_name").toString().toStdString();

        listaTransacciones.push_back(tn);
    }

    qDebug() << "Transacciones netas para id_TB" << id_TB << ":" << listaTransacciones.size();
    cerrarBD();
    return listaTransacciones;
}

std::vector<estructuraTN> SQLManager::obtenerTodasTransaccionesNetas()
{
    std::vector<estructuraTN> listaTransacciones;

    if(!abrirBD()) return listaTransacciones;

    QSqlQuery q;
    QString queryStr = "SELECT tn.id, tn.amount, tn.comment, tn.date, tn.id_TB, "
                       "tn.category_id, c.nombre as category_name "
                       "FROM transaccion_neta tn "
                       "LEFT JOIN categoria c ON tn.category_id = c.id "
                       "ORDER BY tn.date DESC, tn.id DESC";

    if(!q.exec(queryStr)){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        cerrarBD();
        return listaTransacciones;
    }

    while(q.next()){
        estructuraTN tn;
        tn.id = q.value("id").toInt();
        tn.amount = q.value("amount").toDouble();
        tn.comment = q.value("comment").toString().toStdString();
        tn.date = q.value("date").toString().toStdString();
        tn.id_TB = q.value("id_TB").toInt();
        tn.category_id = q.value("category_id").toInt();
        tn.category_name = q.value("category_name").toString().toStdString();

        listaTransacciones.push_back(tn);
    }

    qDebug() << "Transacciones netas recuperadas:" << listaTransacciones.size();
    cerrarBD();
    return listaTransacciones;
}

estructuraTN SQLManager::obtenerTransaccionNetaPorId(int id)
{
    estructuraTN tn = {0, 0.0, "", "", 0, "", 0};

    if(!abrirBD()) return tn;

    QSqlQuery q;
    QString queryStr = "SELECT tn.id, tn.amount, tn.comment, tn.date, tn.id_TB, "
                       "tn.category_id, c.nombre as category_name "
                       "FROM transaccion_neta tn "
                       "LEFT JOIN categoria c ON tn.category_id = c.id "
                       "WHERE tn.id = :id";

    q.prepare(queryStr);
    q.bindValue(":id", id);

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        cerrarBD();
        return tn;
    }

    if(q.next()){
        tn.id = q.value("id").toInt();
        tn.amount = q.value("amount").toDouble();
        tn.comment = q.value("comment").toString().toStdString();
        tn.date = q.value("date").toString().toStdString();
        tn.id_TB = q.value("id_TB").toInt();
        tn.category_id = q.value("category_id").toInt();
        tn.category_name = q.value("category_name").toString().toStdString();
    } else {
        qDebug() << "No se encontró transacción neta con ID:" << id;
    }

    cerrarBD();
    return tn;
}

// Implementación para Categorías
std::vector<estructuraCategoria> SQLManager::obtenerTodasCategorias()
{
    std::vector<estructuraCategoria> listaCategorias;

    if(!abrirBD()) return listaCategorias;

    QSqlQuery q;
    QString queryStr = "SELECT id, nombre, id_padre FROM categoria ORDER BY nombre";

    if(!q.exec(queryStr)){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        cerrarBD();
        return listaCategorias;
    }

    while(q.next()){
        estructuraCategoria cat;
        cat.id = q.value("id").toInt();
        cat.nombre = q.value("nombre").toString().toStdString();
        cat.id_padre = q.value("id_padre").toInt();

        listaCategorias.push_back(cat);
    }

    qDebug() << "Categorías recuperadas:" << listaCategorias.size();
    cerrarBD();
    return listaCategorias;
}

estructuraCategoria SQLManager::obtenerCategoriaPorId(int id)
{
    estructuraCategoria cat = {0, "", 0};

    if(!abrirBD()) return cat;

    QSqlQuery q;
    QString queryStr = "SELECT id, nombre, id_padre FROM categoria WHERE id = :id";

    q.prepare(queryStr);
    q.bindValue(":id", id);

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        cerrarBD();
        return cat;
    }

    if(q.next()){
        cat.id = q.value("id").toInt();
        cat.nombre = q.value("nombre").toString().toStdString();
        cat.id_padre = q.value("id_padre").toInt();
    } else {
        qDebug() << "No se encontró categoría con ID:" << id;
    }

    cerrarBD();
    return cat;
}
/*
std::string SQLManager::obtenerNombreCategoriaPorId(int id){
    std::string name = "";

    if(!abrirBD()) return name;

    QSqlQuery q;
    QString queryStr = "SELECT nombre FROM categoria WHERE id = :id";

    q.prepare(queryStr);
    q.bindValue(":id", id);

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        cerrarBD();
        return name;
    }

    if(q.next()){
        name = q.value("nombre").toString().toStdString();
    } else {
        qDebug() << "No se encontró categoría con ID:" << id;
    }

    cerrarBD();
    return name;

}
*/
std::vector<estructuraCategoria> SQLManager::obtenerCategoriasPorPadre(int id_padre)
{
    std::vector<estructuraCategoria> listaCategorias;

    if(!abrirBD()) return listaCategorias;

    QSqlQuery q;
    QString queryStr = "SELECT id, nombre, id_padre FROM categoria WHERE id_padre = :id_padre ORDER BY nombre";

    q.prepare(queryStr);
    q.bindValue(":id_padre", id_padre);

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        cerrarBD();
        return listaCategorias;
    }

    while(q.next()){
        estructuraCategoria cat;
        cat.id = q.value("id").toInt();
        cat.nombre = q.value("nombre").toString().toStdString();
        cat.id_padre = q.value("id_padre").toInt();

        listaCategorias.push_back(cat);
    }

    qDebug() << "Categorías con padre" << id_padre << ":" << listaCategorias.size();
    cerrarBD();
    return listaCategorias;
}

// Implementación para Divisas
std::vector<estructuraDivisa> SQLManager::obtenerTodasDivisas()
{
    std::vector<estructuraDivisa> listaDivisas;

    if(!abrirBD()) return listaDivisas;

    QSqlQuery q;
    QString queryStr = "SELECT codigo, nombre, simbolo FROM divisa ORDER BY nombre";

    if(!q.exec(queryStr)){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        cerrarBD();
        return listaDivisas;
    }

    while(q.next()){
        estructuraDivisa div;
        div.codigo = q.value("codigo").toString().toStdString();
        div.nombre = q.value("nombre").toString().toStdString();
        div.simbolo = q.value("simbolo").toString().toStdString();

        listaDivisas.push_back(div);
    }

    qDebug() << "Divisas recuperadas:" << listaDivisas.size();
    cerrarBD();
    return listaDivisas;
}

estructuraDivisa SQLManager::obtenerDivisaPorCodigo(const std::string& codigo)
{
    estructuraDivisa div = {"", "", ""};

    if(!abrirBD()) return div;

    QSqlQuery q;
    QString queryStr = "SELECT codigo, nombre, simbolo FROM divisa WHERE codigo = :codigo";

    q.prepare(queryStr);
    q.bindValue(":codigo", QString::fromStdString(codigo));

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        cerrarBD();
        return div;
    }

    if(q.next()){
        div.codigo = q.value("codigo").toString().toStdString();
        div.nombre = q.value("nombre").toString().toStdString();
        div.simbolo = q.value("simbolo").toString().toStdString();
    } else {
        qDebug() << "No se encontró divisa con código:" << QString::fromStdString(codigo);
    }

    cerrarBD();
    return div;
}

// Métodos para INSERTAR Transacciones Brutas
bool SQLManager::insertarTransaccionBruta(const estructuraTB& transaccion)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "INSERT INTO transaccion_bruta (amount, comment, date, currency, processed) "
                       "VALUES (:amount, :comment, :date, :currency, :processed)";

    q.prepare(queryStr);
    q.bindValue(":amount", transaccion.amount);
    q.bindValue(":comment", QString::fromStdString(transaccion.comment));
    q.bindValue(":date", QString::fromStdString(transaccion.date));
    q.bindValue(":currency", QString::fromStdString(transaccion.currency));
    q.bindValue(":processed", transaccion.processed);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error insertando transacción bruta:" << q.lastError().text();
    } else {
        qDebug() << "Transacción bruta insertada correctamente, ID:" << q.lastInsertId().toInt();
    }

    cerrarBD();
    return success;
}

bool SQLManager::actualizarTransaccionBruta(const estructuraTB& transaccion)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "UPDATE transaccion_bruta SET amount = :amount, comment = :comment, "
                       "date = :date, currency = :currency, processed = :processed "
                       "WHERE id = :id";

    q.prepare(queryStr);
    q.bindValue(":amount", transaccion.amount);
    q.bindValue(":comment", QString::fromStdString(transaccion.comment));
    q.bindValue(":date", QString::fromStdString(transaccion.date));
    q.bindValue(":currency", QString::fromStdString(transaccion.currency));
    q.bindValue(":processed", transaccion.processed);
    q.bindValue(":id", transaccion.id);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error actualizando transacción bruta:" << q.lastError().text();
    } else {
        qDebug() << "Transacción bruta actualizada correctamente, filas afectadas:" << q.numRowsAffected();
    }

    cerrarBD();
    return success;
}

bool SQLManager::eliminarTransaccionBruta(int id)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "DELETE FROM transaccion_bruta WHERE id = :id";

    q.prepare(queryStr);
    q.bindValue(":id", id);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error eliminando transacción bruta:" << q.lastError().text();
    } else {
        qDebug() << "Transacción bruta eliminada correctamente, filas afectadas:" << q.numRowsAffected();
    }

    cerrarBD();
    return success;
}

bool SQLManager::marcarTransaccionBrutaComoProcesada(int id, bool processed)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "UPDATE transaccion_bruta SET processed = :processed WHERE id = :id";

    q.prepare(queryStr);
    q.bindValue(":processed", processed);
    q.bindValue(":id", id);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error marcando transacción bruta como procesada:" << q.lastError().text();
    } else {
        qDebug() << "Transacción bruta marcada como procesada =" << processed << ", filas afectadas:" << q.numRowsAffected();
    }

    cerrarBD();
    return success;
}

// Métodos para INSERTAR Transacciones Netas
bool SQLManager::insertarTransaccionNeta(const estructuraTN& transaccion)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "INSERT INTO transaccion_neta (amount, comment, date, id_TB, category_id) "
                       "VALUES (:amount, :comment, :date, :id_TB, :category_id)";

    q.prepare(queryStr);
    q.bindValue(":amount", transaccion.amount);
    q.bindValue(":comment", QString::fromStdString(transaccion.comment));
    q.bindValue(":date", QString::fromStdString(transaccion.date));
    q.bindValue(":id_TB", transaccion.id_TB);
    q.bindValue(":category_id", transaccion.category_id);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error insertando transacción neta:" << q.lastError().text();
    } else {
        qDebug() << "Transacción neta insertada correctamente, ID:" << q.lastInsertId().toInt();
    }

    cerrarBD();
    return success;
}

bool SQLManager::actualizarTransaccionNeta(const estructuraTN& transaccion)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "UPDATE transaccion_neta SET amount = :amount, comment = :comment, "
                       "date = :date, id_TB = :id_TB, category_id = :category_id "
                       "WHERE id = :id";

    q.prepare(queryStr);
    q.bindValue(":amount", transaccion.amount);
    q.bindValue(":comment", QString::fromStdString(transaccion.comment));
    q.bindValue(":date", QString::fromStdString(transaccion.date));
    q.bindValue(":id_TB", transaccion.id_TB);
    q.bindValue(":category_id", transaccion.category_id);
    q.bindValue(":id", transaccion.id);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error actualizando transacción neta:" << q.lastError().text();
    } else {
        qDebug() << "Transacción neta actualizada correctamente, filas afectadas:" << q.numRowsAffected();
    }

    cerrarBD();
    return success;
}

bool SQLManager::eliminarTransaccionNeta(int id)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "DELETE FROM transaccion_neta WHERE id = :id";

    q.prepare(queryStr);
    q.bindValue(":id", id);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error eliminando transacción neta:" << q.lastError().text();
    } else {
        qDebug() << "Transacción neta eliminada correctamente, filas afectadas:" << q.numRowsAffected();
    }

    cerrarBD();
    return success;
}

// Métodos para INSERTAR Categorías
bool SQLManager::insertarCategoria(const estructuraCategoria& categoria)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "INSERT INTO categoria (nombre, id_padre) VALUES (:nombre, :id_padre)";

    q.prepare(queryStr);
    q.bindValue(":nombre", QString::fromStdString(categoria.nombre));

    // Si id_padre es 0, lo tratamos como NULL para la categoría raíz
    if(categoria.id_padre == 0) {
        q.bindValue(":id_padre", QVariant(QVariant::Int));
    } else {
        q.bindValue(":id_padre", categoria.id_padre);
    }

    bool success = q.exec();
    if(!success){
        qDebug() << "Error insertando categoría:" << q.lastError().text();
    } else {
        qDebug() << "Categoría insertada correctamente, ID:" << q.lastInsertId().toInt();
    }

    cerrarBD();
    return success;
}

bool SQLManager::actualizarCategoria(const estructuraCategoria& categoria)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "UPDATE categoria SET nombre = :nombre, id_padre = :id_padre WHERE id = :id";

    q.prepare(queryStr);
    q.bindValue(":nombre", QString::fromStdString(categoria.nombre));

    // Si id_padre es 0, lo tratamos como NULL para la categoría raíz
    if(categoria.id_padre == 0) {
        q.bindValue(":id_padre", QVariant(QVariant::Int));
    } else {
        q.bindValue(":id_padre", categoria.id_padre);
    }

    q.bindValue(":id", categoria.id);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error actualizando categoría:" << q.lastError().text();
    } else {
        qDebug() << "Categoría actualizada correctamente, filas afectadas:" << q.numRowsAffected();
    }

    cerrarBD();
    return success;
}

bool SQLManager::eliminarCategoria(int id)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "DELETE FROM categoria WHERE id = :id";

    q.prepare(queryStr);
    q.bindValue(":id", id);

    bool success = q.exec();
    if(!success){
        qDebug() << "Error eliminando categoría:" << q.lastError().text();
    } else {
        qDebug() << "Categoría eliminada correctamente, filas afectadas:" << q.numRowsAffected();
    }

    cerrarBD();
    return success;
}

// Métodos para INSERTAR Divisas
bool SQLManager::insertarDivisa(const estructuraDivisa& divisa)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "INSERT INTO divisa (codigo, nombre, simbolo) VALUES (:codigo, :nombre, :simbolo)";

    q.prepare(queryStr);
    q.bindValue(":codigo", QString::fromStdString(divisa.codigo));
    q.bindValue(":nombre", QString::fromStdString(divisa.nombre));
    q.bindValue(":simbolo", QString::fromStdString(divisa.simbolo));

    bool success = q.exec();
    if(!success){
        qDebug() << "Error insertando divisa:" << q.lastError().text();
    } else {
        qDebug() << "Divisa insertada correctamente:" << QString::fromStdString(divisa.codigo);
    }

    cerrarBD();
    return success;
}

bool SQLManager::actualizarDivisa(const estructuraDivisa& divisa)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "UPDATE divisa SET nombre = :nombre, simbolo = :simbolo WHERE codigo = :codigo";

    q.prepare(queryStr);
    q.bindValue(":nombre", QString::fromStdString(divisa.nombre));
    q.bindValue(":simbolo", QString::fromStdString(divisa.simbolo));
    q.bindValue(":codigo", QString::fromStdString(divisa.codigo));

    bool success = q.exec();
    if(!success){
        qDebug() << "Error actualizando divisa:" << q.lastError().text();
    } else {
        qDebug() << "Divisa actualizada correctamente, filas afectadas:" << q.numRowsAffected();
    }

    cerrarBD();
    return success;
}

bool SQLManager::eliminarDivisa(const std::string& codigo)
{
    if(!abrirBD()) return false;

    QSqlQuery q;
    QString queryStr = "DELETE FROM divisa WHERE codigo = :codigo";

    q.prepare(queryStr);
    q.bindValue(":codigo", QString::fromStdString(codigo));

    bool success = q.exec();
    if(!success){
        qDebug() << "Error eliminando divisa:" << q.lastError().text();
    } else {
        qDebug() << "Divisa eliminada correctamente, filas afectadas:" << q.numRowsAffected();
    }

    cerrarBD();
    return success;
}

/*

#include "sqlmanager.h"
#include <qDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QCoreApplication>
//#include <backend/transactionmodel/transaction.h>
//#include <backend/transactionmodel/derivativetransaction.h>


// Método template para asignar ID
template<typename T>
void SQLManager::asignarIdDesdeBD(T& transaccion, QSqlQuery& query) {
    int nuevoId = query.lastInsertId().toInt();
    if (nuevoId > 0) {
        transaccion.setId(nuevoId);
        qDebug() << "ID asignado a transaccion:" << nuevoId;
    }
}

SQLManager::SQLManager()
{
    if(!QSqlDatabase::isDriverAvailable("QSQLITE")){
        qDebug() << "Error: QSQLITE no esta disponible";
        return;
    }

    bd = QSqlDatabase::addDatabase("QSQLITE");
    bd.setDatabaseName(QCoreApplication::applicationDirPath() + "/datos_cuentas_cpp.db");
    //qDebug() << QCoreApplication::applicationDirPath();

    if(!bd.open()){
        qDebug() << "Error abriendo BD:" << bd.lastError().text();
        return;
    }

    QSqlQuery q;
    bool success = true;

    // Crear tabla transaccion_bruta
    if(!q.exec("CREATE TABLE IF NOT EXISTS transaccion_bruta ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "amount DECIMAL(10,2) NOT NULL,"
                "comment TEXT NOT NULL,"
                "date TEXT NOT NULL,"
                "currency TEXT NOT NULL,"
                "processed BOOLEAN DEFAULT FALSE"
                ");")){
        qDebug() << "Error creando tabla transaccion_bruta:" << q.lastError().text();
        success = false;
    }

    //crear tabla transaccion neta
    if(success && !q.exec("CREATE TABLE IF NOT EXISTS transaccion_neta ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "amount DECIMAL(10,2) NOT NULL,"
                           "comment TEXT NOT NULL,"
                           "date TEXT NOT NULL,"
                           "id_TB INTEGER NOT NULL,"
                           "category TEXT NOT NULL,"
                           "FOREIGN KEY (id_TB) REFERENCES transaccion_bruta(id) ON DELETE CASCADE"
                           ");")){
        qDebug() << "Error creando tabla transaccion_neta:" << q.lastError().text();
        success = false;
    }

    bd.close();
}


bool SQLManager::saveTransaction(Transaction& tb)  // Mejor pasar por referencia
{
    if(!bd.isOpen()){  // Cambiado: verificar si ya está abierta
        if(!bd.open()){
            qDebug() << "Error abriendo BD:" << bd.lastError().text();
            return false;
        }
    }

    QSqlQuery q;

    // CORRECCIÓN: Usar los nombres de columnas correctos según tu schema
    QString queryStr = "INSERT INTO transaccion_bruta (amount, comment, date, currency, processed) "
                       "VALUES (:amount, :comment, :date, :currency, :processed)";

    q.prepare(queryStr);
    // CORRECCIÓN: Usar los getters del objeto Transaction
    q.bindValue(":amount", tb.getAmount());
    q.bindValue(":comment", QString::fromStdString(tb.getComment()));
    q.bindValue(":date", QString::fromStdString(tb.getDate()));
    q.bindValue(":currency", QString::fromStdString(tb.getCurrency()));
    q.bindValue(":processed", tb.getStateProcessed());

    if(!q.exec()){
        qDebug() << "Error insertando transaccion_bruta:" << q.lastError().text();
        // NO cerrar BD aquí - mantenerla abierta
        return false;
    }

    // ASIGNAR EL ID GENERADO POR LA BD
    asignarIdDesdeBD(tb, q);

    qDebug() << "Transaccion_bruta almacenada correctamente! ID:" << tb.getId();
    bd.close();
    return true;
}


bool SQLManager::saveDerivativeTransaction(DerivativeTransaction& tn)  // Cambié el parámetro a 'tn' para DerivativeTransaction
{
    if(!bd.isOpen()){  // Verificar si ya está abierta
        if(!bd.open()){
            qDebug() << "Error abriendo BD:" << bd.lastError().text();
            return false;
        }
    }

    QSqlQuery q;

    // CORRECCIÓN: Usar los nombres de columnas correctos según tu schema
    QString queryStr = "INSERT INTO transaccion_neta (amount, comment, date, id_TB, category) "
                       "VALUES (:amount, :comment, :date, :id_TB, :category)";

    q.prepare(queryStr);

    // CORRECCIÓN: Usar los getters del objeto DerivativeTransaction
    // Asumiendo que DerivativeTransaction tiene estos métodos (deberías crearlos si no existen)
    q.bindValue(":amount", tn.getAmount());
    q.bindValue(":comment", QString::fromStdString(tn.getComment()));
    q.bindValue(":date", QString::fromStdString(tn.getDate()));
    q.bindValue(":id_TB", tn.getId_TB());  // ID de la transacción bruta relacionada
    q.bindValue(":category", QString::fromStdString(tn.getCategory()));

    if(!q.exec()){
        qDebug() << "Error insertando transaccion_neta:" << q.lastError().text();
        // NO cerrar BD aquí - mantenerla abierta
        return false;
    }

    // ASIGNAR EL ID GENERADO POR LA BD
    asignarIdDesdeBD(tn, q);

    qDebug() << "Transaccion neta almacenada correctamente! ID:" << tn.getId();
    bd.close();
    return true;
}



Transaction SQLManager::retrieveTransactionById(const int id)
{
    Transaction tb;

    if(!bd.isOpen()){
        if(!bd.open()){
            qDebug() << "Error abriendo BD:" << bd.lastError().text();
            return tb; // Retorna objeto vacío en caso de error
        }
    }

    QSqlQuery q;
    QString queryStr = "SELECT id, amount, comment, date, currency, processed "
                       "FROM transaccion_bruta WHERE id = :id";

    q.prepare(queryStr);
    q.bindValue(":id", id);

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        bd.close();
        return tb;
    }

    if(q.next()){
        // Asignar los valores desde la base de datos al objeto Transaction
        tb.setId(q.value("id").toInt());
        tb.setAmount(q.value("amount").toDouble());
        tb.setComment(q.value("comment").toString().toStdString());
        tb.setDate(q.value("date").toString().toStdString());
        tb.setCurrency(q.value("currency").toString().toStdString());
        tb.setStateProcessed(q.value("processed").toBool());

        qDebug() << "Transaccion bruta recuperada correctamente. ID:" << tb.getId();
    } else {
        qDebug() << "No se encontró transacción bruta con ID:" << id;
    }

    bd.close();
    return tb;
}


QList<Transaction> SQLManager::retrieveAllTransactions()
{
    QList<Transaction> listaTransacciones;

    if(!bd.isOpen()){
        if(!bd.open()){
            qDebug() << "Error abriendo BD:" << bd.lastError().text();
            return listaTransacciones; // Retorna lista vacía en caso de error
        }
    }

    QSqlQuery q;
    QString queryStr = "SELECT id, amount, comment, date, currency, processed "
                       "FROM transaccion_bruta ORDER BY id"; // Ordenado por ID

    if(!q.exec(queryStr)){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        bd.close();
        return listaTransacciones;
    }

    int contador = 0;
    while(q.next()){
        Transaction tb;

        // Asignar los valores desde la base de datos al objeto Transaction
        tb.setId(q.value("id").toInt());
        tb.setAmount(q.value("amount").toDouble());
        tb.setComment(q.value("comment").toString().toStdString());
        tb.setDate(q.value("date").toString().toStdString());
        tb.setCurrency(q.value("currency").toString().toStdString());
        tb.setStateProcessed(q.value("processed").toBool());

        // Añadir a la lista
        listaTransacciones.append(tb);
        contador++;
    }

    qDebug() << "Transacciones brutas recuperadas:" << contador;
    bd.close();
    return listaTransacciones;
}


QList<DerivativeTransaction> SQLManager::retrieveDerivativeTransactionsWithId(int id_TB)
{
    QList<DerivativeTransaction> listaTransacciones;

    if(!bd.isOpen()){
        if(!bd.open()){
            qDebug() << "Error abriendo BD:" << bd.lastError().text();
            return listaTransacciones; // Retorna lista vacía en caso de error
        }
    }

    QSqlQuery q;
    QString queryStr = "SELECT id, amount, comment, date, id_TB, category "
                       "FROM transaccion_neta WHERE id_TB = :id_TB ORDER BY id";

    q.prepare(queryStr);
    q.bindValue(":id_TB", id_TB);

    if(!q.exec()){
        qDebug() << "Error ejecutando consulta:" << q.lastError().text();
        bd.close();
        return listaTransacciones;
    }

    int contador = 0;
    while(q.next()){
        DerivativeTransaction tn;

        // Asignar los valores desde la base de datos al objeto DerivativeTransaction
        tn.setId(q.value("id").toInt());
        tn.setAmount(q.value("amount").toDouble());
        tn.setComment(q.value("comment").toString().toStdString());
        tn.setDate(q.value("date").toString().toStdString());
        tn.setId_TB(q.value("id_TB").toInt());
        tn.setCategory(q.value("category").toString().toStdString());

        // Añadir a la lista
        listaTransacciones.append(tn);
        contador++;
    }

    qDebug() << "Transacciones netas recuperadas para id_TB" << id_TB << ":" << contador;
    bd.close();
    return listaTransacciones;
}
*/
