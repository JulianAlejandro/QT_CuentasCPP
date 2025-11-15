
#ifndef DERIVATIVETRANSACTION_H
#define DERIVATIVETRANSACTION_H

#include "basetransaction.h"

class DerivativeTransaction : public BaseTransaction {
public:
    // Constructor parametrizado CORREGIDO
    DerivativeTransaction(double amount, const std::string& comment, const std::string& date,
                    int id_TB, const std::string& category);
    DerivativeTransaction();

    // Getters para los nuevos atributos
    // Implementación de getters
    int getId_TB() const;
    const std::string& getCategory() const;

    std::string toString() const override;


protected:
    void setId_TB (int id_TB);
    void setCategory(const std::string& category);

private:
    int m_id_TB; // id de la transaccion bruta
    std::string m_category;

    friend class SQLManager;
};

#endif // DERIVATIVETRANSACTION_H
