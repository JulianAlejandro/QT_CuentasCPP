
#include "TransaccionNeta.h"
#include <QDebug>

// Constructor CORREGIDO - parámetros consistentes
TransaccionNeta::TransaccionNeta(double amount, const std::string& comment,
                                 const std::string& date, const int id_TB,  const std::string& category)
    : TransaccionBase(amount, comment, date), // Llama al constructor de la clase base
    m_id_TB(id_TB), m_category(category)
{
}

TransaccionNeta::TransaccionNeta()
    : TransaccionBase() // Llama al constructor de la clase base
{
    m_id_TB = 0;
    m_category = "";
}

// Implementación de getters
int TransaccionNeta::getId_TB() const {
    return m_id_TB;
}

// Implementación de getters
std::string TransaccionNeta::getCategory() const {
    return m_category;
}


void TransaccionNeta::SetId_TB (const int id_TB){
    m_id_TB = id_TB;
}
void TransaccionNeta::SetCategory(const std::string& category){
    m_category = category;
}


void TransaccionNeta::show_info() const{
    TransaccionBase::show_info();
    qDebug() << "id Trans Bruta =" << m_id_TB << "Category =" << m_category;
}
