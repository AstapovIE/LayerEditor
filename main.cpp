#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("LayerEditor");

    w.show();
    return a.exec();
}

//#include <QApplication>
//#include <QMainWindow>
//#include "layereditorwidget.h"

//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);

//    QMainWindow mainWindow;
//    LayerEditorWidget *editor = new LayerEditorWidget(&mainWindow);

//    mainWindow.setCentralWidget(editor);
//    mainWindow.resize(400, 300);
//    mainWindow.show();

//    return app.exec();
//}


// Отлично! Теперь надо реализовать список слоёв. Существует класс:
// Class Converter - это класс, который отвечает за работу с файлами. Он подгружает данные из файла, вносит в них изменения. А так же отвечает за создание объектов, опираясь на данные, считанные из файла. После внесения изменений в файл сохраняет изменения в нужный файл.
// Поле
// LayerPack layerpack - экземпляр класса LayerPack, который будет заполнен внутри функции convertJson. Фактически контейнер, содержащий объекты классов Layer и Polygon

// Методы

// void convertJson(const std::string& filename) - функция считывает имя файла топологии , после чего подгружает его данные. Проверяет, что файл существует, иначе создаёт пустой, затем парсит файл json, далее начинает создавать объекты классов Polygon, Layer после чего сохраняет их в LayerPack, связанный с ключом названием файла, для дальнейшего хранения в нем и доступа через методы класса LayerPack. Используется внешняя библиотека RapidJson. В случае проблем с файлом выкидывает сообщение об ошибке.

// void saveToJson(const std::string& filename) -  функция обратная convertJson. Уже существующие объекты классов Polygon, Layer, лежащие в  LayerPack она преобразовывает в текстовую информацию, которую записывает в файл, если он существует, если же его нет, то создаёт новый пустой файл и записывает туда. Так же происходит проверка перед загрузкой на правильный тип данных, если было несовпадение, то выбрасывает сообщение об ошибке. Используется внешняя библиотека RapidJson.

// LayerPack& getLayerPack() -- это функция возвращающая экземпляр класса LayerPack layerpack, который был создан в void convertJson(const std::string& filename) и заполнен данными, считанными из файла.Сейчас при вызове actionOpenFile  происходит следующее:
// ```void MainWindow::on_actionOpenFile_triggered()
// {
//     if (QMessageBox::question(this, "Confirm", "Are you sure you want to open a file?") == QMessageBox::Yes) {
//         QString filename = QFileDialog::getOpenFileName(this, "Open File");
//         // Логика открытия файла
//         QMessageBox::information(this, "Open File", "File opened: " + filename);
//     }
// }```
// Требуется расширить функционал:
// Нужно в mainwindow по вызову on_actionOpenFile_triggered:
// 0) создать объект класс Converter
// 1) передать открытый файл в void convertJson(const std::string& filename)
// 2) затем вызвать getLayerPack() и результат присвоить в переменную layer_pack (теперь тут лежит экземпляр класса LayerPack)
// 3) у этого экземпляра вызвать метод layer_pack.get_pack_names(), который вернет список строк с названиями слоёв
// 4) Отобразить имена из предыдущего пункта с помощью QListWidget, который уже есть в mainwindow.ui (т.е. можно так обращаться ui->listOfLayers), имена надо центрировать
