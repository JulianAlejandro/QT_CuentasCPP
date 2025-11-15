#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QCoreApplication>
#include <QSqlDatabase>
#include "transactionmodel/transaction.h"
#include "transactionmodel/derivativetransaction.h"

class SQLManager
{
public:
    SQLManager();

    bool saveTransaction(Transaction& tb);
    bool saveDerivativeTransaction(DerivativeTransaction& tn);

    Transaction retrieveTransactionById(const int id);
    QList<Transaction> retrieveAllTransactions();

    QList<DerivativeTransaction> retrieveDerivativeTransactionsWithId(int id_TB);


    //Transaction devuelve_transaccion_bruta(const int id);
    //DerivativeTransaction devuelve_transaccion_neta(const int id);

    //devuelve numero de transacciones_netas
    //devuelve numero de transacciones_brutas

    //bool borrar_transaccion_bruta()
    //bool borrar_transaccion_neta()
    //obtener tablas....es eso, ya toca lo otro porque sino complicado

private:
    QSqlDatabase bd;

    // Método helper para asignar el ID
    template<typename T>
    void asignarIdDesdeBD(T& transaccion, QSqlQuery& query);
};

#endif // SQLMANAGER_H
