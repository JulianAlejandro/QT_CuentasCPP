#ifndef TRANSACTIONSMANAGER_H
#define TRANSACTIONSMANAGER_H

#include "backend/sqlmanager.h"
#include "backend/itransactionsmanager.h"

class TransactionsManager : public ITransactionsManager
{
public:
    TransactionsManager();

    // Métodos que coinciden EXACTAMENTE con la interfaz

    std::vector<std::string> getFieldsTableTransactions() override;
    std::vector<std::string> getFieldsTableDerivativeTransactions() override;

    std::vector<std::vector<std::string>> getTransactions() override;
    std::vector<std::vector<std::string>> getDerivativeTransactionsById(int id_TB) override;

private:
    SQLManager m_gestorSQL;
};

#endif // TRANSACTIONSMANAGER_H
