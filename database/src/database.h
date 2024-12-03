#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QtSql>
#include <QVector>


//enum DATABASE{LAYERS = "layers.db"};

enum Table{FILE_TABLE,HISTORY_TABLE};


class FileDTO
{
public:
    int fileId;
    QString name;
    QByteArray file;
};

class HistoryDTO
        //:FileDTO
{
public:
    int id;
    int fileId;
    QByteArray file;
    int stage;
};


class  BaseTable
{
public:
    //virtual InfoAboutElement select(int id)=0;
    //virtual QVector<InfoAboutElement> select_all()=0;
};

class FileTable:public BaseTable
{
public:
    FileDTO select(int id);
    //QVector<InfoAboutElement> select_all();
};

class HistoryTable:public BaseTable
{
public:
    HistoryDTO select(int fileId,int stage);
    void insert(int fileId,int stage,QByteArray file);
    void clearAll();
    //QVector<InfoAboutElement> select_all();
};




#endif // DATABASE_H
