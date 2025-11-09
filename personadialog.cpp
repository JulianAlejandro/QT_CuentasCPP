#include "personadialog.h"
#include "ui_personadialog.h"

PersonaDialog::PersonaDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PersonaDialog)
{
    ui->setupUi(this);
}

PersonaDialog::~PersonaDialog()
{
    delete ui;
}

void PersonaDialog::on_buttonBox_accepted()
{
    accept();
}


void PersonaDialog::on_buttonBox_rejected()
{
    reject();
}


double PersonaDialog::amount() const{
    return ui->amount->value();
}
QString PersonaDialog::comment() const{
    return ui->comment->text();
}
QString PersonaDialog::date() const{
    return ui->date->text();
}
QString PersonaDialog::currency() const{
    return ui->currency->text();
}
bool PersonaDialog::processed() const{
    return ui->processed->isChecked();
}
