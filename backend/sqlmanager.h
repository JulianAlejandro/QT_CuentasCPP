#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QSqlDatabase>
#include <vector>
#include <QString>
#include <string>

// Estructura para Transacción Bruta
struct estructuraTB {
    int id;
    double amount;
    std::string comment;
    std::string date;
    std::string currency;
    bool processed;
};

// Estructura para Transacción Neta
struct estructuraTN {
    int id;
    double amount;
    std::string comment;
    std::string date;
    int id_TB;
    std::string category_name; // Nombre de la categoría
    int category_id;       // ID de la categoría
};

// Estructura para Categoría
struct estructuraCategoria {
    int id;
    std::string nombre;
    int id_padre;
};

// Estructura para Divisa
struct estructuraDivisa {
    std::string codigo;
    std::string nombre;
    std::string simbolo;
};

class SQLManager
{
public:
    SQLManager();

    // Métodos para Transacciones Brutas
    estructuraTB obtenerTransaccionBrutaPorId(int id);
    std::vector<estructuraTB> obtenerTodasTransaccionesBrutas();
    std::vector<estructuraTB> obtenerTransaccionesBrutasPorEstado(bool processed);
    bool insertarTransaccionBruta(const estructuraTB& transaccion);
    bool actualizarTransaccionBruta(const estructuraTB& transaccion);
    bool eliminarTransaccionBruta(int id);
    bool marcarTransaccionBrutaComoProcesada(int id, bool processed = true);

    // Métodos para Transacciones Netas
    std::vector<estructuraTN> obtenerTransaccionesNetasConId_TB(int id_TB);
    std::vector<estructuraTN> obtenerTodasTransaccionesNetas();
    estructuraTN obtenerTransaccionNetaPorId(int id);
    bool insertarTransaccionNeta(const estructuraTN& transaccion);
    bool actualizarTransaccionNeta(const estructuraTN& transaccion);
    bool eliminarTransaccionNeta(int id);

    // Métodos para Categorías
    std::vector<estructuraCategoria> obtenerTodasCategorias();
    estructuraCategoria obtenerCategoriaPorId(int id);
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
    QSqlDatabase bd;
    bool abrirBD();
    void cerrarBD();

};

#endif // SQLMANAGER_H
