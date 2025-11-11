#ifndef PERSONADIALOG_H
#define PERSONADIALOG_H

#include <QDialog>

namespace Ui {
class PersonaDialog;
}

class PersonaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PersonaDialog(QWidget *parent = nullptr);
    ~PersonaDialog();

    double amount() const;
    QString comment() const;
    QString date() const;
    QString currency() const;
    bool processed() const;

   // (double amount, const std::string& comment, const std::string& date,
    // const std::string& currency, bool processed);
    /*
    QString nombre() const;
    int edad() const;
    double salario() const;
    */
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::PersonaDialog *ui;
};

#endif // PERSONADIALOG_H
