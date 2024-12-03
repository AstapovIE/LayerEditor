#include "processor.h"
#include <iostream>

std::string getOperationName(OperationType type) {
    switch (type) {
        case OperationType::UNION: return "UNION"; break;
        case OperationType::INTERSECTION: return "INTERSECTION"; break;
        case OperationType::SUBTRACTION: return "SUBTRACTION"; break;
        default: return "OTHER";
    }
}

bool checkName(const std::string& new_name, std::vector<string> existing_names) {
    for (std::string name : existing_names) {
        if (name == new_name) {
            return false;
        }
    }
    return true;
}

int Processor::doOperationWithLayers(const std::string& first_layer_name, const std::string& second_layer_name,
                           const std::string& result_layer_name, LayerPack& layer_pack, OperationType type) {
    try {
        if (getOperationName(type) == "OTHER") {
            throw std::invalid_argument("Неподходящий тип операции для метода doOperationWithLayers");
        }
        if (checkName(first_layer_name, layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя слоя " + first_layer_name + " не существует в файле топологии.");
        }
        if (checkName(second_layer_name, layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя слоя " + first_layer_name + " не существует в файле топологии.");
        }
        if (!checkName(result_layer_name, layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя результирующего слоя " + result_layer_name + " уже существует в файле топологии.");
        }
        Layer& first_layer = layer_pack[first_layer_name];
        Layer& second_layer = layer_pack[second_layer_name];

        const std::vector<Polygon>& polygons1 = first_layer.get_polygons();
        const std::vector<Polygon>& polygons2 = second_layer.get_polygons();

        std::vector<Polygon> result_polygons = PolygonOperations::processPolygons(polygons1, polygons2, type);
        for (auto& polygon : result_polygons) {
            // std::cout << "Polygon:" << "\n";
            // for (auto point : polygon.get_points()) {
            //     std:: cout << point.x << " " << point.y << "\n";
            // }
        }
        Layer result_layer(result_layer_name, result_polygons);
        layer_pack.append_layer(result_layer);
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка в операции " << getOperationName(type) << " со слоями - " << first_layer_name << " и " << second_layer_name << ": " << e.what() << std::endl;
        return -1;
    }
}

int Processor::doOperationWithLayers(const std::string& first_layer_name, const std::string& second_layer_name,
                           const std::string& result_layer_name, LayerPack& source_layer_pack, LayerPack& target_layer_pack, OperationType type) {
    try {
        if (getOperationName(type) == "OTHER") {
            throw std::invalid_argument("Неподходящий тип операции для метода doOperationWithLayers");
        }
        if (checkName(first_layer_name, source_layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя слоя " + first_layer_name + " не существует в файле топологии.");
        }
        if (checkName(second_layer_name, source_layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя слоя " + first_layer_name + " не существует в файле топологии.");
        }
        if (!checkName(result_layer_name, target_layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя результирующего слоя " + result_layer_name + " уже существует в файле топологии.");
        }
        Layer& first_layer = source_layer_pack[first_layer_name];
        Layer& second_layer = source_layer_pack[second_layer_name];

        const std::vector<Polygon>& polygons1 = first_layer.get_polygons();
        const std::vector<Polygon>& polygons2 = second_layer.get_polygons();

        std::vector<Polygon> result_polygons = PolygonOperations::processPolygons(polygons1, polygons2, type);

        Layer result_layer(result_layer_name, result_polygons);
        target_layer_pack.append_layer(result_layer);
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка в операции " << getOperationName(type) << " со слоями - " << first_layer_name << " и " << second_layer_name << ": " << e.what() << std::endl;
        return -1;
    }
}

int Processor::expandLayer(const std::string& layer_name, const std::string& result_layer_name, float size,
                           LayerPack& layer_pack) {
    try {
        if (checkName(layer_name, layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя слоя " + layer_name + " не существует в файле топологии.");
        }
        if (!checkName(result_layer_name, layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя результирующего слоя " + result_layer_name + " уже существует в файле топологии.");
        }
        Layer& layer = layer_pack[layer_name];

        const std::vector<Polygon>& polygons = layer.get_polygons();

        std::vector<Polygon> result_polygons = PolygonOperations::modifyPolygon(polygons, size);

        Layer result_layer(result_layer_name, result_polygons);
        if (layer_name == result_layer_name) {
            layer = result_layer;
        }
        else {
            layer_pack.append_layer(result_layer);
        }
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Ошбика в операции расширения/сужения фигур внутри слоя с именем - " <<  layer_name << " и с результирующем именем " << result_layer_name  << ": " << e.what() << std::endl;
        return -1;
    }
}

int Processor::expandLayer(const std::string& layer_name, const std::string& result_layer_name, float size,
                           LayerPack& source_layer_pack, LayerPack& target_layer_pack) {
    try {
        if (checkName(layer_name, source_layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя слоя " + layer_name + " не существует в файле топологии.");
        }
        if (!checkName(result_layer_name, target_layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя результирующего слоя " + result_layer_name + " уже существует в файле топологии.");
        }

        Layer& layer = source_layer_pack[layer_name];

        const std::vector<Polygon>& polygons = layer.get_polygons();

        std::vector<Polygon> result_polygons = PolygonOperations::modifyPolygon(polygons, size);

        Layer result_layer(result_layer_name, result_polygons);
        target_layer_pack.append_layer(result_layer);
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Ошбика в операции расширения/сужения фигур внутри слоя с именем - " <<  layer_name << " и с результирующем именем " << result_layer_name  << ": " << e.what() << std::endl;
        return -1;
    }
}

int Processor::shrinkLayer(const std::string& layer_name, const std::string& result_layer_name, float size,
                           LayerPack& source_layer_pack, LayerPack& target_layer_pack) {
    return expandLayer(layer_name, result_layer_name, -size, source_layer_pack, target_layer_pack);
}

int Processor::shrinkLayer(const std::string& layer_name, const std::string& result_layer_name, float size,
                           LayerPack& layer_pack) {
    return expandLayer(layer_name, result_layer_name, -size, layer_pack);
}

int Processor::delLayer(const std::string& layer_name, LayerPack& layer_pack) {
    try {
        if (checkName(layer_name, layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя слоя " + layer_name + " не существует в файле топологии.");
        }
        LayerOperations::delLayerInLayerPack(layer_name, layer_pack);
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка в операции удаления слоя с именем -- " << layer_name << ": " << e.what() << std::endl;
        return -1;
    }
}

int Processor::renameLayer(const std::string& old_layer_name, const std::string& new_layer_name, LayerPack& layer_pack) {
    try {
        if (checkName(old_layer_name, layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя слоя " + old_layer_name + " не существует в файле топологии.");
        }
        if (!checkName(new_layer_name, layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя результирующего слоя " + new_layer_name + " уже существует в файле топологии.");
        }
        LayerOperations::renameLayerInLayerPack(old_layer_name, new_layer_name, layer_pack);

        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка в операции переименования слоя с старым и новым именем соответсвенно -  " << old_layer_name << " и" << new_layer_name << ": " << e.what() << std::endl;
        return -1;
    }
}

int Processor::copyLayer(const std::string& layer_name, const std::string& copy_layer_name, LayerPack& layer_pack) {
    try {
        if (checkName(layer_name, layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя слоя " + layer_name + " не существует в файле топологии.");
        }
        if (!checkName(copy_layer_name, layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя результирующего слоя " + copy_layer_name + " уже существует в файле топологии.");
        }
        LayerOperations::copyLayerFromLayerPack(layer_pack, layer_name, copy_layer_name);
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка в операции копирования слоя с именами слоев - " << layer_name << " и " << copy_layer_name << ": " << e.what() << std::endl;
        return -1;
    }
}

int Processor::copyLayer(const std::string& layer_name, const std::string& copy_layer_name,
                        LayerPack& source_layer_pack, LayerPack& target_layer_pack) {
    try {
        if (checkName(layer_name, source_layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя слоя " + layer_name + " не существует в файле топологии.");
        }
        if (!checkName(copy_layer_name, target_layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя результирующего слоя " + copy_layer_name + " уже существует в файле топологии.");
        }
        LayerOperations::copyLayerFromLayerPack(source_layer_pack, target_layer_pack, layer_name, copy_layer_name);
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка в операции копирования слоя с именами слоев - " << layer_name << " и " << copy_layer_name << ": " << e.what() << std::endl;
        return -1;
    }
}

int Processor::layerHaveFigure(const std::string& layer_name, LayerPack& layer_pack) {
    try {
        Layer& layer = layer_pack[layer_name];
        if (checkName(layer_name, layer_pack.get_layers_names())) {
            throw std::invalid_argument("Имя слоя " + layer_name + " не существует в файле топологии.");
        }
        if (LayerOperations::layerIsEmpty(layer)) {
            return 1;
        } else {
            return 0;
        }
    } catch (const std::exception &e) {
        std::cerr << "Ошибка в операции проверки наличия фигур в слое с именем - " << layer_name << ": " << e.what() << std::endl;
        return -1;
    }
}