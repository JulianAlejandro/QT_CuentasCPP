#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "basetransaction.h"
#include <string>

class Transaction : public BaseTransaction {
public:
    // Constructor - quitar 'const' del bool (no es necesario)
    Transaction(double amount, const std::string& comment, const std::string& date,
                     const std::string& currency, bool processed);
    Transaction();

    // Getters
    const std::string& getCurrency() const;
    bool getStateProcessed() const;

    // Setters
    void setStateProcessed(bool state);
    std::string toString() const override;


protected:
    void setCurrency(const std::string& currency);
    void setProcessed(bool processed);
private:
    std::string m_currency;
    bool m_processed;

    friend class SQLManager;
};

#endif // TRANSACTION_H
