#ifndef TRANSACCIONBASE_H
#define TRANSACCIONBASE_H

//#include <iostream>
#include <string>

class TransaccionBase {
public:
    // Constructor parametrizado
    TransaccionBase(const double amount, const std::string& comment, const std::string& date);
    TransaccionBase();

    int getId() const;
    double getAmount() const;
    std::string getComment() const;
    std::string getDate() const;
    void show_info() const;

protected:
    void setId(int id);
    void setAmount(const double amount);
    void setComment(const std::string& comment);
    void setDate(const std::string& date);
private:

    int m_id;
    double m_amount;
    std::string m_comment;
    std::string m_date;

    friend class Gestor_SQL;
};

#endif // TRANSACCIONBASE_H
