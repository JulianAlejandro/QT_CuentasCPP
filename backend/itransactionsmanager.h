#pragma once
#include <vector>
#include <string>
#include "commonDataTypes.h"


// INTERFAZ ABSTRACTA - solo declaraciones
class ITransactionsManager {
public:
    virtual ~ITransactionsManager() = default;

    // Métodos puros virtuales (= 0) - deben ser implementados por clases hijas
    virtual std::array<std::string, N_FIELDS_T> getFieldsTableTransactions() = 0;
    virtual std::array<std::string, N_FIELDS_DT> getFieldsTableDerivativeTransactions() = 0;

    virtual std::vector<T_Structure> getTransactions() = 0;
    virtual std::vector<DT_Structure> getDerivativeTransactionsById(const int id) = 0;

    virtual std::vector<Category_Structure> getCategoryTable() = 0;

    virtual UpdateResult actualizeDerivativeTransactionsWithId_T(const std::vector<DT_Structure>& new_DTs, const int id_t) = 0;

    virtual void insertNewTransaction(T_Structure Ts) = 0;

    virtual std::vector<std::string> getCurrencies() = 0;

    virtual void deleteTransactionById(const int id) = 0;

    virtual void deleteDerivativeTransactionsBYId_T(const int id_t) = 0;
private:

    virtual void deleteDerivativeTransactionsById(const int id) = 0;
    virtual void insertDerivativeTransaction(const DT_Structure s) = 0;
    virtual void actualizeDerivativeTransaction(const DT_Structure s) = 0;

};
