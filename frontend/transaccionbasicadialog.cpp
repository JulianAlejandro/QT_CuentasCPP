#include "transaccionbasicadialog.h"
#include "ui_transaccionbasicadialog.h"

TransaccionBasicaDialog::TransaccionBasicaDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TransaccionBasicaDialog)
{
    ui->setupUi(this);
}

TransaccionBasicaDialog::~TransaccionBasicaDialog()
{
    delete ui;
}

void TransaccionBasicaDialog::on_buttonBox_accepted()
{
    accept();
}


void TransaccionBasicaDialog::on_buttonBox_rejected()
{
    reject();
}


double TransaccionBasicaDialog::amount() const{
    return ui->amount->value();
}
QString TransaccionBasicaDialog::comment() const{
    return ui->comment->text();
}
QString TransaccionBasicaDialog::date() const{
    return ui->date->text();
}
QString TransaccionBasicaDialog::currency() const{
    return ui->currency->text();
}
bool TransaccionBasicaDialog::processed() const{
    return ui->processed->isChecked();
}
