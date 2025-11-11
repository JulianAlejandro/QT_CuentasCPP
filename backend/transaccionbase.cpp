#include "TransaccionBase.h"
#include <qDebug>

// Constructor parametrizado
TransaccionBase::TransaccionBase(const double amount, const std::string& comment, const std::string& date)
    :m_id(0), m_amount(amount), m_comment(comment), m_date(date)
{
    // Inicialización mediante lista de inicialización
}

TransaccionBase::TransaccionBase(void)
{
    m_id = 0;
    m_amount = 0;
    m_comment = "";
    m_date = "";
}

// Getter para el ID
int TransaccionBase::getId() const {
    return m_id;
}

// Getter para el amount (monto)
double TransaccionBase::getAmount() const {
    return m_amount;
}

// Getter para el comment (comentario)
std::string TransaccionBase::getComment() const {
    return m_comment;
}

// Getter para la date (fecha)
std::string TransaccionBase::getDate() const {
    return m_date;
}

void TransaccionBase::setId(int id){
    m_id = id;
}

void TransaccionBase::setAmount(const double amount){
    m_amount = amount;
}

void TransaccionBase::setComment(const std::string& comment){
    m_comment = comment;
}

void TransaccionBase::setDate(const std::string& date){
    m_date = date;
}

void TransaccionBase::show_info() const{

    qDebug() << "Id =" << m_id << "Amount =" << m_amount << " comment:" << m_comment << " Date:" << m_date;

}
