#pragma once
#include <vector>
#include <string>

// Estructura de datos neutral - solo datos, sin lógica
struct TransactionData {
    double amount;
    std::string comment;
    std::string date;
    std::string currency;
    bool processed;
    int id;

    TransactionData(double amt = 0.0,
                    const std::string& cmt = "",
                    const std::string& dt = "",
                    const std::string& curr = "",
                    bool proc = false,
                         int i = 0)
        : amount(amt), comment(cmt), date(dt), currency(curr), processed(proc), id(i) {}
};

// Estructura de datos neutral - solo datos, sin lógica
struct DerivativeTransactionData {
    double amount;
    std::string comment;
    std::string date;
    int id_TB;
    std::string category;

    DerivativeTransactionData(double amt = 0.0,
                    const std::string& cmt = "",
                    const std::string& dt = "",
                    int id = 0,
                    const std::string& cat = "")
        : amount(amt), comment(cmt), date(dt), id_TB(id), category(cat) {}
};

// INTERFAZ ABSTRACTA - solo declaraciones
class ITransaccionsManager {
public:
    virtual ~ITransaccionsManager() = default;  // Destructor virtual para herencia

    // Métodos puros virtuales (= 0) - deben ser implementados por clases hijas
    virtual std::vector<TransactionData> getTransactions() = 0;
    virtual std::vector<DerivativeTransactionData> getDerivativeTransaccionesById(const int id_DT)=0;
   // virtual bool addTransaccion(const TransaccionData& data) = 0;
   // virtual bool deleteTransaccion(int index) = 0;
   // virtual bool updateTransaccion(int index, const TransaccionData& data) = 0;
   // virtual bool markAsProcessed(int index) = 0;
};
