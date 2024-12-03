#include <iostream>
#include <string>

#include "processor.h"

OperationType getOperationTypeByComand(const std::string& command) {
    if (command == "unite") {
        return OperationType::UNION;
    }
    else if (command == "intersect") {
        return OperationType::INTERSECTION;
    }
    else {
        return OperationType::SUBTRACTION;
    }
}

void printUsage() {
    std::cout << "Usage:\n"
              << "  ./processor source_file_name L1 <operation> L2 to L3 [-o target_file_name]\n"
              << "  ./processor source_file_name L1 <operation> size to L2 [-o target_file_name]\n\n"
              << "Commands:\n"
              << "  unite:      ./processor source_file_name L1 unite L2 to L3 [-o target_file_name]\n"
              << "              Performs union of layers L1 and L2 from source_file_name, saves to L3.\n"
              << "              By default, saves in source_file_name if -o target_file_name is omitted.\n\n"
              << "  intersect:  ./processor source_file_name L1 intersect L2 to L3 [-o target_file_name]\n"
              << "              Performs intersection of layers L1 and L2 from source_file_name, saves to L3.\n"
              << "              By default, saves in source_file_name if -o target_file_name is omitted.\n\n"
              << "  subtract:   ./processor source_file_name L1 subtract L2 to L3 [-o target_file_name]\n"
              << "              Subtracts layer L2 from L1 in source_file_name, saves result to L3.\n"
              << "              By default, saves in source_file_name if -o target_file_name is omitted.\n\n"
              << "  expand:     ./processor source_file_name L1 expand size to L2 [-o target_file_name]\n"
              << "              Expands all polygons in layer L1 by 'size', saves to L2.\n"
              << "              By default, saves in source_file_name if -o target_file_name is omitted.\n\n"
              << "  shrink:     ./processor source_file_name L1 shrink size to L2 [-o target_file_name]\n"
              << "              Shrinks all polygons in layer L1 by 'size', saves to L2.\n"
              << "              If target_file_name and L2 are omitted, results are saved in L1.\n\n"
              << "  del_layer:  ./processor layout_file_name L1 del_layer\n"
              << "              Deletes layer L1 from layout_file_name.\n\n"
              << "  rename_layer: ./processor layout_file_name old_layer_name rename_layer new_layer_name\n"
              << "              Renames old_layer_name to new_layer_name in layout_file_name.\n\n"
              << "  copy_layer: ./processor source_file_name L1 copy_layer to L2 [-o target_file_name]\n"
              << "              Copies layer L1 to a new layer L2 in the target file.\n"
              << "              By default, saves in source_file_name if -o target_file_name is omitted.\n\n"
              << "  have_figure: ./processor source_file_name L1 have_figure\n"
              << "              Checks if layer L1 contains any figures.\n"
              << "              Returns 0 if figures are found, 1 otherwise.\n";
}

void chekcLayerPack(LayerPack& layer_pack, std::string& file_name) {
    for (Layer& layer : layer_pack.get_layers()) {
        if (!LayerOperations::checkLayer(layer)) {
            throw std::invalid_argument("Ошибка в задании полигонов в слое: " + layer.get_name() + " для файла " + file_name + ".");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printUsage();
        return -1;
    }

    Processor processor;
    Converter source_converter;
    Converter target_converter;
    std::string source_file = argv[1];
    source_converter.loadJson(source_file);
    LayerPack& source_layer_pack = source_converter.getLayerPack();
    chekcLayerPack(source_layer_pack, source_file);
    std::string command = argv[3];
    std::string target_file = source_file;  // default target is source file

    try {
        if (command == "unite" || command == "intersect" || command == "subtract") {
            if (argc != 7 && argc != 9 || std::string(argv[5]) != "to") {
                printUsage();
                return -1;
            }
            
            std::string first_layer_name = argv[2];
            std::string second_layer_name = argv[4];
            std::string result_layer_name = argv[6];
            if (argc == 9 && std::string(argv[7]) == "-o") {
                target_file = argv[8];
                target_converter.loadJson(target_file);
                chekcLayerPack(target_converter.getLayerPack(), target_file);
            }
            OperationType op_type = getOperationTypeByComand(command);
            if (target_file == source_file) {
                if (processor.doOperationWithLayers(first_layer_name, second_layer_name, result_layer_name, source_layer_pack, op_type) != 0) {
                    throw std::runtime_error("Ошибка в операции " + getOperationName(op_type) + " при работе с файлом " + source_file);
                }
                source_converter.saveToJson(source_file);
            }
            else {
                if (processor.doOperationWithLayers(first_layer_name, second_layer_name, result_layer_name, source_layer_pack, target_converter.getLayerPack(), op_type) != 0) {
                    throw std::runtime_error("Ошибка в операции " + getOperationName(op_type) + " при работе с файлом " + source_file + " и с результирующим файлом " + target_file);
                }
                target_converter.saveToJson(target_file);
            }
        } else if (command == "expand" || command == "shrink") {
            if (argc != 7 && argc != 9 || std::string(argv[5]) != "to") {
                printUsage();
                return -1;
            }

            std::string first_layer_name = argv[2];
            float size = std::stof(argv[4]);
            std::string result_layer_name = argv[6];
            if (argc == 9 && std::string(argv[7]) == "-o") {
                target_file = argv[8];
            }
            if (command == "expand") {
                if (source_file == target_file) {
                    if (processor.expandLayer(first_layer_name, result_layer_name, size, source_layer_pack) != 0) {
                        throw std::runtime_error("Ошибка в операции расширения при работе с файлом " + source_file);
                    }
                    source_converter.saveToJson(source_file);
                }
                else {
                    if (processor.expandLayer(first_layer_name, result_layer_name, size, source_layer_pack, target_converter.getLayerPack()) != 0) {
                        throw std::runtime_error("Ошибка в операции расширения при работе с файлом " + source_file + " и с результирующим файлом " + target_file);
                    }
                    target_converter.saveToJson(target_file);
                }
            } else if (command == "shrink") {
                if (source_file == target_file) {
                    if (processor.shrinkLayer(first_layer_name, result_layer_name, size, source_layer_pack) != 0) {
                        throw std::runtime_error("Ошибка в операции сужения при работе с файлом " + source_file);
                    }
                    source_converter.saveToJson(source_file);
                }
                else {
                    if (processor.shrinkLayer(first_layer_name, result_layer_name, size, source_layer_pack, target_converter.getLayerPack()) != 0) {
                        throw std::runtime_error("Ошибка в операции сужения при работе с файлом " + source_file + " и с результирующим файлом " + target_file);
                    }
                    target_converter.saveToJson(target_file);
                }
            }
        } else if (command == "del_layer") {
            if (argc != 4) {
                printUsage();
                return -1;
            }

            std::string layer_name = argv[2];
            if (processor.delLayer(layer_name, source_layer_pack) != 0) {
                throw std::runtime_error("Ошибка в операции удаления слоя при работе с файлом " + source_file);
            }
            source_converter.saveToJson(source_file);
        } else if (command == "rename_layer") {
            if (argc != 5) {
                printUsage();
                return -1;
            }

            std::string old_layer_name = argv[2];
            std::string new_layer_name = argv[4];
            if (processor.renameLayer(old_layer_name, new_layer_name, source_layer_pack) != 0) {
                throw std::runtime_error("Ошибка в операции переименования слоя при работе с файлом " + source_file);
            }
            source_converter.saveToJson(source_file);
        } else if (command == "copy_layer") {
            if (argc != 6 && argc != 8 || std::string(argv[4]) != "to") {
                printUsage();
                return -1;
            }
            std::string first_layer_name = argv[2];
            std::string second_layer_name = argv[5];
            if (argc == 8 && std::string(argv[6]) == "-o") {
                target_file = argv[8];
            }
            if (target_file == source_file) {
                if (processor.copyLayer(first_layer_name, second_layer_name, source_layer_pack) != 0) {
                    throw std::runtime_error("Ошибка в операции копирования слоя при работе с файлом " + source_file);
                }
                source_converter.saveToJson(source_file);
            }
            else {
                if (processor.copyLayer(first_layer_name, second_layer_name, source_layer_pack, target_converter.getLayerPack()) != 0) {
                    throw std::runtime_error("Ошибка в операции копирования слоя при работе с файлом " + source_file + " и с результирующим файлом " + target_file);
                }
                target_converter.saveToJson(target_file);
            }
        } else if (command == "have_figure") {
            if (argc != 4) {
                printUsage();
                return -1;
            }

            std::string layer_name = argv[2];
            int result = processor.layerHaveFigure(layer_name, source_layer_pack);
            if (result == -1) {
                throw std::runtime_error("Ошибка в операции проверки наличия фигур при работе с фалом " + source_file);
            }
            int ans = result ? 1 : 0;
            return ans;
        } else {
            printUsage();
            return -1;
        }
    } catch (const std::exception &e) {
        std::cerr << "Ошибка при выполнении main: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
