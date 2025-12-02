#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <memory>
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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionA_adir_transaccion_Basica_triggered();
    void onCustomContextMenuRequested(const QPoint &pos);
    void onAddDerivativeTransaction();

    //void onDeleteRow();
    //void onMarkProcessed();
    void onRowSelected();

    void on_actionQuitar_2_triggered();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<ITransactionsManager> transaccionManager;

    std::vector<T_Structure> last_transactionsloaded; // TODO
    std::vector<DT_Structure> last_DerivativeTransactionsLoaded; //TODO

    // Roles personalizados para almacenar datos
    enum UserRoles {
        IdRole = Qt::UserRole + 1,  // TODO
    };

    // Métodos auxiliares
    //void setupTableWidget(QTableWidget* tableWidget, const QStringList& columnTitles);
    //void loadTransactionsInTable();


    // En la sección private de la clase MainWindow:

    // Métodos para gestión de transacciones derivadas
    void processDerivativeTransactionsChanges(
        const std::vector<DT_Structure>& oldTransactions,
        const std::vector<DT_Structure>& newTransactions,
        int parentId);

    bool transactionsAreEqual(
        const std::vector<DT_Structure>& oldTransactions,
        const std::vector<DT_Structure>& newTransactions,
        int parentId);

    std::vector<int> findDeletedTransactions(
        const std::vector<DT_Structure>& oldTransactions,
        const std::vector<DT_Structure>& newTransactions);

    std::vector<DT_Structure> findModifiedTransactions(
        const std::vector<DT_Structure>& oldTransactions,
        const std::vector<DT_Structure>& newTransactions);

    std::vector<DT_Structure> findNewTransactions(
        const std::vector<DT_Structure>& newTransactions);


    // Métodos para gestionar IDs
    //int obtenerIdDeFila(int fila) const;
    void establecerIdEnFila(int fila, int id);
    int generarNuevoId();  // Generador de IDs únicos

    // Contador para IDs (podrías usar una base de datos después)
    int contadorId;
};
#endif // MAINWINDOW_H
