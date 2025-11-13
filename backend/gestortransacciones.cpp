#include "gestortransacciones.h"
#include "backend/transaccionbruta.h"
#include "backend/transaccionneta.h"
//#include "backend/transaccionneta.h"

namespace {
    TransaccionBrutaData convertirATransaccionData(TransaccionBruta transaccion){

        TransaccionBrutaData aux;
        aux.amount =  transaccion.getAmount();
        aux.comment = transaccion.getComment();
        aux.date = transaccion.getDate();
        aux.currency = transaccion.getCurrency();
        aux.processed = transaccion.getStateProcessed();
        aux.id = transaccion.getId();
        return aux;
    }

    TransaccionNetaData convertirATransaccionData(TransaccionNeta transaccion){
       /*
        double amount;
        std::string comment;
        std::string date;
        int id_TB;
        std::string category;
        */
        TransaccionNetaData aux;
        aux.amount = transaccion.getAmount();
        aux.comment = transaccion.getComment();
        aux.date = transaccion.getDate();
        aux.id_TB = transaccion.getId_TB();
        aux.category = transaccion.getCategory();
        return aux;
    }

}

gestorTransacciones::gestorTransacciones() {}

/*
QList<TransaccionBruta> gestorTransacciones::getTransaccionesBrutas(){

    return m_gestorSQL.devuelve_todas_transacciones_brutas();

}
*/


// Implementación de métodos de la interfaz
std::vector<TransaccionBrutaData> gestorTransacciones::getTransaccionesBrutas() {
    std::vector<TransaccionBrutaData> resultado;

    // Usar tu método existente y convertir
    QList<TransaccionBruta> brutas = m_gestorSQL.devuelve_todas_transacciones_brutas();

    for (const TransaccionBruta& transaccion : brutas) {
        resultado.push_back(convertirATransaccionData(transaccion));
    }

    return resultado;
}


std::vector<TransaccionNetaData> gestorTransacciones::getTransaccionesNetasPorIdTB(const int id_TB){

    std::vector<TransaccionNetaData> resultado;

    QList<TransaccionNeta> netas = m_gestorSQL.devuelve_transacciones_netas(id_TB);

    for (const TransaccionNeta& transaccion : netas) {
        resultado.push_back(convertirATransaccionData(transaccion));
    }
    return resultado;
}
