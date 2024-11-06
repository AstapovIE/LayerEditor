#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <UndoRedoManager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("LayerEditor");
    w.show();

    UndoRedoManager manager = UndoRedoManager();
    return a.exec();
}

