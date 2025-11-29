#ifndef TABLEUTILS_H
#define TABLEUTILS_H

#include "QTableWidget.h"
//#include "QStringList.h"
#include <array>
#include <string>
#include <vector>

namespace TableUtils {

void setFieldsTableWidget(QTableWidget* tableWidget, const QStringList& columnTitles, bool edit);

template <std::size_t N>
QStringList arrayString_to_QStringList(const std::array<std::string, N>& arr) {
    QStringList result;
    for (const auto& s : arr)
        result << QString::fromStdString(s);
    return result;
}

// 🔥 AQUI DEBE IR LA DEFINICIÓN COMPLETA
template<typename S>
void loadTransactionsTableWidget(QTableWidget* table,
                                 const std::vector<S>& transacciones,
                                 int IdRole)
{
    table->setRowCount(0);

    for (const auto& trans : transacciones) {

        int newRow = table->rowCount();
        table->insertRow(newRow);

        int id = trans.id;

        for (std::size_t i = 0; i < trans.values.size(); i++) {

            QTableWidgetItem* item =
                new QTableWidgetItem(QString::fromStdString(trans.values[i]));

            item->setData(IdRole, id);
            table->setItem(newRow, i, item);
        }
    }
}

} // namespace TableUtils

#endif // TABLEUTILS_H




