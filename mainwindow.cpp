#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QToolBar>
#include <QPushButton>
#include <QFileDialog>
#include "processordialog.h"
#include <QProcess>
#include <QDebug>
#include <QFileInfo>
#include <QInputDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , layerEditorWidget(new LayerEditorWidget(this))
{
    ui->setupUi(this);

    ui->editorWidgetLayout->addWidget(layerEditorWidget);

    // Добавление кнопок в toolBar
    QAction *panAction = new QAction("Pan", this);
    QAction *drawAction = new QAction("Draw", this);
    QAction *draw_straightAction = new QAction("Draw Straight", this);
    QAction *moveAction = new QAction("Move", this);
    QAction *eraseAction = new QAction("Erase", this);

    ui->toolBar->addAction(panAction);
    ui->toolBar->addAction(drawAction);
    ui->toolBar->addAction(draw_straightAction);
    ui->toolBar->addAction(moveAction);
    ui->toolBar->addAction(eraseAction);

    // Связь сигналов с обработчиками
    connect(panAction, &QAction::triggered, this, &MainWindow::onPanToolClicked);
    connect(drawAction, &QAction::triggered, this, &MainWindow::onDrawToolClicked);
    connect(draw_straightAction, &QAction::triggered, this, &MainWindow::onDrawStraightToolClicked);
    connect(moveAction, &QAction::triggered, this, &MainWindow::onMoveToolClicked);
    connect(eraseAction, &QAction::triggered, this, &MainWindow::onEraseToolClicked);

    connect(ui->listOfLayers, &QListWidget::itemSelectionChanged, this, &MainWindow::on_selectLayer_clicked);

    connect(ui->autoSaveCheckBox, &QCheckBox::stateChanged, this, &MainWindow::on_autoSaveCheckBox_stateChanged);

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

void MainWindow::updateLayerNames(const std::vector<std::string>& newLayerNames)
{
    ui->listOfLayers->clear();

    for (const std::string& name : newLayerNames) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(name));
        item->setTextAlignment(Qt::AlignCenter);
        ui->listOfLayers->addItem(item);
    }
}


void MainWindow::on_actionOpenFile_triggered()
{
    if (QMessageBox::question(this, "Confirm", "Are you sure you want to open a file?") == QMessageBox::Yes) {
        filename = QFileDialog::getOpenFileName(this, "Open File");
        qDebug() << "selected file: " << filename;

        if (!filename.isEmpty()) {
            layerEditorWidget->setFile(filename.toStdString());

            updateLayerNames(layerEditorWidget->getLayerNames());

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
        layerEditorWidget->saveAll(filename.toStdString());
        //QString filename = QFileDialog::getSaveFileName(this, "Save File");
        // Логика сохранения файла

        QFileInfo fileInfo(filename);
        QMessageBox::information(this, "Save File", "File " + fileInfo.fileName() + " saved: ");
    }
}


void MainWindow::on_actionRedo_triggered() {
    layerEditorWidget->redo();
    updateLayerNames(layerEditorWidget->getLayerNames());
}

void MainWindow::on_actionUndo_triggered() {
    layerEditorWidget->undo();
    updateLayerNames(layerEditorWidget->getLayerNames());
}

void MainWindow::onPanToolClicked() {
    handleToolSelection(PAN);
}

void MainWindow::onDrawToolClicked() {
    handleToolSelection(DRAW);
}

void MainWindow::onDrawStraightToolClicked()
{
    handleToolSelection(DRAW_STRAIGHT);
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
        QString program = "./processor/processor.exe";
        // QString program = "../../fake_processor.exe";
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

void MainWindow::on_selectLayer_clicked()
{
    QListWidgetItem *selectedItem = ui->listOfLayers->currentItem();
    if (selectedItem) {
        QString layerName = selectedItem->text();
        layerEditorWidget->setSelectedLayer(layerName.toStdString());
    }
}

void MainWindow::on_addNewLayer_clicked()
{
    QString newLayerName;
    bool ok = false;

    while (!ok) {
        newLayerName = QInputDialog::getText(this, "Добавить новый слой",
                                             "Введите название слоя:", QLineEdit::Normal, "", &ok);
        if (!ok || newLayerName.isEmpty()) {
            QMessageBox::warning(this, "Неверное имя", "Имя слоя не может быть пустым.");
            return;
        }

        // Проверка на уникальность имени слоя
        bool nameExists = false;
        for (int i = 0; i < ui->listOfLayers->count(); ++i) {
            if (ui->listOfLayers->item(i)->text() == newLayerName) {
                nameExists = true;
                break;
            }
        }

        if (!nameExists) {
            break;
        } else {
            QMessageBox::warning(this, "Имя занято", "Слой с таким именем уже существует. Пожалуйста, выберите другое имя.");
            ok = false;
        }
    }

    // Вызов метода добавления слоя в layerEditorWidget
    layerEditorWidget->addLayer(newLayerName.toStdString());

    updateLayerNames(layerEditorWidget->getLayerNames());

    // Показ сообщения об успешном добавлении слоя
    QMessageBox::information(this, "Слой добавлен", "Новый слой '" + newLayerName + "' успешно добавлен.");
}

void MainWindow::on_copyLayer_clicked()
{
    QListWidgetItem *selectedItem = ui->listOfLayers->currentItem();

    if (selectedItem)
    {
        QString originalLayerName = selectedItem->text();
        QString copyLayerName;
        bool ok;

        while (true) {
            copyLayerName = QInputDialog::getText(this, "Копировать слой",
                                                  "Введите название для копии слоя:", QLineEdit::Normal, "", &ok);

            if (!ok || copyLayerName.isEmpty()) {
                QMessageBox::warning(this, "Неверное имя", "Имя слоя не может быть пустым.");
                return;
            }

            // Проверка на уникальность имени слоя
            bool nameExists = false;
            for (int i = 0; i < ui->listOfLayers->count(); ++i) {
                if (ui->listOfLayers->item(i)->text() == copyLayerName) {
                    nameExists = true;
                    break;
                }
            }

            if (!nameExists) {
                break;
            } else {
                QMessageBox::warning(this, "Имя занято", "Слой с таким именем уже существует. Пожалуйста, выберите другое имя.");
            }
        }

        // Вызов метода копирования слоя в layerEditorWidget
        layerEditorWidget->copyLayer(originalLayerName.toStdString(), copyLayerName.toStdString());

        updateLayerNames(layerEditorWidget->getLayerNames());

        // Показ сообщения об успешном копировании слоя
        QMessageBox::information(this, "Слой скопирован", "Копия слоя '" + copyLayerName + "' успешно добавлена.");
    }
    else
    {
        // Показ сообщения, если не выбран слой
        QMessageBox::warning(this, "Копировать слой", "Выберите слой для копирования.");
    }
}

void MainWindow::on_deleteLayer_clicked()
{
    QListWidgetItem *selectedItem = ui->listOfLayers->currentItem();

    if (selectedItem)
    {
        QString layerName = selectedItem->text();

        // Вызов метода удаления слоя
        layerEditorWidget->deleteLayer(layerName.toStdString());

        updateLayerNames(layerEditorWidget->getLayerNames());

        // Показ сообщения
        QMessageBox::information(this, "Удаление слоя", "Слой '" + layerName + "' удален.");
    }
    else
    {
        // Показ сообщения, если не выбран слой
        QMessageBox::warning(this, "Удаление слоя", "Сначала выберите слой для удаления.");
    }
}

void MainWindow::on_autoSaveCheckBox_stateChanged(int state)
{
    if (state == Qt::Checked) {
        // Включение режима авто-сохранения
        qDebug() << "autoSaveMode ON";
        layerEditorWidget->autoSaveMode(true);
    } else {
        // Отключение режима авто-сохранения
        qDebug() << "autoSaveMode OFF";
        layerEditorWidget->autoSaveMode(false);
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














