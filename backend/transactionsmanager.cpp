

#include "transactionsmanager.h"
#include "backend/sqlmanager.h"
#include <QMessageBox>
#include <set>
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


TransactionsManager::TransactionsManager() {
}


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

void TransactionsManager::actualizeDerivativeTransactionsById_T(const std::vector<DT_Structure> current_DTs, const std::vector<DT_Structure> new_DTs, const int id_t){


    // Si no hay cambios, no hacer nada
    if (transactionsAreEqual(current_DTs, new_DTs)) {
        return;
    }

    // Procesar los cambios
    processDerivativeTransactionsChanges(current_DTs, new_DTs, id_t);

}

bool TransactionsManager::transactionsAreEqual(const std::vector<DT_Structure>& oldTransactions,
                                      const std::vector<DT_Structure>& newTransactions)
{
    // Primero comparar tamaños
    if (oldTransactions.size() != newTransactions.size()) {
        return false;
    }

    // Crear mapas para comparación más eficiente
    std::map<int, DT_Structure> oldMap, newMap;

    for (const auto& dt : oldTransactions) {
        oldMap[dt.id] = dt;
    }

    for (const auto& dt : newTransactions) {
        // Para transacciones nuevas (id = -1), las consideramos diferentes
        if (dt.id == -1) {
            return false;
        }
        newMap[dt.id] = dt;
    }

    // Comparar cada transacción
    for (const auto& [id, oldDT] : oldMap) {
        auto it = newMap.find(id);
        if (it == newMap.end()) {
            return false; // Transacción eliminada
        }

        const auto& newDT = it->second;

        // Comparar todos los valores
        for (size_t i = 0; i < oldDT.values.size(); ++i) {
            if (oldDT.values[i] != newDT.values[i]) {
                return false; // Valor modificado
            }
        }
    }

    return true;
}


// Método para procesar todos los cambios
void TransactionsManager::processDerivativeTransactionsChanges(
    const std::vector<DT_Structure>& oldTransactions,
    const std::vector<DT_Structure>& newTransactions,
    int parentId)
{
    // Crear mapas para las transacciones
    std::map<int, DT_Structure> oldMap;
    std::map<int, DT_Structure> newMap;

    for (const auto& dt : oldTransactions) {
        oldMap[dt.id] = dt;
    }

    // Crear una copia modificable de newTransactions para establecer id_T
    std::vector<DT_Structure> modifiedNewTransactions = newTransactions;

    for (auto& dt : modifiedNewTransactions) {
        dt.id_T = parentId; // Establecer el ID padre
        if (dt.id != -1) {
            newMap[dt.id] = dt;
        }
    }

    // 1. Encontrar transacciones eliminadas (están en oldMap pero no en newMap)
    for (const auto& [id, oldDT] : oldMap) {
        if (newMap.find(id) == newMap.end()) {
            // Transacción eliminada
            //transactionManager->eliminarDerivativeTransaction(id);
            deleteDerivativeTransactionsById(id);
            qDebug() << "Transacción derivada ID" << id << "eliminada";
        }
    }

    // 2. Encontrar transacciones modificadas
    for (const auto& [id, newDT] : newMap) {
        auto oldIt = oldMap.find(id);
        if (oldIt != oldMap.end()) {
            // La transacción existe, verificar si fue modificada
            const auto& oldDT = oldIt->second;
            bool modified = false;

            // Comparar todos los valores
            for (size_t i = 0; i < oldDT.values.size(); ++i) {
                if (oldDT.values[i] != newDT.values[i]) {
                    modified = true;
                    break;
                }
            }

            if (modified) {
                // Transacción modificada
                actualizeDerivativeTransaction(newDT);
                qDebug() << "Transacción derivada ID" << id << "actualizada";
            }
        }
    }

    // 3. Encontrar transacciones nuevas (id = -1)
    for (const auto& newDT : modifiedNewTransactions) {
        if (newDT.id == -1) {
            // Transacción nueva
            insertDerivativeTransaction(newDT);
            qDebug() << "Nueva transacción derivada añadida para transacción padre ID" << parentId;
        }
    }
}



// Método auxiliar para obtener transacciones eliminadas
std::vector<int> TransactionsManager::findDeletedTransactions(
    const std::vector<DT_Structure>& oldTransactions,
    const std::vector<DT_Structure>& newTransactions)
{
    std::vector<int> deletedIds;
    std::set<int> newIds;

    // Recoger todos los IDs de las nuevas transacciones (excluyendo -1)
    for (const auto& dt : newTransactions) {
        if (dt.id != -1) {
            newIds.insert(dt.id);
        }
    }

    // Buscar IDs que están en las viejas pero no en las nuevas
    for (const auto& dt : oldTransactions) {
        if (newIds.find(dt.id) == newIds.end()) {
            deletedIds.push_back(dt.id);
        }
    }

    return deletedIds;
}

// Método auxiliar para obtener transacciones modificadas
std::vector<DT_Structure> TransactionsManager::findModifiedTransactions(
    const std::vector<DT_Structure>& oldTransactions,
    const std::vector<DT_Structure>& newTransactions)
{
    std::vector<DT_Structure> modifiedTransactions;
    std::map<int, DT_Structure> oldMap;

    // Crear mapa de transacciones antiguas
    for (const auto& dt : oldTransactions) {
        oldMap[dt.id] = dt;
    }

    // Comparar con las nuevas
    for (const auto& newDT : newTransactions) {
        if (newDT.id == -1) continue; // Saltar nuevas

        auto it = oldMap.find(newDT.id);
        if (it != oldMap.end()) {
            const auto& oldDT = it->second;

            // Verificar si hay cambios
            bool changed = false;
            for (size_t i = 0; i < oldDT.values.size(); ++i) {
                if (oldDT.values[i] != newDT.values[i]) {
                    changed = true;
                    break;
                }
            }

            if (changed) {
                modifiedTransactions.push_back(newDT);
            }
        }
    }

    return modifiedTransactions;
}

// Método auxiliar para obtener transacciones nuevas
std::vector<DT_Structure> TransactionsManager::findNewTransactions(
    const std::vector<DT_Structure>& newTransactions)
{
    std::vector<DT_Structure> newTrans;

    for (const auto& dt : newTransactions) {
        if (dt.id == -1) {
            newTrans.push_back(dt);
        }
    }

    return newTrans;
}
