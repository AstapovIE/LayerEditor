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
    , layerEditorWidget{new LayerEditorWidget(this)}
{
    ui->setupUi(this);

    ui->editorWidgetLayout->addWidget(layerEditorWidget);

//    QWidget* ui->LayerEditorWidget;
//    ui->layout()->addWidget(layerEditorWidget);
//    ui->LayerEditorWidget->;

//    setCentralWidget(layerEditorWidget);
    // QPushButton *showLayerPack = new QPushButton(LayerEditorWidget);
    // showLayerPack->setText("click");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete layerEditorWidget;
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
    ProcessorDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString sourceFileName = dialog.getSourceFileName();
        QString layer1 = dialog.getLayer1Name();
        QString layer2 = dialog.getLayer2Name();
        QString layer3 = dialog.getLayer3Name();
        QString operation = dialog.getOperation();
        QString outputFile = dialog.getOutputFileName();
        double size = dialog.getSize();

        // Формирование аргументов для запуска process
        QStringList arguments;
        arguments << sourceFileName << layer1 << operation;

        if (operation == "unite" || operation == "intersect" || operation == "subtract") {
            arguments << layer2 << "to" << layer3;
        } else if (operation == "expand" || operation == "shrink") {
            arguments << QString::number(size) << "to" << layer2;
        } else if (operation == "rename_layer") {
            arguments << layer2 << layer3;
        } else if (operation == "copy_layer") {
            arguments << "to" << layer2;
        }

        if (!outputFile.isEmpty()) {
            arguments << "-o" << outputFile;
        }

        QProcess process;

        QString program = "../../fake_processor.exe"; //если файл в исходниках
        // QString program = "./fake_processor.exe"; // если файл в билде..
        //QString program = "./processor"; // Укажите путь к вашему процессору
        process.start(program, arguments);
        process.waitForFinished();

        QString output(process.readAllStandardOutput());
        QString error(process.readAllStandardError());

        if (process.exitCode() == 0) {
            QMessageBox::information(this, "Result", output);
        } else {
            QMessageBox::critical(this, "Error", error);
        }
    }
}




// void MainWindow::on_processorButton_clicked()
// {
//     ProcessorDialog dialog(this); // Измените название класса здесь

//     if (dialog.exec() == QDialog::Accepted) {
//         double num1 = dialog.getNum1();
//         double num2 = dialog.getNum2();
//         QString operation = dialog.getOperation();

//         // Запуск processor.exe с параметрами
//         QProcess process;

//         QString program = "../../fake_processor.exe"; //если файл в исходниках
//         // QString program = "./fake_processor.exe"; // если файл в билде..

//         QStringList arguments;
//         arguments << operation << QString::number(num1) << QString::number(num2);

//         process.start(program, arguments);
//         process.waitForFinished();

//         QString output(process.readAllStandardOutput());
//         QMessageBox::information(this, "Result", output);
//     }
// }

