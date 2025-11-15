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
