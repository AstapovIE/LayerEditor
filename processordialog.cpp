#include "processordialog.h"
#include "ui_processordialog.h"
#include <QMessageBox>

ProcessorDialog::ProcessorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessorDialog)
{
    ui->setupUi(this);
    ui->operationComboBox->addItems({"sum", "mul"});
}

ProcessorDialog::~ProcessorDialog()
{
    delete ui;
}

double ProcessorDialog::getNum1() const
{
    return ui->num1TextEdit->toPlainText().toDouble();
}

double ProcessorDialog::getNum2() const
{
    return ui->num2TextEdit->toPlainText().toDouble();
}

QString ProcessorDialog::getOperation() const
{
    return ui->operationComboBox->currentText();
}

void ProcessorDialog::on_confirmButton_clicked()
{
    if (ui->num1TextEdit->toPlainText().isEmpty() || ui->num2TextEdit->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both numbers.");
        return;
    }
    accept(); // Закрыть диалог и вернуть результат
}
