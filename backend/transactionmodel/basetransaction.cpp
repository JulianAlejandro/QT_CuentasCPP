#include "basetransaction.h"
#include <sstream>

// Constructor parametrizado
BaseTransaction::BaseTransaction(const double amount, const std::string& comment, const std::string& date)
    :m_id(0), m_amount(amount), m_comment(comment), m_date(date)
{
    // Inicialización mediante lista de inicialización
}

BaseTransaction::BaseTransaction()
    : m_id(0), m_amount(0.0), m_comment(""), m_date("")
{}

// Getter para el ID
int BaseTransaction::getId() const {
    return m_id;
}

// Getter para el amount (monto)
double BaseTransaction::getAmount() const {
    return m_amount;
}

// Getter para el comment (comentario)
const std::string& BaseTransaction::getComment() const {
    return m_comment;
}

// Getter para la date (fecha)
const std::string& BaseTransaction::getDate() const {
    return m_date;
}

void BaseTransaction::setId(int id){
    m_id = id;
}

void BaseTransaction::setAmount(const double amount){
    m_amount = amount;
}

void BaseTransaction::setComment(const std::string& comment){
    m_comment = comment;
}

void BaseTransaction::setDate(const std::string& date){
    m_date = date;
}

std::string BaseTransaction::toString() const {
    std::ostringstream oss;
    oss << "ID: " << m_id << ", Amount: " << m_amount
        << ", Comment: " << m_comment << ", Date: " << m_date;
    return oss.str();
}
