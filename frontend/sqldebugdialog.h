#ifndef SQLDEBUGDIALOG_H
#define SQLDEBUGDIALOG_H

#include <QDialog>
#include "backend/sqlmanager.h"

namespace Ui {
class SQLDebugDialog;
}

class SQLDebugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SQLDebugDialog(SQLManager* sqlManager, QWidget *parent = nullptr);
    ~SQLDebugDialog();

private slots:
    void on_refreshButton_clicked();

private:
    void cargarTablas();

    Ui::SQLDebugDialog *ui;
    SQLManager* _sqlManager;
};

#endif // SQLDEBUGDIALOG_H
