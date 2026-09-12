#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QSqlDatabase>
#include <vector>
#include <QString>
#include <QStringList>
#include <string>
#include "DataTypes.h"

#define BBDD_NAME "/datos_cuentas_cpp.db" // TODO: no se si es buena idea poner asi el path

struct TablaSQL {
    QString nombre;
    QStringList columnas;
    std::vector<QStringList> filas;
};

class SQLManager
{
public:
    SQLManager();

    // Métodos para Transacciones Brutas
    estructuraTB obtenerTransaccionBrutaPorId(int id);
    std::vector<estructuraTB> obtenerTodasTransaccionesBrutas();
    std::vector<estructuraTB> obtenerTransaccionesBrutasPorFecha(const std::string& fechaInicio, const std::string& fechaFin);
    int insertarTransaccionesBruta(const estructuraTB& transaccion);
    bool eliminarTransaccionBruta(int id);

    // Métodos para Transacciones Netas
    std::vector<estructuraTN> obtenerTransaccionesNetasConId_TB(int id_TB);
    bool insertarTransaccionesNetas(const estructuraTN& transaccion);
    bool actualizarTransaccionNeta(const estructuraTN& transaccion);
    bool eliminarTransaccionNeta(int id);

    // Métodos para Categorías
    std::vector<estructuraCategoria> obtenerTodasCategorias();
    int obtenerIdCategoriaPorNombre(const std::string& nombre);
    std::string obtenerNombreCategoriaPorId(int id);

    // Métodos para Divisas
    std::vector<estructuraDivisa> obtenerTodasDivisas();

    // Debug
    std::vector<TablaSQL> obtenerTodasLasTablas();

private:
    QSqlDatabase _bd;
};

#endif // SQLMANAGER_H
