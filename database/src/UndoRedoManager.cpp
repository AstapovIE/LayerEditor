#include "UndoRedoManager.h"

UndoRedoManager::UndoRedoManager()
{
    db = new DatabaseController;

}

void UndoRedoManager::doChanges(QString path)
{
    QFile file(path);
    if (!file.exists()) {
        qDebug() << "Файл не существует:" << path;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Ошибка открытия файла:" << file.errorString();
    }

    QByteArray fileData = file.readAll();
    file.close();

    db->historyTable->insert(1,currentStage,fileData);//TODO: do fileId

    if (QFile::remove(path)) {
        qDebug() << "Файл удален:" << path;
    } else {
        qDebug() << "Ошибка удаления файла:" << path;
    }
}

QString UndoRedoManager::undo()
{
    HistoryDTO selection = db->historyTable->select(1,currentStage);
    currentStage++;
    saveFile(selection.file);
    return defaultPath;

}

QString UndoRedoManager::redo()
{
    HistoryDTO selection = db->historyTable->select(1,currentStage);
    currentStage--;
    saveFile(selection.file);
    return defaultPath;
}


void UndoRedoManager::saveFile(QByteArray byteArray)
{
    QFile file(defaultPath);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Ошибка открытия файла:" << file.errorString();
    }

    qint64 bytesWritten = file.write(byteArray);

    if (bytesWritten == -1) {
        qDebug() << "Ошибка записи в файл:" << file.errorString();
    } else {
        qDebug() << "Записано байт:" << bytesWritten;
    }

    file.close();
    qDebug() << "Файл успешно сохранен:" << defaultPath;

}
