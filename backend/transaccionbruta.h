#ifndef TRANSACCIONBRUTA_H
#define TRANSACCIONBRUTA_H

#include "TransaccionBase.h"
#include <string>

class TransaccionBruta : public TransaccionBase {
public:
    // Constructor - quitar 'const' del bool (no es necesario)
    TransaccionBruta(double amount, const std::string& comment, const std::string& date,
                     const std::string& currency, bool processed);
    TransaccionBruta();

    // Getters
    std::string getCurrency() const;
    bool getStateProcessed() const;

    // Setters
    void setStateProcessed(bool state);
    void show_info() const;

protected:
    void setCurrency(const std::string& currency);
    void setProcessed(const bool processed);
private:
    std::string m_currency;
    bool m_processed;

    friend class Gestor_SQL;
};

#endif // TRANSACCIONBRUTA_H
