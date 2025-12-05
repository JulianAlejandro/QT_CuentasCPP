#ifndef TRANSACTIONSMANAGER_H
#define TRANSACTIONSMANAGER_H

#include "backend/sqlmanager.h"
#include "backend/itransactionsmanager.h"

// En el header (transactionsmanager.h)


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

    UpdateResult actualizeDerivativeTransactionsWithId_T(const std::vector<DT_Structure>& new_DTs, const int id_t) override;

    void insertNewTransaction(T_Structure Ts) override;

    std::vector<std::string> getCurrencies() override;


    void deleteTransactionById(const int id) override;
    void deleteDerivativeTransactionsBYId_T(const int id_t) override;
private:
    SQLManager m_SQLManager;

    std::vector<T_Structure> m_current_Ts;
    std::vector<DT_Structure> m_current_DTs;

    std::vector<Category_Structure> m_current_category_table;

    static constexpr std::array<const char*, N_FIELDS_T> TRANSACTION_FIELD_TITLES = {
        "Date", "Concept", "Amount", "Currency"
    };

    static constexpr std::array<const char*, N_FIELDS_DT> DERIVATIVE_TRANSACTION_FIELD_TITLES = {
        "Date", "Concept", "Amount", "Category"
    };


    void deleteDerivativeTransactionsById(const int id) override;
    void insertDerivativeTransaction(const DT_Structure s) override;
    void actualizeDerivativeTransaction(const DT_Structure s) override;

    bool validateDerivativeTransactionsSum(const std::vector<DT_Structure>& DTs, int parentId);
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
