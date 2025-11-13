#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "backend/itransactionmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionA_adir_transaccion_Basica_triggered();
    void onCustomContextMenuRequested(const QPoint &pos);
    void onEditRow();
    void onDeleteRow();
    void onMarkProcessed();
    void onRowSelected();

    void on_actionQuitar_2_triggered();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<ITransaccionManager> transaccionManager;

    // Roles personalizados para almacenar datos
    enum UserRoles {
        IdRole = Qt::UserRole + 1,  // Para almacenar el ID
        ProcessedRole = Qt::UserRole + 2  // Para almacenar estado processed
    };

    enum Columna {
        AMOUNT, COMMENT, DATE, CURRENCY, PROCESSED
    };

    // Métodos auxiliares
    void crearTablaTransaccionesBrutas();
    void crearTablaTransaccionesNetas();

    void cargarTransaccionesEnTabla();

    // Métodos para gestionar IDs
    int obtenerIdDeFila(int fila) const;
    void establecerIdEnFila(int fila, int id);
    int generarNuevoId();  // Generador de IDs únicos

    // Contador para IDs (podrías usar una base de datos después)
    int contadorId;
};
#endif // MAINWINDOW_H
