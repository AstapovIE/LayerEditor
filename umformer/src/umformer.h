#ifndef UMFORMER_H
#define UMFORMER_H


#include <iostream>
#include <fstream>
#include "Entity.h"
#include "rapidjson-1.1.0/include/rapidjson/filereadstream.h"
#include "rapidjson-1.1.0/include/rapidjson/filewritestream.h"
#include "rapidjson-1.1.0/include/rapidjson/document.h"
#include "rapidjson-1.1.0/include/rapidjson/prettywriter.h"
#include "rapidjson-1.1.0/include/rapidjson/stringbuffer.h"
#include "rapidjson-1.1.0/include/rapidjson/error/en.h"


using namespace std;
using namespace rapidjson;

#define BUFF_SIZE 65536

// Класс для разбора JSON-файла
class Converter {
    LayerPack layerpack;
public:
    // Метод для загрузки и разбора JSON
    Converter() {};
    void loadJson(const std::string& filename);
    void saveToJson(const std::string& filename);
    LayerPack& getLayerPack();
};


#endif 
