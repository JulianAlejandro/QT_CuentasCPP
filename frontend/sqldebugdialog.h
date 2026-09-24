#ifndef SQLDEBUGDIALOG_H
#define SQLDEBUGDIALOG_H

#include <QDialog>
#include <vector>
#include <utility>

namespace Ui {
class SQLDebugDialog;
}

class SQLDebugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SQLDebugDialog(QWidget *parent = nullptr);
    ~SQLDebugDialog();

private slots:
    void onRefreshClicked();
    void onTableSelected(int index);
    void onResetCategoriesClicked();
    void onAddTestDataClicked();

private:
    void loadTables();
    void loadTableContent(const QString &tableName);
    void resetAndInsertCategories();
    void resetAndInsertTestTransactions();
    bool deleteAllData();
    bool insertCategoriesData();
    bool insertTransactionsData();

    Ui::SQLDebugDialog *_ui;
};

#endif
