#include "TransaccionBruta.h"
#include <QDebug>

// Constructor CORREGIDO
TransaccionBruta::TransaccionBruta(double amount, const std::string& comment,
                                   const std::string& date, const std::string& currency, bool processed)
    : TransaccionBase(amount, comment, date),
    m_currency(currency), m_processed(processed)
{
}

TransaccionBruta::TransaccionBruta()
    : TransaccionBase()
{
    m_currency = "";
    m_processed = false;
}

// Getters
std::string TransaccionBruta::getCurrency() const {
    return m_currency;
}

bool TransaccionBruta::getStateProcessed() const {
    return m_processed;
}

// Setter
void TransaccionBruta::setStateProcessed(bool state) {
    m_processed = state;
}

void TransaccionBruta::setCurrency(const std::string& currency){
    m_currency = currency;
}
void TransaccionBruta::setProcessed(const bool processed){
    m_processed = processed;
}

void TransaccionBruta::show_info() const{
    TransaccionBase::show_info();

    qDebug() << "currency =" << m_currency << "processed =" << m_processed;

}
