#include "widget.h"
#include "ui_widget.h"
#include "personadialog.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QStringList titulos;
    setWindowTitle("Trasnsacciones brutas");
    ui->tableWidget->setColumnCount(5);
    titulos << "Amount"  << "Comment"  << "Date" << "Currency" << "Processed";
    ui->tableWidget->setHorizontalHeaderLabels(titulos);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_agrega_clicked()
{

    int res;
    PersonaDialog pd(this);
    pd.setWindowTitle("Transacciones brutas");
    res = pd.exec();
    if (res ==QDialog::Rejected){
        return;
    }

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, AMOUNT, new QTableWidgetItem(QString::number(pd.amount())));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, COMMENT, new QTableWidgetItem(pd.comment()));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, DATE, new QTableWidgetItem(pd.date()));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, CURRENCY, new QTableWidgetItem(pd.currency()));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, PROCESSED, new QTableWidgetItem(pd.processed() ? "Yes" : "No"));

}

