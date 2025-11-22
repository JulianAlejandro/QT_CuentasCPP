

#include "transactionsmanager.h"
#include "backend/sqlmanager.h"
//#include "backend/transactionmodel/transaction.h"
//#include "backend/transactionmodel/derivativetransaction.h"

/*
// Estructura para Transacción Bruta
struct estructuraTB {
    int id;
    double amount;
    QString comment;
    QString date;
    QString currency;
    bool processed;
};

// Estructura para Transacción Neta
struct estructuraTN {
    int id;
    double amount;
    QString comment;
    QString date;
    int id_TB;
    QString category_name; // Nombre de la categoría
    int category_id;       // ID de la categoría
};

// Estructura para Categoría
struct estructuraCategoria {
    int id;
    QString nombre;
    int id_padre;
};

// Estructura para Divisa
struct estructuraDivisa {
    QString codigo;
    QString nombre;
    QString simbolo;
};
*/



// Función para convertir Transaction a vector de strings
std::vector<std::string> convertirAStringVector(const estructuraTB& estructuraTB) {
    return {
        //std::to_string(transaccion.getId()),
        std::to_string(estructuraTB.id),
        std::to_string(estructuraTB.amount),
        estructuraTB.comment,
        estructuraTB.date,
        estructuraTB.currency,
        estructuraTB.processed ? "true" : "false"
    };
}


// Función para convertir DerivativeTransaction a vector de strings
std::vector<std::string> convertirAStringVectorNeta(const estructuraTN& estructuraTN) {
    return {
        std::to_string(estructuraTN.id),
        std::to_string(estructuraTN.amount),
        estructuraTN.comment,
        estructuraTN.date,
        std::to_string(estructuraTN.id_TB),
        estructuraTN.category_name,
        std::to_string(estructuraTN.category_id)
    };
}


TransactionsManager::TransactionsManager() {}

// Implementación CORREGIDA - mismos nombres que la interfaz
std::vector<std::vector<std::string>> TransactionsManager::getTransactions() {
    std::vector<std::vector<std::string>> resultado;


    std::vector<estructuraTB> brutas = m_SQLManager.obtenerTodasTransaccionesBrutas();

    for (const auto& transaccion : brutas) {
        resultado.push_back(convertirAStringVector(transaccion));
    }


    /*
    QList<Transaction> brutas = m_SQLManager.retrieveAllTransactions();

    for (const Transaction& transaccion : brutas) {
        resultado.push_back(convertirAStringVector(transaccion));
    }
    */


    return resultado;
}

std::vector<std::vector<std::string>> TransactionsManager::getDerivativeTransactionsById(int id_TB) {
    std::vector<std::vector<std::string>> resultado;

    std::vector<estructuraTN> netas = m_SQLManager.obtenerTransaccionesNetasConId_TB(id_TB);

    for (const auto& transaccion : netas){
        resultado.push_back(convertirAStringVectorNeta(transaccion));
    }

    /*
    QList<DerivativeTransaction> netas = m_SQLManager.retrieveDerivativeTransactionsWithId(id_TB);

    for (const DerivativeTransaction& transaccion : netas) {
        resultado.push_back(convertirAStringVectorDerivada(transaccion));
    }
    */
    return resultado;
}

std::vector<std::string> TransactionsManager::getFieldsTableTransactions(){ //TODO. esto depende del SQL

    return {"id","Amount", "Comment", "date", "currency", "processed"};
}

std::vector<std::string> TransactionsManager::getFieldsTableDerivativeTransactions(){ //TODO. esto depende del SQL

    return {"id", "Amount", "Comment", "Date", "id_TB", "Category", "Category_id"};
}


