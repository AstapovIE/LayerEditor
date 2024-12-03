#ifndef UNDOREDOMANAGER_H
#define UNDOREDOMANAGER_H
#include <QMainWindow>
#include <QTableWidget>
#include <QDialog>
#include <QDebug>
#include <QGridLayout>
#include <QMainWindow>
#include <QSqlTableModel>
#include <QFile>
#include <QTextStream>

#include <databasecontroller.h>





class UndoRedoManager:public QObject
{
    Q_OBJECT
public:
    UndoRedoManager();
    QString undo();
    QString redo();
    void clearHistory();
    void doChanges(QString path);

private:
    int currentStage = 0;
    DatabaseController *db;
    QString saveFile(QByteArray byteArray);
};


#endif // UNDOREDOMANAGER_H
