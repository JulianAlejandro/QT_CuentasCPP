#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "backend/itransactionmanager.h"  // Incluir la interfaz

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

private:
    Ui::MainWindow *ui;
    std::unique_ptr<ITransaccionManager> transaccionManager;  // Usar interfaz

    // Métodos auxiliares
    void crearTablaTransaccionesBrutas();
    void cargarTransaccionesEnTabla();



    enum columna{
        AMOUNT, COMMENT, DATE, CURRENCY, PROCESSED
    };
};
#endif // MAINWINDOW_H
