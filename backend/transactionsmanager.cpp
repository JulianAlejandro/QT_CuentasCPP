

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

/*
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
*/



T_Structure obtain_TStruct(const estructuraTB& estructuraTB) {
    T_Structure e;
    e.id = estructuraTB.id;
    e.processed = estructuraTB.processed;
   // std::array<std::string, e.values.size()> data = {estructuraTB.date, estructuraTB.comment,
   //                                             std::to_string(estructuraTB.amount), estructuraTB.currency};

   // for(int i = 0; i < e.values.size(); i++){
    e.values={estructuraTB.date, estructuraTB.comment,
            std::to_string(estructuraTB.amount), estructuraTB.currency};

    return e;
}

DT_Structure obtain_DT_Struct(const estructuraTN& estructuraTN){
    DT_Structure e;
    e.id = estructuraTN.id;
    e.id_T = estructuraTN.id_TB;
    e.values = {estructuraTN.date, estructuraTN.comment, std::to_string(estructuraTN.amount),
                estructuraTN.category_name};
    return e;
}


TransactionsManager::TransactionsManager() {}

// Implementación CORREGIDA - mismos nombres que la interfaz
std::vector<T_Structure> TransactionsManager::getTransactions() {
    std::vector<T_Structure> resultado;

    std::vector<estructuraTB> brutas = m_SQLManager.obtenerTodasTransaccionesBrutas();

    for (const auto& transaccion : brutas) {
        resultado.push_back(obtain_TStruct(transaccion));
    }

    return resultado;
}

std::vector<DT_Structure> TransactionsManager::getDerivativeTransactionsById(int id_TB) {
    std::vector<DT_Structure> resultado;
    std::vector<estructuraTN> netas = m_SQLManager.obtenerTransaccionesNetasConId_TB(id_TB);

    for (const auto& transaccion : netas){
        resultado.push_back(obtain_DT_Struct(transaccion));
    }

    return resultado;
}


std::array<std::string, N_FIELDS_T> TransactionsManager::getFieldsTableTransactions(){

    return T_FIELD_TITLES;
}



std::array<std::string, N_FIELDS_DT> TransactionsManager::getFieldsTableDerivativeTransactions(){

    return DT_FIELD_TITLES;
}

void TransactionsManager::deleteDerivativeTransactionsById(const int id){
    m_SQLManager.eliminarTransaccionNeta(id);
}


void TransactionsManager::insertDerivativeTransaction(const DT_Structure s){
    estructuraTN e;
    e.id = s.id; // no hay
    e.amount = std::stod(s.values[dt_AMOUNT]);
    e.comment = s.values[dt_CONCEPT];
    e.date = s.values[dt_DATE];
    e.id_TB = s.id_T;
    e.category_name = s.values[dt_CATEGORY];
    e.category_id = m_SQLManager.obtenerIdCategoriaPorNombre(s.values[dt_CATEGORY]);

    m_SQLManager.insertarTransaccionesNetas(e);
/*
   estructuraTN e;
   for (const auto& i : values){
       e.id = 0; // da igual el valor
       e.amount = std::stod(i[dt_AMOUNT]);
       e.comment = i[dt_CONCEPT];
       e.date = i[dt_DATE];
       e.id_TB = id_t;
       e.category_name = i[dt_CATEGORY];
       e.category_id = m_SQLManager.obtenerIdCategoriaPorNombre(i[dt_CATEGORY]);
       m_SQLManager.insertarTransaccionesNetas(e);
   }
*/
}


/*
struct estructuraTN {
    int id;
    double amount;
    std::string comment;
    std::string date;
    int id_TB;
    std::string category_name;
    int category_id;
};
*/

/*
void TransactionsManager::addNewDerivativeTransactions(std::vector<std::vector<std::string>> vec){


    std::vector<estructuraTN> result;
    estructuraTN e = {0, 0.0, "", "", 0, "", 0};

    for (const auto& v : vec){
        e.id = std::stol(v[0]); // no se usa, con lo que puede tener cualquier valor
        e.amount = std::stod(v[1]);
        e.comment = v[2];
        e.date = v[3];
        e.id_TB = std::stoi(v[4]);
        e.category_name = v[5];
        e.category_id = std::stoi(v[6]);

        result.push_back(e);   
    }
    m_SQLManager.insertarTransaccionesNetas(result);
}
*/

/*
std::vector<estructuraCategoria> TransactionsManager::getCategories(){
    return m_SQLManager.obtenerTodasCategorias();
}
*/

