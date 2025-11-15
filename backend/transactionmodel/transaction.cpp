#include "transaction.h"
#include <sstream>

// Constructor CORREGIDO
Transaction::Transaction(double amount, const std::string& comment,
                                   const std::string& date, const std::string& currency, bool processed)
    : BaseTransaction(amount, comment, date),
    m_currency(currency), m_processed(processed)
{
}

Transaction::Transaction()
    : BaseTransaction()
{
    m_currency = "";
    m_processed = false;
}

// Getters
const std::string& Transaction::getCurrency() const {
    return m_currency;
}

bool Transaction::getStateProcessed() const {
    return m_processed;
}

// Setter
void Transaction::setStateProcessed(bool state) {
    m_processed = state;
}

void Transaction::setCurrency(const std::string& currency){
    m_currency = currency;
}
void Transaction::setProcessed(const bool processed){
    m_processed = processed;
}

std::string Transaction::toString() const{
    std::ostringstream oss;

    oss << BaseTransaction::toString() << "Currency: " << m_currency << ", Procesed: " << m_processed;

    return oss.str();
}
