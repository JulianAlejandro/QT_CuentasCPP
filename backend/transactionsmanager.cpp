

#include "transactionsmanager.h"
#include "backend/sqlmanager.h"
#include <QMessageBox>

#include <QDate>


T_Structure obtain_TStruct(const estructuraTB& estructuraTB) {
    T_Structure e;
    e.id = estructuraTB.id;
    e.processed = estructuraTB.processed;
    e.values={estructuraTB.date, estructuraTB.comment,
            std::to_string(estructuraTB.amount), estructuraTB.currency};
    e.tipo = estructuraTB.tipo;
    return e;
}

DT_Structure obtain_DT_Struct(const estructuraTN& estructuraTN){
    DT_Structure e;
    e.id = estructuraTN.id;
    e.id_T = estructuraTN.id_TB;
    e.values = {estructuraTN.date, estructuraTN.comment, std::to_string(estructuraTN.amount),
                estructuraTN.category_name};
    e.tipo = estructuraTN.tipo;
    return e;
}


TransactionsManager::TransactionsManager(std::shared_ptr<SQLManager> sqlManager)
: _sqlManager(sqlManager)
{
}


// Implementación CORREGIDA - mismos nombres que la interfaz
std::vector<T_Structure> TransactionsManager::getTransactions() {
    std::vector<T_Structure> resultado;

    std::vector<estructuraTB> brutas = _sqlManager->obtenerTodasTransaccionesBrutas();

    for (const auto& transaccion : brutas) {
        resultado.push_back(obtain_TStruct(transaccion));
    }

    return resultado;
}

std::vector<T_Structure> TransactionsManager::getTransactionsByMonth(const std::string& fecha) {
    std::vector<T_Structure> resultado;

    // 1. Convertimos el std::string ("2026-06") a QDate de Qt para calcular los días
    // Le añadimos "-01" temporalmente para que Qt pueda interpretar una fecha válida
    QString formatoMes = QString::fromStdString(fecha) + "-01";
    QDate fechaAux = QDate::fromString(formatoMes, "yyyy-MM-dd");

    if (!fechaAux.isValid()) {
        qWarning() << "Error: La fecha recibida en el backend no es válida:" << formatoMes;
        return resultado;
    }

    // 2. Construimos los strings de inicio y fin que necesita tu SQLManager
    // fechaInicio será: "2026-06-01"
    std::string fechaInicio = fechaAux.toString("yyyy-MM-01").toStdString();
    // fechaFin será: "2026-06-30" (Qt calcula automáticamente si es 28, 29, 30 o 31)
    std::string fechaFin = QDate(fechaAux.year(), fechaAux.month(), fechaAux.daysInMonth()).toString("yyyy-MM-dd").toStdString();

    // 3. Llamamos a tu método de la base de datos que ya funciona perfectamente
    std::vector<estructuraTB> brutas = _sqlManager->obtenerTransaccionesBrutasPorFecha(fechaInicio, fechaFin);

    // 4. Mapeamos las estructuras a tu T_Structure de la app
    for (const auto& transaccion : brutas) {
        resultado.push_back(obtain_TStruct(transaccion));
    }

    return resultado;
}

std::vector<DT_Structure> TransactionsManager::getDerivativeTransactionsById(int id_TB) {
    std::vector<DT_Structure> resultado;
    std::vector<estructuraTN> netas = _sqlManager->obtenerTransaccionesNetasConId_TB(id_TB);

    for (const auto& transaccion : netas){
        resultado.push_back(obtain_DT_Struct(transaccion));
    }

    return resultado;
}

void TransactionsManager::deleteDerivativeTransactionsById(const int id){
    _sqlManager->eliminarTransaccionNeta(id);
}


void TransactionsManager::insertDerivativeTransaction(const DT_Structure s){
    estructuraTN e;
    e.id = s.id;
    e.amount = std::stod(s.values[dt_AMOUNT]);
    e.comment = s.values[dt_CONCEPT];
    e.date = s.values[dt_DATE];
    e.id_TB = s.id_T;
    e.category_name = s.values[dt_CATEGORY];
    e.category_id = _sqlManager->obtenerIdCategoriaPorNombreYTipo(s.values[dt_CATEGORY], s.tipo);
    e.tipo = s.tipo;

    _sqlManager->insertarTransaccionesNetas(e);

}


void TransactionsManager::actualizeDerivativeTransaction(const DT_Structure s){
    estructuraTN e;
    e.id = s.id;
    e.amount = std::stod(s.values[dt_AMOUNT]);
    e.comment = s.values[dt_CONCEPT];
    e.date = s.values[dt_DATE];
    e.id_TB = s.id_T;
    e.category_name = s.values[dt_CATEGORY];
    e.category_id = _sqlManager->obtenerIdCategoriaPorNombreYTipo(s.values[dt_CATEGORY], s.tipo);
    e.tipo = s.tipo;

    _sqlManager->actualizarTransaccionNeta(e);
}

std::vector<Category_Structure> TransactionsManager::getCategoryTable(){

    std::vector<Category_Structure> c_struct;
    std::vector<estructuraCategoria> estr_c = _sqlManager->obtenerTodasCategorias();

    for (const auto& i : estr_c){
        Category_Structure aux;

        aux.id = i.id;
        aux.id_parent = i.id_padre;
        aux.name = i.nombre;
        aux.tipo = i.tipo;
        c_struct.push_back(aux);
    }
    _current_category_table = c_struct;
    return _current_category_table;

}

std::vector<Category_Structure> TransactionsManager::getCategoryTable(const std::string& tipo){
    std::vector<Category_Structure> c_struct;
    std::vector<estructuraCategoria> estr_c = _sqlManager->obtenerCategoriasPorTipo(tipo);

    for (const auto& i : estr_c){
        Category_Structure aux;
        aux.id = i.id;
        aux.id_parent = i.id_padre;
        aux.name = i.nombre;
        aux.tipo = i.tipo;
        c_struct.push_back(aux);
    }
    return c_struct;
}

UpdateResult TransactionsManager::actualizeDerivativeTransactionsWithId_T(const std::vector<DT_Structure>& new_DTs, const int id_t){

    if(!validateDerivativeTransactionsSum(new_DTs, id_t)){
        return UpdateResult::SumMismatch;
    }

    std::vector<DT_Structure> old_DTs = getDerivativeTransactionsById(id_t);

    if (transactionsAreEqual(old_DTs, new_DTs)) {
        return UpdateResult::NoChanges;
    }

    processDerivativeTransactionsChanges(old_DTs, new_DTs, id_t);
    return UpdateResult::Success;
}

bool TransactionsManager::validateDerivativeTransactionsSum(const std::vector<DT_Structure>& DTs, int parentId) {
    estructuraTB parentTB = _sqlManager->obtenerTransaccionBrutaPorId(parentId);

    if (parentTB.id == 0) {
        throw std::runtime_error("Transacción padre con ID " + std::to_string(parentId) + " no encontrada");
    }

    double parentAmount = parentTB.amount;

    double sumDerivativeAmounts = 0.0;

    for (const auto& dt : DTs) {
        try {
            sumDerivativeAmounts += std::stod(dt.values[dt_AMOUNT]);
        } catch (const std::exception& e) {
            throw std::runtime_error("Error al convertir el monto de una transacción derivada: " +
                                     std::string(e.what()));
        }
    }

    const double EPSILON = 0.001;

    if (std::abs(sumDerivativeAmounts - parentAmount) > EPSILON) {
        return false;
    }else{
        return true;
    }
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



void TransactionsManager::insertNewTransaction(T_Structure Ts){

    estructuraTB e;
    e.id = Ts.id;
    e.amount = stod(Ts.values[t_AMOUNT]);
    e.comment = Ts.values[t_CONCEPT];
    e.currency = Ts.values[t_CURRENCY];
    e.date = Ts.values[t_DATE];
    e.processed = Ts.processed;
    e.tipo = Ts.tipo;

    _sqlManager->insertarTransaccionesBruta(e);
}


int TransactionsManager::insertNewTransactionWithDefaultDerivative(const T_Structure& Ts){

    estructuraTB e;
    e.id = Ts.id;
    e.amount = stod(Ts.values[t_AMOUNT]);
    e.comment = Ts.values[t_CONCEPT];
    e.currency = Ts.values[t_CURRENCY];
    e.date = Ts.values[t_DATE];
    e.processed = Ts.processed;
    e.tipo = Ts.tipo;

    int id_bruta = _sqlManager->insertarTransaccionesBruta(e);
    if (id_bruta == -1) return -1;

    std::string categoryName = _sqlManager->obtenerNombreCategoriaPorId(Ts.category_id);

    estructuraTN tn;
    tn.id = -1;
    tn.amount = e.amount;
    tn.comment = e.comment;
    tn.date = e.date;
    tn.id_TB = id_bruta;
    tn.category_id = Ts.category_id;
    tn.category_name = categoryName;
    tn.tipo = e.tipo;

    bool ok = _sqlManager->insertarTransaccionesNetas(tn);
    if (!ok) return -1;

    getDerivativeTransactionsById(id_bruta);

    return id_bruta;
}


std::vector<std::string> TransactionsManager::getCurrencies(){

    std::vector<std::string> result;
    std::vector<estructuraDivisa> e;
    e = _sqlManager->obtenerTodasDivisas();
    for (const auto& i : e){
        result.push_back(i.codigo);
    }
    return result;
}


void TransactionsManager::deleteTransactionById(const int id){
    _sqlManager->eliminarTransaccionBruta(id);
}
