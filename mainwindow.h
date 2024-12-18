#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "layoutEditorWidget/src/layereditorwidget.h"
#include "UndoRedoManager.h"


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

    void on_actionRedo_triggered();
    void on_actionUndo_triggered();

    void onPanToolClicked();
    void onDrawToolClicked();
    void onDrawStraightToolClicked();
    void onMoveToolClicked();
    void onEraseToolClicked();

    void on_processorButton_clicked();

    void on_selectLayer_clicked();
    void on_addNewLayer_clicked();
    void on_copyLayer_clicked();
    void on_deleteLayer_clicked();

    void on_autoSaveCheckBox_stateChanged(int state);


private:
    void updateLayerNames(const std::vector<std::string>& newLayerNames);

private:
    Ui::MainWindow *ui;
    LayerEditorWidget* layerEditorWidget;
    UndoRedoManager* manager;
    QString filename;

};
#endif // MAINWINDOW_H
