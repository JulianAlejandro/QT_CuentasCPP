#ifndef GESTOR_SQL_H
#define GESTOR_SQL_H

#include <QCoreApplication>
#include <QSqlDatabase>
#include "transaccionbruta.h"
#include "transaccionneta.h"

class Gestor_SQL
{
public:
    Gestor_SQL();

    bool almacena_transaccion_bruta(TransaccionBruta& tb);
    bool almacena_transaccion_neta(TransaccionNeta& tn);

    TransaccionBruta devuelve_transaccion_bruta(const int id);
    QList<TransaccionBruta> devuelve_todas_transacciones_brutas();

    //TransaccionBruta devuelve_transaccion_bruta(const int id);
    //TransaccionNeta devuelve_transaccion_neta(const int id);

    //devuelve numero de transacciones_netas
    //devuelve numero de transacciones_brutas

    //bool borrar_transaccion_bruta()
    //bool borrar_transaccion_neta()
    //obtener tablas....es eso, ya toca lo otro porque sino complicado

private:
    QSqlDatabase bd;

    // Método helper para asignar el ID
    template<typename T>
    void asignarIdDesdeBD(T& transaccion, QSqlQuery& query);
};

#endif // GESTOR_SQL_H
