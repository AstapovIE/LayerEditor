#include "databasecontroller.h"
#include "database.h"

DatabaseController::DatabaseController()
{
    this->connection();
}

int DatabaseController::connection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("layers.db");

    if (db.open())
    {
        qInfo() << "Database is open!";
        model = new QSqlTableModel(NULL,db);
        return 0;
    }
    else {
        qInfo() << "Can not open database! "<<db.lastError().databaseText();
        return -1;
    }


}

//InfoAboutElement DatabaseController::select(Table table,int id)
//{
//    switch (table) {
//    case AIRPLANS:
//        return airplanTable->select(id);
//        break;
//    case ZRK:
//        return zrkTable->select(id);
//        break;
//    default:
//        return InfoAboutElement();
//        break;
//    }
//}


//QVector<InfoAboutElement> DatabaseController::select_all(Table table)
//{
//    QVector<InfoAboutElement> ans;
//    switch (table) {
//    case AIRPLANS:
//        ans = airplanTable->select_all();
//        break;
//    case ZRK:
//        ans = zrkTable->select_all();
//        break;
//    case ROCKET:
//        rocketTable->select_all();
//        break;
//    default:
//        break;
//    }
//    return ans;
//}


QSqlDatabase DatabaseController::return_db()
{
    return db;
}

int DatabaseController::closing()
{
     db.close();
     qInfo() << "Database is closed!";

     return 0;
}
