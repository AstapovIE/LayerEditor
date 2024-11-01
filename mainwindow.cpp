#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QToolBar>
#include <QPushButton>
#include <QFileDialog>
#include "processordialog.h" // Измените на новый заголовок
#include <QProcess>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // LayerEditorWidget = new QWidget(this);

    // QPushButton *showLayerPack = new QPushButton(LayerEditorWidget);
    // showLayerPack->setText("click");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNewFile_triggered()
{
    QMessageBox *msg = new QMessageBox;
    msg->setText("Select new file");
    msg->exec();
}


void MainWindow::on_actionOpenFile_triggered()
{
    if (QMessageBox::question(this, "Confirm", "Are you sure you want to open a file?") == QMessageBox::Yes) {
        QString filename = QFileDialog::getOpenFileName(this, "Open File");
        // Логика открытия файла
        QMessageBox::information(this, "Open File", "File opened: " + filename);
    }
}


void MainWindow::on_actionSaveFile_triggered()
{
    if (QMessageBox::question(this, "Confirm", "Are you sure you want to save the file?") == QMessageBox::Yes) {
        QString filename = QFileDialog::getSaveFileName(this, "Save File");
        // Логика сохранения файла
        QMessageBox::information(this, "Save File", "File saved: " + filename);
    }
}


void MainWindow::on_actionredo_triggered()
{
    QMessageBox *msg = new QMessageBox;
    msg->setText("did redo");
    msg->exec();

}



void MainWindow::on_processorButton_clicked()
{
    ProcessorDialog dialog(this); // Измените название класса здесь

    if (dialog.exec() == QDialog::Accepted) {
        double num1 = dialog.getNum1();
        double num2 = dialog.getNum2();
        QString operation = dialog.getOperation();

        // Запуск processor.exe с параметрами
        QProcess process;

        //QString program = "../../processor.exe"; //если файл в исходниках
        QString program = "./fake_processor.exe"; // если файл в билде..

        QStringList arguments;
        arguments << operation << QString::number(num1) << QString::number(num2);

        process.start(program, arguments);
        process.waitForFinished();

        QString output(process.readAllStandardOutput());
        QMessageBox::information(this, "Result", output);
    }
}

