#ifndef TRANSACTIONSMANAGER_H
#define TRANSACTIONSMANAGER_H

#include "backend/sqlmanager.h"
#include "backend/itransactionsmanager.h"

class TransactionsManager : public ITransactionsManager
{
public:
    TransactionsManager();

    std::array<std::string, N_FIELDS_T> getFieldsTableTransactions() override {
        std::array<std::string, N_FIELDS_T> result;
        std::copy(std::begin(TRANSACTION_FIELD_TITLES),
                  std::end(TRANSACTION_FIELD_TITLES),
                  result.begin());
        return result;
    }

    std::array<std::string, N_FIELDS_DT> getFieldsTableDerivativeTransactions() override {
        std::array<std::string, N_FIELDS_DT> result;
        std::copy(std::begin(DERIVATIVE_TRANSACTION_FIELD_TITLES),
                  std::end(DERIVATIVE_TRANSACTION_FIELD_TITLES),
                  result.begin());
        return result;
    }

    std::vector<T_Structure> getTransactions() override;
    std::vector<DT_Structure> getDerivativeTransactionsById(int id) override;

    std::vector<Category_Structure> getCategoryTable() override;

    void actualizeDerivativeTransactionsById_T(const std::vector<DT_Structure> current_DTs,const std::vector<DT_Structure> new_DTs, const int id_t) override;

private:
    SQLManager m_SQLManager;

    static constexpr std::array<const char*, N_FIELDS_T> TRANSACTION_FIELD_TITLES = {
        "Date", "Concept", "Amount", "Currency"
    };

    static constexpr std::array<const char*, N_FIELDS_DT> DERIVATIVE_TRANSACTION_FIELD_TITLES = {
        "Date", "Concept", "Amount", "Category"
    };


    void deleteDerivativeTransactionsById(const int id) override;
    void insertDerivativeTransaction(const DT_Structure s) override;
    void actualizeDerivativeTransaction(const DT_Structure s) override;


    void processDerivativeTransactionsChanges(
        const std::vector<DT_Structure>& oldTransactions,
        const std::vector<DT_Structure>& newTransactions,
        int parentId);

    bool transactionsAreEqual(
        const std::vector<DT_Structure>& oldTransactions,
        const std::vector<DT_Structure>& newTransactions);

    std::vector<int> findDeletedTransactions(
        const std::vector<DT_Structure>& oldTransactions,
        const std::vector<DT_Structure>& newTransactions);

    std::vector<DT_Structure> findModifiedTransactions(
        const std::vector<DT_Structure>& oldTransactions,
        const std::vector<DT_Structure>& newTransactions);

    std::vector<DT_Structure> findNewTransactions(
        const std::vector<DT_Structure>& newTransactions);
};

#endif // TRANSACTIONSMANAGER_H
