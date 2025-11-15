#ifndef BASETRANSACTION_H
#define BASETRANSACTION_H

//#include <iostream>
#include <string>

class BaseTransaction {
public:
    // Constructor parametrizado
    BaseTransaction(double amount, const std::string& comment, const std::string& date);
    BaseTransaction();

    virtual ~BaseTransaction() = default;

    int getId() const;
    double getAmount() const;
    const std::string& getComment() const;
    const std::string& getDate() const;
    virtual std::string toString() const;

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

    friend class Manager_SQL;
};

#endif // BASETRANSACTION_H
