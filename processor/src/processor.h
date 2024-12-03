#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include "umformer.h"
#include "GeometryOperations.h"
#include "Entity.h"
#include "LayerOperations.h"
#include "Oshmetkov.h"

std::string getOperationName(OperationType type);
bool checkName(const std::string& new_name, std::vector<string> existing_names);

class Processor {
public:
    int doOperationWithLayers(const std::string& first_layer_name, const std::string& second_layer_name, const std::string& result_layer_name, LayerPack& layer_pack, OperationType type);
    int doOperationWithLayers(const std::string& first_layer_name, const std::string& second_layer_name, const std::string& result_layer_name, LayerPack& source_layer_pack, LayerPack& target_layer_pack, OperationType type);


    int expandLayer(const std::string& layer_name, const std::string& result_layer_name, float size, LayerPack& layer_pack);
    int expandLayer(const std::string& layer_name, const std::string& result_layer_name, float size, LayerPack& source_layer_pack, LayerPack& target_layer_pack);

    int shrinkLayer(const std::string& layer_name, const std::string& result_layer_name, float size, LayerPack& layer_pack);
    int shrinkLayer(const std::string& layer_name, const std::string& result_layer_name, float size, LayerPack& source_layer_pack, LayerPack& target_layer_pack);

    int delLayer(const std::string& layer_name, LayerPack& layer_pack);

    int renameLayer(const std::string& old_layer_name, const std::string& new_layer_name, LayerPack& layer_pack);

    int copyLayer(const std::string& layer_name, const std::string& copy_layer_name, LayerPack& layer_pack);
    int copyLayer(const std::string& layer_name, const std::string& copy_layer_name, LayerPack& source_layer_pack, LayerPack& target_layer_pack);

    int layerHaveFigure(const std::string& layer_name, LayerPack& layer_pack);
};

#endif // PROCESSOR_H
