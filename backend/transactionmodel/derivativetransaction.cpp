#include "DerivativeTransaction.h"
#include <sstream>

// Constructor CORREGIDO - parámetros consistentes
DerivativeTransaction::DerivativeTransaction(double amount, const std::string& comment,
                                 const std::string& date, const int id_TB,  const std::string& category)
    : BaseTransaction(amount, comment, date), // Llama al constructor de la clase base
    m_id_TB(id_TB), m_category(category)
{
}

DerivativeTransaction::DerivativeTransaction()
    : BaseTransaction(),  // Opcional - se llama automáticamente
    m_id_TB(0),
    m_category("")
{}

// Implementación de getters
int DerivativeTransaction::getId_TB() const {
    return m_id_TB;
}

// Implementación de getters
const std::string& DerivativeTransaction::getCategory() const {
    return m_category;
}


void DerivativeTransaction::setId_TB (const int id_TB){
    m_id_TB = id_TB;
}
void DerivativeTransaction::setCategory(const std::string& category){
    m_category = category;
}


std::string DerivativeTransaction::toString() const {
    std::ostringstream oss;

    oss << BaseTransaction::toString() << "ID_tb: " << m_id_TB << ", category: " << m_category;

    return oss.str();
}
