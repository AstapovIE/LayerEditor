#ifndef PROCESSORDIALOG_H
#define PROCESSORDIALOG_H

#include <QDialog>

namespace Ui {
class ProcessorDialog;
}

class ProcessorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessorDialog(QWidget *parent = nullptr);
    ~ProcessorDialog();

    double getNum1() const;
    double getNum2() const;
    QString getOperation() const;

private slots:
    void on_confirmButton_clicked();

private:
    Ui::ProcessorDialog *ui;
};

#endif // PROCESSORDIALOG_H
