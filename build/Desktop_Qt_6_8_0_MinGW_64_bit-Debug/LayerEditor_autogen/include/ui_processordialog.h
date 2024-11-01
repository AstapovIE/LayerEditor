/********************************************************************************
** Form generated from reading UI file 'processordialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROCESSORDIALOG_H
#define UI_PROCESSORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ProcessorDialog
{
public:
    QComboBox *operationComboBox;
    QTextEdit *num1TextEdit;
    QTextEdit *num2TextEdit;
    QPushButton *confirmButton;

    void setupUi(QDialog *ProcessorDialog)
    {
        if (ProcessorDialog->objectName().isEmpty())
            ProcessorDialog->setObjectName("ProcessorDialog");
        ProcessorDialog->resize(962, 541);
        operationComboBox = new QComboBox(ProcessorDialog);
        operationComboBox->setObjectName("operationComboBox");
        operationComboBox->setGeometry(QRect(360, 260, 171, 71));
        num1TextEdit = new QTextEdit(ProcessorDialog);
        num1TextEdit->setObjectName("num1TextEdit");
        num1TextEdit->setGeometry(QRect(170, 260, 104, 70));
        num2TextEdit = new QTextEdit(ProcessorDialog);
        num2TextEdit->setObjectName("num2TextEdit");
        num2TextEdit->setGeometry(QRect(630, 260, 104, 70));
        confirmButton = new QPushButton(ProcessorDialog);
        confirmButton->setObjectName("confirmButton");
        confirmButton->setGeometry(QRect(650, 440, 201, 51));

        retranslateUi(ProcessorDialog);

        QMetaObject::connectSlotsByName(ProcessorDialog);
    } // setupUi

    void retranslateUi(QDialog *ProcessorDialog)
    {
        ProcessorDialog->setWindowTitle(QCoreApplication::translate("ProcessorDialog", "Dialog", nullptr));
        confirmButton->setText(QCoreApplication::translate("ProcessorDialog", "Run", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProcessorDialog: public Ui_ProcessorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROCESSORDIALOG_H
