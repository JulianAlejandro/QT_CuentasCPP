
#include "DerivativeTransaction.h"
#include <QDebug>

// Constructor CORREGIDO - parámetros consistentes
DerivativeTransaction::DerivativeTransaction(double amount, const std::string& comment,
                                 const std::string& date, const int id_TB,  const std::string& category)
    : BaseTransaction(amount, comment, date), // Llama al constructor de la clase base
    m_id_TB(id_TB), m_category(category)
{
}

DerivativeTransaction::DerivativeTransaction()
    : BaseTransaction() // Llama al constructor de la clase base
{
    m_id_TB = 0;
    m_category = "";
}

// Implementación de getters
int DerivativeTransaction::getId_TB() const {
    return m_id_TB;
}

// Implementación de getters
std::string DerivativeTransaction::getCategory() const {
    return m_category;
}


void DerivativeTransaction::SetId_TB (const int id_TB){
    m_id_TB = id_TB;
}
void DerivativeTransaction::SetCategory(const std::string& category){
    m_category = category;
}


void DerivativeTransaction::show_info() const{
    BaseTransaction::show_info();
    qDebug() << "id Trans Bruta =" << m_id_TB << "Category =" << m_category;
}
