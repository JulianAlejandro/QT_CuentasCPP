#pragma once
#include <vector>
#include <string>
#include "commonDataTypes.h"


// INTERFAZ ABSTRACTA - solo declaraciones
class ITransactionsManager {
public:
    virtual ~ITransactionsManager() = default;  // Destructor virtual para herencia

    // Métodos puros virtuales (= 0) - deben ser implementados por clases hijas
    virtual std::array<std::string, N_FIELDS_T> getFieldsTableTransactions() = 0;
    virtual std::array<std::string, N_FIELDS_DT> getFieldsTableDerivativeTransactions() = 0;

    virtual std::vector<T_Structure> getTransactions() = 0;
    virtual std::vector<DT_Structure> getDerivativeTransactionsById(const int id) = 0;

    //virtual void addNewDerivativeTransactions(std::vector<DT_Structure> vec) = 0;

    //virtual std::vector<Category_Structure> getCategories() = 0;
   // virtual bool addTransaccion(const TransaccionData& data) = 0;
   // virtual bool deleteTransaccion(int index) = 0;
   // virtual bool updateTransaccion(int index, const TransaccionData& data) = 0;
   // virtual bool markAsProcessed(int index) = 0;
};
