#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QToolBar>
#include <QPushButton>
#include <QFileDialog>
#include "processordialog.h" // Измените на новый заголовок
#include <QProcess>
#include <QDebug>
#include <QFileInfo>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , layerEditorWidget(new LayerEditorWidget(this))
{
    ui->setupUi(this);

    ui->editorWidgetLayout->addWidget(layerEditorWidget);

    // Добавление кнопок в toolBar
    QAction *selectAction = new QAction("Select", this);
    QAction *drawAction = new QAction("Draw", this);
    QAction *moveAction = new QAction("Move", this);
    QAction *eraseAction = new QAction("Erase", this);

    ui->toolBar->addAction(selectAction);
    ui->toolBar->addAction(drawAction);
    ui->toolBar->addAction(moveAction);
    ui->toolBar->addAction(eraseAction);

    // Связь сигналов с обработчиками
    connect(selectAction, &QAction::triggered, this, &MainWindow::onSelectToolClicked);
    connect(drawAction, &QAction::triggered, this, &MainWindow::onDrawToolClicked);
    connect(moveAction, &QAction::triggered, this, &MainWindow::onMoveToolClicked);
    connect(eraseAction, &QAction::triggered, this, &MainWindow::onEraseToolClicked);

    //connect(ui->autoSaveCheckBox, &QCheckBox::checkStateChanged, this, &MainWindow::on_autoSaveCheckBox_stateChanged);
    connect(ui->autoSaveCheckBox, &QCheckBox::stateChanged, this, &MainWindow::on_autoSaveCheckBox_stateChanged);



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
        //QString filename = QFileDialog::getOpenFileName(this, "Open File");
        filename = QFileDialog::getOpenFileName(this, "Open File");
        qDebug() << "selected file: " << filename;

        if (!filename.isEmpty()) {
            //layerEditorWidget->setFilename(filename.toStdString());

            // Получить список строк с названиями слоёв
            // std::vector<std::string> layer_names = layerEditorWidget->getLayersNames();
            std::vector<std::string> layer_names = {"layer1", "layer2", "layer3", "layer4"};

            // Очистить QListWidget перед добавлением новых элементов
            ui->listOfLayers->clear();

            // Добавить элементы в QListWidget и центрировать их
            for (const std::string& name : layer_names) {
                QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(name));
                item->setTextAlignment(Qt::AlignCenter);
                ui->listOfLayers->addItem(item);
            }
            QFileInfo fileInfo(filename);
            QMessageBox::information(this, "Open File", "File " + fileInfo.fileName() + " opened");
        } else {
            QMessageBox::warning(this, "Open File", "No file selected.");
        }
    }
}


void MainWindow::on_actionSaveFile_triggered()
{
    if (QMessageBox::question(this, "Confirm", "Are you sure you want to save the file?") == QMessageBox::Yes) {
        //сохранение изменений
        //layerEditorWidget->saveFile(filename);
        //QString filename = QFileDialog::getSaveFileName(this, "Save File");
        // Логика сохранения файла

        QFileInfo fileInfo(filename);
        QMessageBox::information(this, "Save File", "File " + fileInfo.fileName() + " saved: ");
    }
}


void MainWindow::on_actionredo_triggered()
{
    QMessageBox *msg = new QMessageBox;
    msg->setText("did redo");
    msg->exec();

}

void MainWindow::on_actionundo_triggered()
{
    QMessageBox *msg = new QMessageBox;
    msg->setText("did undo");
    msg->exec();
}


void MainWindow::onSelectToolClicked() {
    handleToolSelection(SELECT);
}

void MainWindow::onDrawToolClicked() {
    handleToolSelection(DRAW);
}

void MainWindow::onMoveToolClicked() {
    handleToolSelection(MOVE);
}

void MainWindow::onEraseToolClicked() {
    handleToolSelection(ERASE);
}

void MainWindow::handleToolSelection(ToolType tool) {
    // Проверка, что выбран элемент списка
    QListWidgetItem *currentItem = ui->listOfLayers->currentItem();
    if (currentItem) {
        std::string layerName = currentItem->text().toStdString();
        layerEditorWidget->setSelectedLayer(layerName);
        layerEditorWidget->setCurrentTool(tool);
    } else {
        QMessageBox::warning(this, "Selection Error", "Please select a layer first.");
    }
}

void MainWindow::on_processorButton_clicked()
{
    ProcessorDialog dialog(this);

    // Вывод только имени текущего файла
    QFileInfo fileInfo(filename);
    QString shortFileName = fileInfo.fileName();
    qDebug() << "Current filename:" << shortFileName;
    dialog.setSourceFileName(shortFileName);

    //сохранение изменений
    //layerEditorWidget->saveFile(filename);

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

        //QString program = "../../processDialog/src/fake_processor.exe"; //если файл в исходниках
        // QString program = "./fake_processor.exe"; // если файл в билде..
        QString program = "../../fake_processor.exe";
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

void MainWindow::on_addNewLayer_clicked()
{
    // Создание нового слоя с именем по умолчанию
    static int layerCounter = 1;
    QString newLayerName = "NewLayer" + QString::number(layerCounter++);
    QListWidgetItem *newItem = new QListWidgetItem(newLayerName);
    newItem->setTextAlignment(Qt::AlignCenter);
    ui->listOfLayers->addItem(newItem);

    // Вызов метода добавления слоя
    //layerEditorWidget->addLayer(newLayerName.toStdString());

    // Показ сообщения
    QMessageBox::information(this, "Layer Added", "New layer '" + newLayerName + "' has been added.");
}

void MainWindow::on_copyLayer_clicked()
{
    QListWidgetItem *selectedItem = ui->listOfLayers->currentItem();

    if (selectedItem)
    {
        QString layerName = selectedItem->text();

        // Создание нового слоя с именем по умолчанию
        static int layerCounter = 1;
        QString copyLayerName = "CopyLayer" + QString::number(layerCounter++);
        QListWidgetItem *newItem = new QListWidgetItem(copyLayerName);
        newItem->setTextAlignment(Qt::AlignCenter);
        ui->listOfLayers->addItem(newItem);

        // Вызов метода копирования слоя
        //layerEditorWidget->copyLayer(layerName.toStdString(), copyLayerName.toStdString());

        // Показ сообщения
        QMessageBox::information(this, "Layer Copied", "Layer '" + copyLayerName + "' has been added.");
    }
    else
    {
        // Показ сообщения, если не выбран слой
        QMessageBox::warning(this, "Copy Layer", "Select a layer to copy.");
    }
}

void MainWindow::on_deleteLayer_clicked()
{
    QListWidgetItem *selectedItem = ui->listOfLayers->currentItem();

    if (selectedItem)
    {
        QString layerName = selectedItem->text();

        // Удаление из списка
        delete selectedItem;

        // Вызов метода удаления слоя
        //layerEditorWidget->deleteLayer(layerName.toStdString());

        // Показ сообщения
        QMessageBox::information(this, "Layer Deleted", "Layer '" + layerName + "' has been deleted.");
    }
    else
    {
        // Показ сообщения, если не выбран слой
        QMessageBox::warning(this, "Delete Layer", "Select a layer to delete.");
    }
}

void MainWindow::on_autoSaveCheckBox_stateChanged(int state)
{
    if (state == Qt::Checked) {
        // Включение режима авто-сохранения
        qDebug() << "autoSaveMode ON";
        //layerEditorWidget->autoSaveMode(true);
    } else {
        // Отключение режима авто-сохранения
        qDebug() << "autoSaveMode OFF";
        //layerEditorWidget->autoSaveMode(false);
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













