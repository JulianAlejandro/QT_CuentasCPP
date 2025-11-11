#ifndef TRANSACCIONBASICADIALOG_H
#define TRANSACCIONBASICADIALOG_H

#include <QDialog>

namespace Ui {
class TransaccionBasicaDialog;
}

class TransaccionBasicaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransaccionBasicaDialog(QWidget *parent = nullptr);
    ~TransaccionBasicaDialog();

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
    Ui::TransaccionBasicaDialog *ui;
};

#endif // TRANSACCIONBASICADIALOG_H
