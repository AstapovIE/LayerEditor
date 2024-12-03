#include "UndoRedoManager.h"
#include <filesystem>


UndoRedoManager::UndoRedoManager()
{
    db = new DatabaseController;

}

void UndoRedoManager::doChanges(QString path)
{
    QFile file(path);
    if (!file.exists()) {
        qDebug() << "UndoRedoManager: Файл не существует:" << path;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "UndoRedoManager: Ошибка открытия файла:" << file.errorString();
    }

    QByteArray fileData = file.readAll();
    file.close();

    currentStage++;
    db->historyTable->insert(1,currentStage,fileData);//TODO: do fileId

    if (QFile::remove(path)) {
        qDebug() << "UndoRedoManager: Файл удален:" << path;
    } else {
        qDebug() << "UndoRedoManager: Ошибка удаления файла:" << path;
    }
}

QString UndoRedoManager::undo()
{
    currentStage--;
    HistoryDTO selection = db->historyTable->select(1,currentStage);
    return saveFile(selection.file);

}

QString UndoRedoManager::redo()
{
    currentStage++;
    HistoryDTO selection = db->historyTable->select(1,currentStage);
    return saveFile(selection.file);
}


QString UndoRedoManager::saveFile(QByteArray byteArray)
{       
    std::filesystem::path currentSrcFile = __FILE__;
    std::filesystem::path currentDir = currentSrcFile.parent_path().parent_path().parent_path();
    currentDir += "/database/";

    QString f_path = QString::fromStdString(std::string(currentDir.generic_string())  + "DB.json");

    QFile file(f_path);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "UndoRedoManager: Ошибка открытия файла:" << file.errorString();
    }

    qint64 bytesWritten = file.write(byteArray);

    if (bytesWritten == -1) {
        qDebug() << "UndoRedoManager: Ошибка записи в файл:" << file.errorString();
    } else {
        qDebug() << "UndoRedoManager: Записано байт:" << bytesWritten;
    }

    file.close();
    qDebug() << "UndoRedoManager: Файл успешно сохранен:" << f_path;

    return f_path;

}

void UndoRedoManager::clearHistory()
{
    db->historyTable->clearAll();
}
