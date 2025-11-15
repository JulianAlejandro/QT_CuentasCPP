#include "transactionsmanager.h"
#include "backend/transactionmodel/transaction.h"
#include "backend/transactionmodel/derivativetransaction.h"
//#include "backend/DerivativeTransaction.h"


TransactionData convertirATransaccionData(Transaction transaccion){

    TransactionData aux;
    aux.amount =  transaccion.getAmount();
    aux.comment = transaccion.getComment();
    aux.date = transaccion.getDate();
    aux.currency = transaccion.getCurrency();
    aux.processed = transaccion.getStateProcessed();
    aux.id = transaccion.getId();
    return aux;
}

DerivativeTransactionData convertirATransaccionData(DerivativeTransaction transaccion){
    /*
    double amount;
    std::string comment;
    std::string date;
    int id_TB;
    std::string category;
    */
    DerivativeTransactionData aux;
    aux.amount = transaccion.getAmount();
    aux.comment = transaccion.getComment();
    aux.date = transaccion.getDate();
    aux.id_TB = transaccion.getId_TB();
    aux.category = transaccion.getCategory();
    return aux;
}


TransactionsManager::TransactionsManager() {}

/*
QList<Transaction> TransactionsManager::getTransaccionesBrutas(){

    return m_gestorSQL.devuelve_todas_transacciones_brutas();

}
*/


// Implementación de métodos de la interfaz
std::vector<TransactionData> TransactionsManager::getTransactions() {
    std::vector<TransactionData> resultado;

    // Usar tu método existente y convertir
    QList<Transaction> brutas = m_gestorSQL.retrieveAllTransactions();

    for (const Transaction& transaccion : brutas) {
        resultado.push_back(convertirATransaccionData(transaccion));
    }

    return resultado;
}


std::vector<DerivativeTransactionData> TransactionsManager::getDerivativeTransaccionesById(const int id){

    std::vector<DerivativeTransactionData> resultado;

    QList<DerivativeTransaction> netas = m_gestorSQL.retrieveDerivativeTransactionsWithId(id);

    for (const DerivativeTransaction& transaccion : netas) {
        resultado.push_back(convertirATransaccionData(transaccion));
    }
    return resultado;
}
