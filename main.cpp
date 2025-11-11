/*
#include <QCoreApplication>
#include "gestor_sql.h"
#include "Transaccionneta.h"
#include "transaccionbruta.h"
#include <qDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Crear algunas transacciones de prueba
    TransaccionBruta tb1(-3.2, "Compra supermercado", "2024-01-15", "EUR", false);
    TransaccionBruta tb2(-2.0, "Cafetería", "2024-01-16", "EUR", true);
    TransaccionBruta tb3(-15.5, "Gasolina", "2024-01-17", "EUR", false);
    TransaccionBruta tb4(1000.0, "Nómina", "2024-01-18", "EUR", true);
    TransaccionBruta aux;

    // Mostrar las transacciones creadas
    qDebug() << "=== TRANSACCIONES CREADAS ===";
    tb1.show_info();
    tb2.show_info();
    tb3.show_info();
    tb4.show_info();

    Gestor_SQL g;

    // Almacenar las transacciones en la BD
    qDebug() << "\n=== ALMACENANDO TRANSACCIONES EN BD ===";
    g.almacena_transaccion_bruta(tb1);
    g.almacena_transaccion_bruta(tb2);
    g.almacena_transaccion_bruta(tb3);
    g.almacena_transaccion_bruta(tb4);

    // Probar la recuperación individual
    qDebug() << "\n=== PRUEBA RECUPERACIÓN INDIVIDUAL ===";
    aux = g.devuelve_transaccion_bruta(1);
    aux.show_info();

    // PROBAR LA NUEVA FUNCIONALIDAD - OBTENER TODAS LAS TRANSACCIONES
    qDebug() << "\n=== PRUEBA OBTENER TODAS LAS TRANSACCIONES BRUTAS ===";
    QList<TransaccionBruta> todasLasTransacciones = g.devuelve_todas_transacciones_brutas();

    qDebug() << "Número total de transacciones recuperadas:" << todasLasTransacciones.size();

    // Mostrar todas las transacciones usando foreach de Qt
    qDebug() << "\n=== LISTADO COMPLETO CON FOREACH ===";
    int contador = 1;
    foreach (const TransaccionBruta& transaccion, todasLasTransacciones) {
        qDebug() << "Transacción" << contador << ":";
        transaccion.show_info();
        contador++;
    }

    // Mostrar usando iterador de C++11
    qDebug() << "\n=== LISTADO COMPLETO CON ITERADOR C++11 ===";
    for (const auto& transaccion : todasLasTransacciones) {
        transaccion.show_info();
    }

    // Mostrar información específica de cada transacción
    qDebug() << "\n=== RESUMEN DE TRANSACCIONES ===";
    for (int i = 0; i < todasLasTransacciones.size(); ++i) {
        const TransaccionBruta& tb = todasLasTransacciones[i];
        qDebug() << "ID:" << tb.getId()
                 << "| Monto:" << tb.getAmount()
                 << "| Fecha:" << QString::fromStdString(tb.getDate())
                 << "| Procesada:" << (tb.getStateProcessed() ? "Sí" : "No");
    }

    // Probar algunas operaciones con la lista
    qDebug() << "\n=== OPERACIONES CON LA LISTA ===";

    // Encontrar la transacción con mayor monto
    if (!todasLasTransacciones.isEmpty()) {
        TransaccionBruta mayorMonto = todasLasTransacciones.first();
        for (const auto& transaccion : todasLasTransacciones) {
            if (transaccion.getAmount() > mayorMonto.getAmount()) {
                mayorMonto = transaccion;
            }
        }
        qDebug() << "Transacción con mayor monto:";
        mayorMonto.show_info();
    }

    // Contar transacciones procesadas vs no procesadas
    int procesadas = 0;
    int noProcesadas = 0;
    for (const auto& transaccion : todasLasTransacciones) {
        if (transaccion.getStateProcessed()) {
            procesadas++;
        } else {
            noProcesadas++;
        }
    }
    qDebug() << "Estadísticas:";
    qDebug() << "Transacciones procesadas:" << procesadas;
    qDebug() << "Transacciones no procesadas:" << noProcesadas;
    qDebug() << "Total:" << todasLasTransacciones.size();

    // Probar con lista vacía (caso extremo)
    qDebug() << "\n=== PRUEBA CONSULTA VACÍA (caso extremo) ===";
    // Esto simularía qué pasa si no hay transacciones
    // En una situación real, podrías borrar todas y luego consultar

    qDebug() << "\n=== PRUEBA COMPLETADA ===";

    return a.exec();
}


*/

/*
#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    Widget w;
    w.show();
    return a.exec();
}
*/

#include "frontend/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
