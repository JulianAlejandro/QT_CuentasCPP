
#ifndef DERIVATIVETRANSACTION_H
#define DERIVATIVETRANSACTION_H

#include "basetransaction.h"

class DerivativeTransaction : public BaseTransaction {
public:
    // Constructor parametrizado CORREGIDO
    DerivativeTransaction(double amount, const std::string& comment, const std::string& date,
                    const int id_TB, const std::string& category);
    DerivativeTransaction();

    // Getters para los nuevos atributos
    // Implementación de getters
    int getId_TB() const;
    std::string getCategory() const;

    void show_info() const;

protected:
    void SetId_TB (const int id_TB);
    void SetCategory(const std::string& category);

private:
    int m_id_TB; // id de la transaccion bruta
    std::string m_category;

    friend class SQLManager;
};



#endif // TRANSACCIONBRUTA_H
