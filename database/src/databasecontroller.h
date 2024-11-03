#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H
#include <QMainWindow>
#include <QTableWidget>
#include <QDialog>
#include <QDebug>
#include <QGridLayout>
#include <QMainWindow>
#include <QSqlTableModel>


#include <database.h>





class DatabaseController:public QObject
{
    Q_OBJECT
public:
    DatabaseController();
    QSqlDatabase return_db();
    int connection();
    int closing();
    //InfoAboutElement select(Table table,int id);//where and what
    //QVector<InfoAboutElement> select_all(Table table);
    QSqlTableModel *model = nullptr;
    FileTable* fileTable;
    HistoryTable* historyTable;


private:
    QSqlDatabase db;
    int currentRow;
};


#endif // DATABASECONTROLLER_H
