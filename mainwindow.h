#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "layoutEditorWidget/src/layereditorwidget.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleToolSelection(ToolType tool);
    void on_actionNewFile_triggered();
    void on_actionOpenFile_triggered();
    void on_actionSaveFile_triggered();

    void on_actionredo_triggered();
    void on_actionundo_triggered();

    void onSelectToolClicked();
    void onDrawToolClicked();
    void onMoveToolClicked();
    void onEraseToolClicked();

    void on_processorButton_clicked();

    void on_addNewLayer_clicked();
    void on_copyLayer_clicked();
    void on_deleteLayer_clicked();

    void on_autoSaveCheckBox_stateChanged(int state);


private:
    Ui::MainWindow *ui;
    LayerEditorWidget* layerEditorWidget;
    QString filename; // Переменная для хранения имени текущего файла

};
#endif // MAINWINDOW_H
