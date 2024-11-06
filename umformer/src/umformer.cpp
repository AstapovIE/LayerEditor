#include "umformer.h"


void Converter::loadJson(const std::string& filename) {
    // Чтение JSON-файла и создание объектов
    /*std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << filename << std::endl;
        return;
    }*/
    FILE* fp = fopen(filename.c_str(), "rb"); //r for non Windows
    if (!fp){
        std::cerr << "Error. Can't open a file " << filename << std::endl;
        return;
    }
     /*std::stringstream buffer;
    buffer << fp.rdbuf();
    std::string jsonString = buffer.str();*/
    char readBuffer[65536]; 
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document document; 
    document.ParseStream(is);
    if (document.HasParseError()) {
        std::cerr << "Parse error JSON: " << rapidjson::GetParseError_En(document.GetParseError()) << std::endl;
        return;
    }
    // Check layers
    assert(document.IsObject());
    const Value& b = document["layers"];
    assert(b.IsArray()); 
for (SizeType i = 0; i < b.Size(); i++) {
    assert(b[i].IsObject());
    assert(b[i].HasMember("name"));
    
    // Get layer name
    Layer layer{b[i]["name"].GetString()};
    
    assert(b[i].HasMember("polygons"));
    const Value& pol = b[i]["polygons"];
    assert(pol.IsArray());
    
    // Check all polygon in this layer
    for (SizeType j = 0; j < pol.Size(); j++) {
        assert(pol[j].IsObject());
        assert(pol[j].HasMember("cords"));
        
        const Value& cordar = pol[j]["cords"];
        assert(cordar.IsArray());
        
        Polygon polygon;

        // Coords to polygon
        for (const Value& cord : cordar.GetArray()) {
            Point dot{cord[0].GetDouble(), cord[1].GetDouble()};
            polygon.append(dot);
        }

        assert(pol[j].HasMember("holes"));
        const Value& holear = pol[j]["holes"];
        assert(holear.IsArray());

        // Holes array to current polygon
        for (const Value& holee : holear.GetArray()) {
            Hole hole;
            for (const Value& pt : holee.GetArray()) {
                Point point{pt[0].GetDouble(), pt[1].GetDouble()};
                hole.append(point); // Add point to hole
            }
            polygon.add_hole(hole); // Add hole to polygon
        }

        // add polygon to layer
        layer.append(polygon);
    }

    //Add to layer
    this->layerpack.append_layer(layer);
}
    fclose(fp);   
};
void Converter::saveToJson(const std::string& filename){
    // Write to file, if no file - make one
    rapidjson::Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();
    Value layersObject(kArrayType);//empty array for layers
    //unordered_map<std::string, Layer> layerMap =  this->layerpack.get_layers_by_name();//key name - layer
    vector<Layer> layerIndexs = this->layerpack.get_layers();//vector of layers
    int lsize = static_cast<int>(layerIndexs.size());
    for(int i=0; i<lsize;i++)//to add all layers
    {
        Document::AllocatorType& layerAllocator = document.GetAllocator();
        const Layer& layer = layerIndexs[i];//layer with index i
        string names = layer.get_name();//layer name with index i
        Value layerObject(kObjectType);//making layer
        layerObject.AddMember("name",Value(names.c_str(), layerAllocator),layerAllocator);
        layerObject.AddMember("level",Value(to_string(i).c_str(), layerAllocator),layerAllocator);//maybewrong
        vector<Polygon> polygonIndex = layer.get_polygons();//polygons in layer i
        Value polygonsArray(rapidjson::kArrayType);//array to store polygons
        int psize = static_cast<int>(polygonIndex.size());
        for (int j=0;j<psize;j++)
        {
            Document::AllocatorType& polyAllocator = document.GetAllocator();
            const Polygon& polygon = polygonIndex[j];//polygon in layer
            Value polygonObject(kObjectType);//polygon object
            //polygon.ravel() VarientType
            vector<Point> pointIndex = polygon.get_vertices();
            Value pointArray(rapidjson::kArrayType);//points array
            int ppsize = static_cast<int>(pointIndex.size());
            for (int k=0;k<ppsize;k++)
            {
                Document::AllocatorType& pointAllocator = document.GetAllocator();
                Value coordArray(rapidjson::kArrayType);//coord array
                const Point& point = pointIndex[k];//coord of polygon
                unordered_map<string,double> pointCoords = point.ravel();//"x"-x,"y"-y
                double x = pointCoords.at("x");//add as x
                double y = pointCoords.at("y");// add as y
                coordArray.PushBack(x,pointAllocator);//[x]
                coordArray.PushBack(y,pointAllocator);//[x,y]
                pointArray.PushBack(coordArray,pointAllocator);//[[x,y]]
            }
            polygonObject.AddMember("cords",pointArray,polyAllocator);
            pointArray.SetArray();
            Value holesArrays(kArrayType);//holes=[]
            vector<Hole> holeIndex = polygon.get_holes();//holes in polygon
            int hsize = static_cast<int>(holeIndex.size());
            for (int k=0;k<hsize;k++)
            {
                Document::AllocatorType& holesAllocator = document.GetAllocator();
                Value holeOneArray(kArrayType);//hole=[[a]]
                const Hole& hole = holeIndex.at(k);
                vector<Point> pointHole = hole.get_vertices();
                int msize = static_cast<int>(pointHole.size());
                for(int m=0; m<msize;m++)
                {
                    Document::AllocatorType& holeAllocator = document.GetAllocator();
                    Value holeCoord(kArrayType);
                    const Point& point = pointHole[m];//coord of hole
                    unordered_map<string,double> holeCoords = point.ravel();//"x"-x,"y"-y
                    double x = holeCoords.at("x");//add as x
                    double y = holeCoords.at("y");
                    holeCoord.PushBack(x,holeAllocator);//[x]
                    holeCoord.PushBack(y,holeAllocator);//[x,y]
                    holeOneArray.PushBack(holeCoord,holeAllocator);//[]
                }
                holesArrays.PushBack(holeOneArray,holesAllocator);
            };
            polygonObject.AddMember("holes",holesArrays,polyAllocator);
            polygonsArray.PushBack(polygonObject,polyAllocator);
        };
    layerObject.AddMember("polygons",polygonsArray,layerAllocator);
    layersObject.PushBack(layerObject,layerAllocator);
    }
    document.AddMember("layers",layersObject,allocator);
    //Write to file
    FILE* fp = fopen(filename.c_str(), "wb");// w for Non-windows

    if(!fp)
    {
         std::cerr << "Failed to open file: " << filename << std::endl;
         return;
    }

    char writeBuffer[65536];
    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    PrettyWriter<FileWriteStream> writer(os);
    //writer.SetIndent(' ',2);//try this one'\n' 
    document.Accept(writer);
    fclose(fp);
    };
    
LayerPack& Converter::getLayerPack(){
        return this->layerpack;
    }

/*int main()
{
    Converter converter;
    converter.loadJson("layout.json");
    converter.saveToJson("liter.json");
    return 0;
};*/