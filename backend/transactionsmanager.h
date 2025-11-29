#ifndef TRANSACTIONSMANAGER_H
#define TRANSACTIONSMANAGER_H

#include "backend/sqlmanager.h"
#include "backend/itransactionsmanager.h"

class TransactionsManager : public ITransactionsManager
{
public:
    TransactionsManager();

    // Métodos que coinciden EXACTAMENTE con la int erfaz

    std::array<std::string, N_FIELDS_T> getFieldsTableTransactions() override;
    std::array<std::string, N_FIELDS_DT> getFieldsTableDerivativeTransactions() override;

    std::vector<T_Structure> getTransactions() override;
    std::vector<DT_Structure> getDerivativeTransactionsById(int id) override;

    //void addNewDerivativeTransactions(std::vector<DT_Structure> vec) override;

    //std::vector<Category_Structure> getCategories() override;

private:
    SQLManager m_SQLManager;
};

#endif // TRANSACTIONSMANAGER_H
