#pragma once
#include <vector>
#include <string>



// INTERFAZ ABSTRACTA - solo declaraciones
class ITransactionsManager {
public:
    virtual ~ITransactionsManager() = default;  // Destructor virtual para herencia

    // Métodos puros virtuales (= 0) - deben ser implementados por clases hijas
    virtual std::vector<std::string> getFieldsTableTransactions() = 0;
    virtual std::vector<std::string> getFieldsTableDerivativeTransactions() = 0;

    virtual std::vector<std::vector<std::string>> getTransactions() = 0;
    virtual std::vector<std::vector<std::string>> getDerivativeTransactionsById(const int id_TB)=0;
   // virtual bool addTransaccion(const TransaccionData& data) = 0;
   // virtual bool deleteTransaccion(int index) = 0;
   // virtual bool updateTransaccion(int index, const TransaccionData& data) = 0;
   // virtual bool markAsProcessed(int index) = 0;
};
