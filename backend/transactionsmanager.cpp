#include "transactionsmanager.h"
#include "backend/transactionmodel/transaction.h"
#include "backend/transactionmodel/derivativetransaction.h"

// Función para convertir Transaction a vector de strings
std::vector<std::string> convertirAStringVector(const Transaction& transaccion) {
    return {
        std::to_string(transaccion.getId()),
        std::to_string(transaccion.getAmount()),
        transaccion.getComment(),
        transaccion.getDate(),
        transaccion.getCurrency(),
        transaccion.getStateProcessed() ? "true" : "false",
    };
}

// Función para convertir DerivativeTransaction a vector de strings
std::vector<std::string> convertirAStringVectorDerivada(const DerivativeTransaction& transaccion) {
    return {
        std::to_string(transaccion.getAmount()),
        transaccion.getComment(),
        transaccion.getDate(),
        std::to_string(transaccion.getId_TB()),
        transaccion.getCategory()
    };
}

TransactionsManager::TransactionsManager() {}

// Implementación CORREGIDA - mismos nombres que la interfaz
std::vector<std::vector<std::string>> TransactionsManager::getTransactions() {
    std::vector<std::vector<std::string>> resultado;

    QList<Transaction> brutas = m_gestorSQL.retrieveAllTransactions();

    for (const Transaction& transaccion : brutas) {
        resultado.push_back(convertirAStringVector(transaccion));
    }

    return resultado;
}

std::vector<std::vector<std::string>> TransactionsManager::getDerivativeTransactionsById(int id_TB) {
    std::vector<std::vector<std::string>> resultado;

    QList<DerivativeTransaction> netas = m_gestorSQL.retrieveDerivativeTransactionsWithId(id_TB);

    for (const DerivativeTransaction& transaccion : netas) {
        resultado.push_back(convertirAStringVectorDerivada(transaccion));
    }
    return resultado;
}

std::vector<std::string> TransactionsManager::getFieldsTableTransactions(){ //TODO. esto depende del SQL

    return {"id","Amount", "Comment", "date", "currency", "processed"};
}

std::vector<std::string> TransactionsManager::getFieldsTableDerivativeTransactions(){ //TODO. esto depende del SQL

    return {"Amount", "Comment", "Date", "id_TB", "Category"};
}


