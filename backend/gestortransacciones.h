#ifndef GESTORTRANSACCIONES_H
#define GESTORTRANSACCIONES_H

#include <QObject>
#include "backend/gestor_sql.h"
//#include "backend/transaccionbruta.h"
//#include "backend/transaccionneta.h"
#include "backend/itransactionmanager.h"

class gestorTransacciones : public QObject, public ITransaccionManager
{
    Q_OBJECT
public:
    gestorTransacciones();
    //metodos de comunicacion con frontend
   // QList<TransaccionBruta> getTransaccionesBrutas();
    //QList<TransaccionNeta> getTransaccionesNetasPorId_TB ();
    std::vector<TransaccionBrutaData> getTransaccionesBrutas() override;
    std::vector<TransaccionNetaData> getTransaccionesNetasPorIdTB(const int id_TB) override;


private:
    Gestor_SQL m_gestorSQL;
};

#endif // GESTORTRANSACCIONES_H
