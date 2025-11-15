#ifndef TRANSACTIONSMANAGER_H
#define TRANSACTIONSMANAGER_H

#include <QObject>
#include "backend/sqlmanager.h"
//#include "backend/Transaction.h"
//#include "backend/DerivativeTransaction.h"
#include "backend/itransactionsmanager.h"

class TransactionsManager : public QObject, public ITransaccionsManager
{
    Q_OBJECT
public:
    TransactionsManager();
    //metodos de comunicacion con frontend
   // QList<Transaction> getTransaccionesBrutas();
    //QList<DerivativeTransaction> getTransaccionesNetasPorId_TB ();
    std::vector<TransactionData> getTransactions() override;
    std::vector<DerivativeTransactionData> getDerivativeTransaccionesById(const int id_DT) override;


private:
    SQLManager m_gestorSQL;
};

#endif // SQLManager_H
