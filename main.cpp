

#include "frontend/mainwindow.h"

#include <QApplication>
#include "frontend/categorytreewidgetdialog.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}


/*

#include <QCoreApplication>
#include <QDebug>
#include "backend/sqlmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SQLManager sqlManager;

    qDebug() << "=== INICIALIZACIÓN DE DATOS DE PRUEBA ===";

    // 1. Insertar una categoría
    qDebug() << "\n1. Insertando categoría...";
    estructuraCategoria nuevaCategoria;
    nuevaCategoria.nombre = "gastos";
    nuevaCategoria.id_padre = 0; // Categoría raíz

    if (sqlManager.insertarCategoria(nuevaCategoria)) {
        qDebug() << "✓ Categoría insertada correctamente";
    } else {
        qDebug() << "✗ Error insertando categoría";
    }

    // 2. Insertar una divisa (si no existe)
    qDebug() << "\n2. Insertando divisa...";
    estructuraDivisa nuevaDivisa;
    nuevaDivisa.codigo = "JPY";
    nuevaDivisa.nombre = "Yen Japonés";
    nuevaDivisa.simbolo = "¥";

    if (sqlManager.insertarDivisa(nuevaDivisa)) {
        qDebug() << "✓ Divisa insertada correctamente";
    } else {
        qDebug() << "✗ Error insertando divisa (puede que ya exista)";
    }

    // 3. Insertar una transacción bruta
    qDebug() << "\n3. Insertando transacción bruta...";
    estructuraTB nuevaTransaccionBruta;
    nuevaTransaccionBruta.amount = 150.75;
    nuevaTransaccionBruta.comment = "Compra en supermercado";
    nuevaTransaccionBruta.date = "2024-01-15";
    nuevaTransaccionBruta.currency = "EUR";
    nuevaTransaccionBruta.processed = false;

    if (sqlManager.insertarTransaccionBruta(nuevaTransaccionBruta)) {
        qDebug() << "✓ Transacción bruta insertada correctamente";
    } else {
        qDebug() << "✗ Error insertando transacción bruta";
    }

    // 4. Insertar una transacción neta (necesitamos el ID de la transacción bruta y categoría)
    qDebug() << "\n4. Insertando transacción neta...";

    estructuraTN nuevaTransaccionNeta;
    nuevaTransaccionNeta.amount = 150.75;
    nuevaTransaccionNeta.comment = "Compra de alimentos";
    nuevaTransaccionNeta.date = "2024-01-15";
    nuevaTransaccionNeta.id_TB = 3;
    nuevaTransaccionNeta.category_id = 1;

    sqlManager.insertarTransaccionNeta(nuevaTransaccionNeta);


    // 5. Mostrar los datos insertados
    qDebug() << "\n5. Mostrando datos insertados:";

    qDebug() << "\n--- Categorías ---";
    auto categorias = sqlManager.obtenerTodasCategorias();
    for (const auto& cat : categorias) {
        qDebug() << "ID:" << cat.id << "| Nombre:" << cat.nombre << "| Padre:" << cat.id_padre;
    }

    qDebug() << "\n--- Divisas ---";
    auto divisas = sqlManager.obtenerTodasDivisas();
    for (const auto& div : divisas) {
        qDebug() << "Código:" << div.codigo << "| Nombre:" << div.nombre << "| Símbolo:" << div.simbolo;
    }

    qDebug() << "\n--- Transacciones Brutas ---";
    auto tBrutas = sqlManager.obtenerTodasTransaccionesBrutas();
    for (const auto& tb : tBrutas) {
        qDebug() << "ID:" << tb.id << "| Amount:" << tb.amount << "| Comment:" << tb.comment
                 << "| Date:" << tb.date << "| Currency:" << tb.currency << "| Processed:" << tb.processed;
    }

    qDebug() << "\n--- Transacciones Netas ---";
    auto tNetas = sqlManager.obtenerTodasTransaccionesNetas();
    for (const auto& tn : tNetas) {
        qDebug() << "ID:" << tn.id << "| Amount:" << tn.amount << "| Comment:" << tn.comment
                 << "| Date:" << tn.date << "| ID_TB:" << tn.id_TB << "| Categoría:" << tn.category_name;
    }

    qDebug() << "\n=== PRUEBA COMPLETADA ===";
    qDebug() << "Base de datos inicializada con datos de prueba.";

    return 0;
    // return app.exec(); // No necesitamos el event loop para esta prueba simple
}

*/
