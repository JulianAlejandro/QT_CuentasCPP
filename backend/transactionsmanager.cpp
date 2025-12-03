

#include "transactionsmanager.h"
#include "backend/sqlmanager.h"
//#include "backend/transactionmodel/transaction.h"
//#include "backend/transactionmodel/derivativetransaction.h"


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

}


void TransactionsManager::actualizeDerivativeTransaction(const DT_Structure s){
    estructuraTN e;
    e.id = s.id;
    e.amount = std::stod(s.values[dt_AMOUNT]);
    e.comment = s.values[dt_CONCEPT];
    e.date = s.values[dt_DATE];
    e.id_TB = s.id_T;
    e.category_name = s.values[dt_CATEGORY];
    e.category_id = m_SQLManager.obtenerIdCategoriaPorNombre(s.values[dt_CATEGORY]);

    m_SQLManager.actualizarTransaccionNeta(e);
}

std::vector<Category_Structure> TransactionsManager::getCategoryTable(){

    std::vector<Category_Structure> c_struct;
    std::vector<estructuraCategoria> estr_c = m_SQLManager.obtenerTodasCategorias();

    for (const auto& i : estr_c){
        Category_Structure aux;

        aux.id = i.id;
        aux.id_parent = i.id_padre;
        aux.name = i.nombre;
        c_struct.push_back(aux);
    }

    return c_struct;

}

/*
std::vector<estructuraCategoria> TransactionsManager::getCategories(){
    return m_SQLManager.obtenerTodasCategorias();
}
*/

