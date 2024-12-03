#ifndef LAYER_OPERATIONS_H
#define LAYER_OPERATIONS_H

#include <vector>
#include <string>
#include "Entity.h"


namespace LayerOperations {

    bool checkLayer(const Layer& layer);

    void delLayerInLayerPack(const std::string& layer_name, LayerPack& layer_pack);

    void renameLayerInLayerPack(const std::string& old_layer_name, const std::string& new_layer_name, LayerPack& layer_pack);

} // namespace LayerOperations

#endif // LAYER_OPERATIONS_H
