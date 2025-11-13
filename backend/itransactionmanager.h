#pragma once
#include <vector>
#include <string>

// Estructura de datos neutral - solo datos, sin lógica
struct TransaccionBrutaData {
    double amount;
    std::string comment;
    std::string date;
    std::string currency;
    bool processed;
    int id;

    TransaccionBrutaData(double amt = 0.0,
                    const std::string& cmt = "",
                    const std::string& dt = "",
                    const std::string& curr = "",
                    bool proc = false,
                         int i = 0)
        : amount(amt), comment(cmt), date(dt), currency(curr), processed(proc), id(i) {}
};

// Estructura de datos neutral - solo datos, sin lógica
struct TransaccionNetaData {
    double amount;
    std::string comment;
    std::string date;
    int id_TB;
    std::string category;

    TransaccionNetaData(double amt = 0.0,
                    const std::string& cmt = "",
                    const std::string& dt = "",
                    int id = 0,
                    const std::string& cat = "")
        : amount(amt), comment(cmt), date(dt), id_TB(id), category(cat) {}
};

// INTERFAZ ABSTRACTA - solo declaraciones
class ITransaccionManager {
public:
    virtual ~ITransaccionManager() = default;  // Destructor virtual para herencia

    // Métodos puros virtuales (= 0) - deben ser implementados por clases hijas
    virtual std::vector<TransaccionBrutaData> getTransaccionesBrutas() = 0;
    virtual std::vector<TransaccionNetaData> getTransaccionesNetasPorIdTB(const int id_TB)=0;
   // virtual bool addTransaccion(const TransaccionData& data) = 0;
   // virtual bool deleteTransaccion(int index) = 0;
   // virtual bool updateTransaccion(int index, const TransaccionData& data) = 0;
   // virtual bool markAsProcessed(int index) = 0;
};
