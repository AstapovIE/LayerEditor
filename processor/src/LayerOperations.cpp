#include <vector>              
#include <algorithm>           
#include "LayerOperations.h"  
#include <iostream>           

namespace LayerOperations{
    double crossProduct(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    bool isCollinear(const Point& a, const Point& b, const Point& c) {
        return std::abs(crossProduct(a, b, c)) < EPSILON;  
    }

    bool doSegmentsIntersect(const Point& a, const Point& b, const Point& c, const Point& d) {
        auto ccw = [](const Point& p1, const Point& p2, const Point& p3) {
            return crossProduct(p1, p2, p3) > 0;
        };

        return ccw(a, c, d) != ccw(b, c, d) && ccw(a, b, c) != ccw(a, b, d);
    }

    bool checkSelfIntersection(const Polygon& polygon) {
        const std::vector<Point>& vertices = polygon.get_points();
        size_t n = vertices.size();

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 2; j < n; ++j) {
                // Пропускаем проверки соседних сегментов (i и i+1) и замыкающего сегмента с начальным
                if ((i == 0 && j == n - 1) || (j == i + 1)) {
                    continue;
                }

                if (doSegmentsIntersect(vertices[i], vertices[(i + 1) % n], vertices[j], vertices[(j + 1) % n])) {
                    return true;  
                }
            }
        }
        return false;  
    }

    bool isPointInsidePolygon(const Point& p, const Polygon& polygon) {
        const auto& vertices = polygon.get_points();
        size_t n = vertices.size();

        // Проверка четности пересечений луча, исходящего из точки p
        bool inside = false;
        for (size_t i = 0; i < n; ++i) {
            const Point& v1 = vertices[i];
            const Point& v2 = vertices[(i + 1) % n];

            // Проверяем пересечение горизонтального луча, исходящего из p, с ребром v1-v2
            if (((v1.y > p.y) != (v2.y > p.y)) &&
                (p.x < (v2.x - v1.x) * (p.y - v1.y) / (v2.y - v1.y) + v1.x)) {
                inside = !inside;
            }
        }
        return inside;
    }

    bool doPolygonsIntersect(const Polygon& p1, const Polygon& p2) {
        const auto& vertices1 = p1.get_points();
        const auto& vertices2 = p2.get_points();
        size_t n_1 = vertices1.size();
        size_t n_2 = vertices2.size();

        if (n_1 < 3 || n_2 < 3) {
            std::cout << "Polygon does not have enough points" << std::endl;
            return false;
        }

        // Для простоты проверяем пересечение всех отрезков двух многоугольников
        for (size_t i = 0; i < vertices1.size(); ++i) {
            for (size_t j = 0; j < vertices2.size(); ++j) {
                if (doSegmentsIntersect(vertices1[i], vertices1[(i + 1) % vertices1.size()],
                                        vertices2[j], vertices2[(j + 1) % vertices2.size()])) {
                    return true;  
                }
            }
        }

        // Если нет пересечений границ, проверяем вложенность
        // Проверяем, лежит ли хотя бы одна точка одного многоугольника внутри другого
        if (isPointInsidePolygon(vertices1[0], p2) || isPointInsidePolygon(vertices2[0], p1)) {
            return true;
        }

        return false;
    }

    bool checkPolygon(const Polygon& polygon) {
        const auto& vertices = polygon.get_points();
        size_t n = vertices.size();

        // Проверка на минимальное количество точек
        if (n < 3) {
            std::cout << "Polygon does not have enough points" << std::endl;
            return false;
        }
        
        // Проверка на коллинеарные вершины
        for (size_t i = 0; i < n; ++i) {
            if (isCollinear(vertices[i], vertices[(i + 1) % n], vertices[(i + 2) % n])) {
                return false;
            }
        }

        // Проверка на самопересечение
        if (checkSelfIntersection(polygon)) {
            return false;  
        }

        return true;
    }

    bool checkLayer(const Layer& layer) {
        const auto& polygons = layer.get_polygons();

        // Проверяем каждый полигон в слое
        for (const auto& polygon : polygons) {
            if (!checkPolygon(polygon)) {
                return false;  
            }
        }

        // Проверка на пересечение полигонов в слое
        size_t n = polygons.size();
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                if (doPolygonsIntersect(polygons[i], polygons[j])) {
                    return false; 
                }
            }
        }

        return true;  // Все проверки прошли успешно
    }

    // Удаление слоя с заданным именем из LayerPack
    void delLayerInLayerPack(const std::string& layer_name, LayerPack& layer_pack) {
        // Пробегаемся по всем слоям, чтобы найти нужный по имени
        for (size_t i = 0; i < layer_pack.get_layers().size(); ++i) {
            if (layer_pack.get_layers()[i].get_name() == layer_name) {
                // Удаляем слой, если найден
                layer_pack.remove_layer(layer_name);
                return; 
            }
        }

        // Если слой не найден, выводим сообщение
        std::cout << "Layer \"" << layer_name << "\" was not found and could not be deleted." << std::endl;
    }

    // Переименование слоя в LayerPack
    void renameLayerInLayerPack(const std::string& old_layer_name, const std::string& new_layer_name, LayerPack& layer_pack) {
        // Получаем список слоев
        auto& layers = layer_pack.get_layers(); 

        // Проверяем, существует ли уже слой с именем new_layer_name
        auto existing_layer = std::find_if(layers.begin(), layers.end(), [&](const Layer& layer) {
            return layer.get_name() == new_layer_name;
        });

        if (existing_layer != layers.end()) {
            std::cout << "A layer with the name \"" << new_layer_name << "\" already exists and cannot be renamed." << std::endl;
            return; 
        }

        // Ищем слой с именем old_layer_name
        auto target_layer = std::find_if(layers.begin(), layers.end(), [&](const Layer& layer) {
            return layer.get_name() == old_layer_name;
        });

        if (target_layer == layers.end()) {
            std::cout << "Слой с именем \"" << old_layer_name << "\" was not found and could not be renamed." << std::endl;
            return; 
        }

        // Переименовываем найденный слой
        size_t index = std::distance(layers.begin(), target_layer);
        layers[index].rename(new_layer_name); 
    }


}  // namespace LayerOperations

