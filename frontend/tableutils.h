
#ifndef TABLEUTILS_H
#define TABLEUTILS_H

//class QTableWidget;
//class QStringList;

#include "QTableWidget.h"
#include "QStringList.h"

namespace TableUtils {
void setupTableWidget(QTableWidget* tableWidget, const QStringList& columnTitles, bool edit);
QStringList vectorString_to_QStringList( std::vector<std::string> v);
void loadTableTransactions(QTableWidget* table, std::vector<std::vector<std::string>> transacciones, int IdRole);
}

//void load(QTableWidget* table, const std::vector<std::vector<std::string>>& data);
//void loadTransactionsInTable();

#endif // TABLEUTILS_H






