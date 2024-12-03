#ifndef GEOMETRYOPERATIONS_H
#define GEOMETRYOPERATIONS_H

#include "Entity.h"
// Forward declarations

class Trapezoid {
public:
    double x1_top, x2_top, x1_bottom, x2_bottom;
    double y_top, y_bottom;

    Trapezoid(double x1_top, double x2_top, double x1_bottom, double x2_bottom, double y_top, double y_bottom);

    // Копирующий конструктор
    Trapezoid(const Trapezoid& other);

    // Конструктор перемещения
    Trapezoid(Trapezoid&& other);

    // Копирующее присваивание
    Trapezoid& operator=(const Trapezoid& other);

};

namespace TrapezoidOperations {
    std::vector<Trapezoid> unite(const std::vector<Trapezoid>& trapezoids1, const std::vector<Trapezoid>& trapezoids2);
    std::vector<Trapezoid> intersect(const std::vector<Trapezoid>& trapezoids1, const std::vector<Trapezoid>& trapezoids2);
    std::vector<Trapezoid> subtract(const std::vector<Trapezoid>& trapezoids1, const std::vector<Trapezoid>& trapezoids2);
}

namespace PolygonOperations {
    std::vector<Polygon> modifyPolygon(const std::vector<Polygon>& polygons, float size);
}

namespace LayerOperations {
    void copyLayerFromLayerPack(LayerPack& layerpack, const std::string& sourceLayerName, const std::string& targetLayerName);
    void copyLayerFromLayerPack(const LayerPack& layerpack1, LayerPack& layerpack2, const std::string& sourceLayerName, const std::string& targetLayerName);
    bool layerIsEmpty(const Layer& layer);
}


#endif // GEOMETRY_OPERATIONS_H