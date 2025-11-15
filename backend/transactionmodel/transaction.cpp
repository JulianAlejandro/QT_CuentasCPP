#include "transaction.h"
#include <QDebug>

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
std::string Transaction::getCurrency() const {
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

void Transaction::show_info() const{
    BaseTransaction::show_info();

    qDebug() << "currency =" << m_currency << "processed =" << m_processed;

}
