#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QSqlDatabase>
#include <vector>
#include <QString>
#include <string>
#include "DataTypes.h"

#define BBDD_NAME "/datos_cuentas_cpp.db" // TODO: no se si es buena idea poner asi el path

class SQLManager
{
public:
    SQLManager();

    // Métodos para Transacciones Brutas
    estructuraTB obtenerTransaccionBrutaPorId(int id);
    std::vector<estructuraTB> obtenerTodasTransaccionesBrutas();
    std::vector<estructuraTB> obtenerTransaccionesBrutasPorFecha(const std::string& fechaInicio, const std::string& fechaFin);
    std::vector<estructuraTB> obtenerTransaccionesBrutasPorEstado(bool processed);
    bool insertarTransaccionesBruta(const estructuraTB& transaccion);
    bool insertarTransaccionesBrutas(const std::vector<estructuraTB>& transacciones);

    bool actualizarTransaccionBruta(const estructuraTB& transaccion);
    bool eliminarTransaccionBruta(int id);
    bool marcarTransaccionBrutaComoProcesada(int id, bool processed = true);

    // Métodos para Transacciones Netas
    std::vector<estructuraTN> obtenerTransaccionesNetasConId_TB(int id_TB);
    std::vector<estructuraTN> obtenerTodasTransaccionesNetas();
    estructuraTN obtenerTransaccionNetaPorId(int id);
    bool insertarTransaccionesNetas(const estructuraTN& transaccion);
    bool insertarTransaccionesNetas(const std::vector<estructuraTN>& transacciones);
    bool actualizarTransaccionNeta(const estructuraTN& transaccion);
    bool eliminarTransaccionNeta(int id);

    // Métodos para Categorías
    std::vector<estructuraCategoria> obtenerTodasCategorias();
    estructuraCategoria obtenerCategoriaPorId(int id);
    int obtenerIdCategoriaPorNombre(const std::string& nombre);
    //std::string obtenerNombreCategoriaPorId(int id);
    std::vector<estructuraCategoria> obtenerCategoriasPorPadre(int id_padre);
    bool insertarCategoria(const estructuraCategoria& categoria);
    bool actualizarCategoria(const estructuraCategoria& categoria);
    bool eliminarCategoria(int id);

    // Métodos para Divisas
    std::vector<estructuraDivisa> obtenerTodasDivisas();
    estructuraDivisa obtenerDivisaPorCodigo(const std::string& codigo);
    bool insertarDivisa(const estructuraDivisa& divisa);
    bool actualizarDivisa(const estructuraDivisa& divisa);
    bool eliminarDivisa(const std::string& codigo);

private:
    QSqlDatabase _bd;
    bool abrirBD();
    void cerrarBD();
};

#endif // SQLMANAGER_H
