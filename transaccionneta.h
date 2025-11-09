
#ifndef TRANSACCIONNETA_H
#define TRANSACCIONNETA_H

#include "TransaccionBase.h"

class TransaccionNeta : public TransaccionBase {
public:
    // Constructor parametrizado CORREGIDO
    TransaccionNeta(double amount, const std::string& comment, const std::string& date,
                    const int id_TB, const std::string& category);
    TransaccionNeta();

    // Getters para los nuevos atributos
    // Implementación de getters
    int getId_TB() const;
    std::string getCategory() const;

    void show_info() const;

protected:
    void SetId_TB (const int id_TB);
    void SetCategery(const std::string& category);

private:
    int m_id_TB; // id de la transaccion bruta
    std::string m_category;

    //friend class Gestor_SQL;
};



#endif // TRANSACCIONBRUTA_H
