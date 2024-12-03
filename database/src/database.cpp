#include "database.h"

//FileDTO::FileDTO(int _fileId, QString _name, QByteArray _file):fileId{_fileId},name{_name},file{_file}{}

//HistoryDTO::HistoryDTO(int _fileId, QString _name, QByteArray _file, int _stage):fileId{_fileId},name{_name},file{_file},stage{_stage}{}

FileDTO FileTable::select(int id)
{
    QSqlQuery my_query;
    my_query.prepare("SELECT * FROM FILE WHERE file_id = (:id)");
    my_query.bindValue(":id", id);
    my_query.exec();

    QSqlRecord rec = my_query.record();
    my_query.first();

    QString name = my_query.value(rec.indexOf("name")).toString();
    QByteArray file = my_query.value(rec.indexOf("file")).toByteArray();


    qInfo() << "Row with id =" <<id<<"Name = "<< name;
    FileDTO fileDTO = {id, name, file};
    return fileDTO;
}


HistoryDTO HistoryTable::select(int fileId,int stage)
{
    QSqlQuery my_query;
    my_query.prepare("SELECT * FROM HISTORY WHERE file_id = (:fileId) and stage = (:stage)");
    my_query.bindValue(":fileId", fileId);
    my_query.bindValue(":stage", stage);
    my_query.exec();

    QSqlRecord rec = my_query.record();
    my_query.first();

    int id = my_query.value(rec.indexOf("id")).toInt();
    QByteArray file = my_query.value(rec.indexOf("file")).toByteArray();


    //qInfo() << "Row with id =" <<id<<"Name = "<< name <<"Speed ="<< speed <<"Mass ="<< mass;
    HistoryDTO historyDTO = {id,fileId,file, stage};
    return historyDTO;
}

void HistoryTable::insert(int fileId,int stage,QByteArray file)
{
    QSqlQuery query;
    query.prepare("INSERT INTO HISTORY(file_id,file,stage) VALUES (:file_id,:file,:stage)");
    query.bindValue(":file_id", fileId);
    query.bindValue(":file", file);
    query.bindValue(":stage", stage);

    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
    } else {
        qDebug() << "Файл успешно сохранен в БД";
    }
}

void HistoryTable::clearAll()
{
    QSqlQuery my_query;
    my_query.prepare("DELETE FROM HISTORY");
    my_query.exec();
}




