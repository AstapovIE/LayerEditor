#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

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
