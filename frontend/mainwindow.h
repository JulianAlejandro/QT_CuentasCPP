#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "backend/itransactionsmanager.h"
#include "commonDataTypes.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<ITransactionsManager> backend, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionA_adir_transaccion_Basica_triggered();
    void onCustomContextMenuRequested(const QPoint &pos);
    void onAddDerivativeTransaction();

    void onDeleteRow();
    //void onMarkProcessed();
    void onRowSelected();

    void on_actionQuitar_2_triggered();

    void on_dateEdit_userDateChanged(const QDate &date);

private:
    Ui::MainWindow *_ui;
    std::shared_ptr<ITransactionsManager> _transactionManager;

    std::vector<T_Structure> _last_transactionsloaded; // TODO
    std::vector<DT_Structure> _last_DerivativeTransactionsLoaded; //TODO

    // Roles personalizados para almacenar datos
    enum UserRoles {
        IdRole = Qt::UserRole + 1,  // TODO
    };

    // Métodos auxiliares
    //void setupTableWidget(QTableWidget* tableWidget, const QStringList& columnTitles);
    //void loadTransactionsInTable();


    // Métodos para gestionar IDs
    //int obtenerIdDeFila(int fila) const;
    void establecerIdEnFila(int fila, int id);
    int generarNuevoId();  // Generador de IDs únicos

    // Contador para IDs (podrías usar una base de datos después)
    int _contadorId;
};
#endif // MAINWINDOW_H
