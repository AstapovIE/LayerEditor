#include "processordialog.h"
#include "ui_processordialog.h"
#include <QMessageBox>

ProcessorDialog::ProcessorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessorDialog)
{
    ui->setupUi(this);
    ui->operationComboBox->addItems({"unite", "intersect", "subtract", "expand", "shrink", "del_layer", "rename_layer", "copy_layer", "layer_have_figure"});
    ui->sizeEdit->setVisible(false);

    connect(ui->operationComboBox, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, &ProcessorDialog::on_operationComboBox_currentIndexChanged);

}

ProcessorDialog::~ProcessorDialog()
{
    delete ui;
}

QString ProcessorDialog::getSourceFileName() const
{
    return ui->sourceFileNameEdit->text();
}

QString ProcessorDialog::getLayer1Name() const
{
    return ui->layer1NameEdit->text();
}

QString ProcessorDialog::getLayer2Name() const
{
    return ui->layer2NameEdit->text();
}

QString ProcessorDialog::getLayer3Name() const
{
    return ui->layer3NameEdit->text();
}

QString ProcessorDialog::getOperation() const
{
    return ui->operationComboBox->currentText();
}

QString ProcessorDialog::getOutputFileName() const
{
    return ui->outputFileNameEdit->text();
}

double ProcessorDialog::getSize() const
{
    return ui->sizeEdit->text().toDouble();
}

void ProcessorDialog::on_operationComboBox_currentIndexChanged(const QString &operation)
{
    // Сброс значений полей
    // ui->layer1NameEdit->clear();
    // ui->layer2NameEdit->clear();
    // ui->layer3NameEdit->clear();
    // ui->sizeEdit->clear();
    // ui->outputFileNameEdit->clear();

    // Отображение всех элементов перед настройкой видимости
    ui->layer1NameEdit->setVisible(true);
    ui->layer2NameEdit->setVisible(true);
    ui->layer3NameEdit->setVisible(true);
    ui->sizeEdit->setVisible(true);
    ui->toLabel->setVisible(true);
    ui->outputFileNameEdit->setVisible(true);

    if (operation == "unite" || operation == "intersect" || operation == "subtract") {
        ui->sizeEdit->setVisible(false);
    } else if (operation == "expand" || operation == "shrink") {
        ui->layer2NameEdit->setVisible(false);
        ui->layer3NameEdit->setText("L2");
    } else if (operation == "del_layer") {
        ui->layer1NameEdit->setVisible(false);
        ui->layer2NameEdit->setVisible(false);
        ui->toLabel->setVisible(false);
        ui->sizeEdit->setVisible(false);
        ui->outputFileNameEdit->setVisible(false);
        ui->layer3NameEdit->setText("L1");
    } else if (operation == "rename_layer") {
        ui->layer1NameEdit->setVisible(false);
        ui->toLabel->setVisible(false);
        ui->sizeEdit->setVisible(false);
        ui->outputFileNameEdit->setVisible(false);
        ui->layer3NameEdit->setText("new");
        ui->layer2NameEdit->setText("old");
    } else if (operation == "copy_layer") {
        ui->layer1NameEdit->setVisible(false);
        ui->sizeEdit->setVisible(false);
        ui->layer3NameEdit->setText("L2");
        ui->layer2NameEdit->setText("L1");
    } else if (operation == "layer_have_figure") {
        ui->layer3NameEdit->setVisible(false);
        ui->layer2NameEdit->setVisible(false);
        ui->toLabel->setVisible(false);
        ui->sizeEdit->setVisible(false);
        ui->outputFileNameEdit->setVisible(false);
    }
}


void ProcessorDialog::on_confirmButton_clicked()
{
    if (ui->sourceFileNameEdit->text().isEmpty() || ui->layer1NameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter all required fields.");
        return;
    }
    accept();
}

// #include "processordialog.h"
// #include "ui_processordialog.h"
// #include <QMessageBox>

// ProcessorDialog::ProcessorDialog(QWidget *parent) :
//     QDialog(parent),
//     ui(new Ui::ProcessorDialog)
// {
//     ui->setupUi(this);
//     ui->operationComboBox->addItems({"sum", "mul"});
// }

// ProcessorDialog::~ProcessorDialog()
// {
//     delete ui;
// }

// double ProcessorDialog::getNum1() const
// {
//     return ui->num1TextEdit->toPlainText().toDouble();
// }

// double ProcessorDialog::getNum2() const
// {
//     return ui->num2TextEdit->toPlainText().toDouble();
// }

// QString ProcessorDialog::getOperation() const
// {
//     return ui->operationComboBox->currentText();
// }

// void ProcessorDialog::on_confirmButton_clicked()
// {
//     if (ui->num1TextEdit->toPlainText().isEmpty() || ui->num2TextEdit->toPlainText().isEmpty()) {
//         QMessageBox::warning(this, "Input Error", "Please enter both numbers.");
//         return;
//     }
//     accept(); // Закрыть диалог и вернуть результат
// }
