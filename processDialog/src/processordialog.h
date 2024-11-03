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

    QString getSourceFileName() const;
    QString getLayer1Name() const;
    QString getLayer2Name() const;
    QString getLayer3Name() const;
    QString getOperation() const;
    QString getOutputFileName() const;
    double getSize() const;
public:
    void setSourceFileName(const QString &filename);

public slots:
    void on_operationComboBox_currentIndexChanged(const QString &operation);
    void on_confirmButton_clicked();

private:
    Ui::ProcessorDialog *ui;
};

#endif // PROCESSORDIALOG_H
