//#ifndef DATABASE_H
//#define DATABASE_H

//#include <QSqlDatabase>
//#include <QSqlError>
//#include <QDebug>
//#include <QtSql>
//#include <QVector>
//#include "../../objects/src/point.h"

//enum Table{AIRPLANS,ZRK,ROCKET,SPRITE};//will be more tables

//class InfoAboutRocket
//{
//public:
//    int id;
//    QString name;
//    int speed;
//    int damage;
//    int distance;
//    int guidance_type;
//    int sprite_id;

//};

//class InfoAboutElement
//{
//public:
//    int id;
//    Table type;
//    QString name;
//    int speed;
//    int mass;
//    int health;
//    int distance;
//    int rocket;
//    int sprite_id;

//};
//class Packet
//{
//public:
//    Packet(int _health,QString _model,int _x,int _y):health{_health},model{_model},x{_x},y{_y} {};
//    int health;
//    QString model;
//    int x;
//    int y;
//};

//class PacketToEngine_sams:public Packet
//{
//public:
//    PacketToEngine_sams(int _health, QString _model, int _x, int _y, int _battery, int _distance):Packet( _health, _model, _x, _y),
//        battery{_battery},distance{_distance} {};

//    int battery;
//    int distance;


//};

//class PacketToEngine_planes:public Packet
//{
//public:
//    PacketToEngine_planes(int _health,QString _model,int _x,int _y,int _speed,int _angle,std::shared_ptr<QVector<std::shared_ptr<Point> > > _tragectory):
//        Packet( _health, _model, _x, _y),speed{_speed},angle{_angle},tragectory{_tragectory} {};

//    int speed;
//    int angle;

//    std::shared_ptr<QVector<std::shared_ptr<Point> > > tragectory;
//};

//class  BaseTable
//{
//public:
//    //virtual InfoAboutElement select(int id)=0;
//    //virtual QVector<InfoAboutElement> select_all()=0;
//};

//class AirplansTable:public BaseTable
//{
//public:
//    InfoAboutElement select(int id);
//    QVector<InfoAboutElement> select_all();
//};

//class ZRKTable:public BaseTable
//{
//public:
//    InfoAboutElement select(int id);
//    QVector<InfoAboutElement> select_all();
//};

//class ROCKETTable
//{
//public:
//    InfoAboutRocket select(int id);
//    QVector<InfoAboutRocket> select_all();
//};

//class SpriteTable
//{
//public:
//    QString select(int id);
//};



//#endif // DATABASE_H