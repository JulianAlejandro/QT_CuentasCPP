#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    //pulsar en anadir transaccion basica
    void on_actionA_adir_transaccion_Basica_triggered();

    //acciones de context menu request
    void onCustomContextMenuRequested(const QPoint &pos);
    void onEditRow();
    void onDeleteRow();
    void onMarkProcessed();
    void onRowSelected(); // ← Agrega esta línea

    //aplicar color en filas....a modificar
    //void applyRowColor(int row, bool processed);

private:
    Ui::MainWindow *ui;

    enum columna{
        AMOUNT, COMMENT, DATE, CURRENCY
    };
};
#endif // MAINWINDOW_H

