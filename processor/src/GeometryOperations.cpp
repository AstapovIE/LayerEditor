#include <algorithm>
#include <utility>
#include <tuple>
#include "GeometryOperations.h"

Trapezoid :: Trapezoid(double x1_top, double x2_top, double x1_bottom, double x2_bottom, double y_top, double y_bottom)
        : x1_top(x1_top), x2_top(x2_top), x1_bottom(x1_bottom), x2_bottom(x2_bottom), y_top(y_top), y_bottom(y_bottom) {}

    // Копирующий конструктор
Trapezoid :: Trapezoid(const Trapezoid& other)
        : x1_top(other.x1_top), x2_top(other.x2_top), x1_bottom(other.x1_bottom), x2_bottom(other.x2_bottom),
          y_top(other.y_top), y_bottom(other.y_bottom) {}

    // Конструктор перемещения
Trapezoid :: Trapezoid(Trapezoid&& other)
        : x1_top(std::move(other.x1_top)), x2_top(std::move(other.x2_top)),
          x1_bottom(std::move(other.x1_bottom)), x2_bottom(std::move(other.x2_bottom)),
          y_top(std::move(other.y_top)), y_bottom(std::move(other.y_bottom)) {}


Trapezoid& Trapezoid :: operator=(const Trapezoid& other) {
        if (this != &other) { // проверка самоприсваивания
            x1_top = other.x1_top;
            x2_top = other.x2_top;
            x1_bottom = other.x1_bottom;
            x2_bottom = other.x2_bottom;
            y_top = other.y_top;
            y_bottom = other.y_bottom;
        }
        return *this;
    }



namespace TrapezoidOperations {

    // Функция для вычисления пересечения двух трапецоидов по оси y
    std::pair<double, double> overlapY(const Trapezoid& a, const Trapezoid& b) {
        double y_top = std::min(a.y_top, b.y_top);
        double y_bottom = std::max(a.y_bottom, b.y_bottom);
        if(a.x2_bottom >= b.x1_bottom || a.x2_top >= b.x1_top || b.x1_bottom <= a.x2_bottom || b.x1_top <= a.x2_top)
            return std::make_pair(y_top, y_bottom);
        else
            return std::make_pair(-1, 0);
    }

    // Линейная интерполяция для вычисления координаты x на высоте y
    double interpolateX(double y, double y1, double y2, double x1, double x2) {
        return x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    }

    Point findIntersection(Point A, Point B, Point C, Point D) {
        double a1 = B.y - A.y;
        double b1 = A.x - B.x;
        double c1 = a1 * A.x + b1 * A.y;

        double a2 = D.y - C.y;
        double b2 = C.x - D.x;
        double c2 = a2 * C.x + b2 * C.y;

        double det = a1 * b2 - a2 * b1;

        if (det == 0) {
            throw std::runtime_error("Lines are parallel or coincident");
        }

        double x = (b2 * c1 - b1 * c2) / det;
        double y = (a1 * c2 - a2 * c1) / det;

        return {x, y};
    }

    bool isBetween(Point A, Point B, Point P) {
        if (A.x != B.x) { // Если прямая не вертикальная
            return (P.x - A.x) * (P.x - B.x) <= 0 && std::abs(P.x - A.x) + std::abs(P.x - B.x) >= std::abs(B.x - A.x);
        } else { // Если прямая вертикальная
            return (P.y - A.y) * (P.y - B.y) <= 0 && std::abs(P.y - A.y) + std::abs(P.y - B.y) >= std::abs(B.y - A.y);
        }
    }

    std::vector<Point> getIntersectionPoints(const Trapezoid& t1, const Trapezoid& t2) {
        std::vector<Point> intersections;

        // Define points for edges in both trapezoids
        Point A1 = {t1.x1_top, t1.y_top}, B1 = {t1.x2_top, t1.y_top};
        Point C1 = {t1.x1_bottom, t1.y_bottom}, D1 = {t1.x2_bottom, t1.y_bottom};
        Point A2 = {t2.x1_top, t2.y_top}, B2 = {t2.x2_top, t2.y_top};
        Point C2 = {t2.x1_bottom, t2.y_bottom}, D2 = {t2.x2_bottom, t2.y_bottom};

        // Check each edge pair for intersections
        try { intersections.push_back(findIntersection(A1, B1, A2, B2)); } catch (...) {}
        try { intersections.push_back(findIntersection(C1, D1, C2, D2)); } catch (...) {}
        try { intersections.push_back(findIntersection(A1, C1, A2, C2)); } catch (...) {}
        try { intersections.push_back(findIntersection(B1, D1, B2, D2)); } catch (...) {}

        // Filter points within the trapezoid bounds to avoid out-of-bounds intersections
        intersections.erase(std::remove_if(intersections.begin(), intersections.end(), [&](Point p) {
            return !(p.y <= std::max(t1.y_top, t2.y_top) && p.y >= std::min(t1.y_bottom, t2.y_bottom));
        }), intersections.end());

        return intersections;
    }


    // Создает трапецоид для области пересечения,
    // вычисляя актуальные координаты трапецоида для рассматриваемой координаты y
    Trapezoid create_trapezoid(double y_top, double y_bottom, const Trapezoid& a, const Trapezoid& b, bool is_union) {
        double x1_top_a = interpolateX(y_top, a.y_top, a.y_bottom, a.x1_top, a.x1_bottom);
        double x1_top_b = interpolateX(y_top, b.y_top, b.y_bottom, b.x1_top, b.x1_bottom);

        double x2_top_a = interpolateX(y_top, a.y_top, a.y_bottom, a.x2_top, a.x2_bottom);
        double x2_top_b = interpolateX(y_top, b.y_top, b.y_bottom, b.x2_top, b.x2_bottom);

        double x1_bottom_a = interpolateX(y_bottom, a.y_top, a.y_bottom, a.x1_top, a.x1_bottom);
        double x1_bottom_b = interpolateX(y_bottom, b.y_top, b.y_bottom, b.x1_top, b.x1_bottom);

        double x2_bottom_a = interpolateX(y_bottom, a.y_top, a.y_bottom, a.x2_top, a.x2_bottom);
        double x2_bottom_b = interpolateX(y_bottom, b.y_top, b.y_bottom, b.x2_top, b.x2_bottom);

        double x1_top = is_union ? std::min(x1_top_a, x1_top_b)
                                 : std::max(x1_top_a, x1_top_b);

        double x2_top = is_union ? std::max(x2_top_a, x2_top_b)
                                 : std::min(x2_top_a, x2_top_b);

        double x1_bottom = is_union ? std::min(x1_bottom_a, x1_bottom_b)
                                    : std::max(x1_bottom_a, x1_bottom_b);


        double x2_bottom = is_union ? std::max(x2_bottom_a, x2_bottom_b)
                                    : std::min(x2_bottom_a, x2_bottom_b);

        return Trapezoid(x1_top, x2_top, x1_bottom, x2_bottom, y_top, y_bottom);

    }

    std::vector<Trapezoid> create_trapezoids_substructed(double y_top, double y_bottom, const Trapezoid& a, const Trapezoid& b, bool is_substruct=false){
        std::vector<Trapezoid> result;
        double x1_top_a = interpolateX(y_top, a.y_top, a.y_bottom, a.x1_top, a.x1_bottom);
        double x1_top_b = interpolateX(y_top, b.y_top, b.y_bottom, b.x1_top, b.x1_bottom);

        double x2_top_a = interpolateX(y_top, a.y_top, a.y_bottom, a.x2_top, a.x2_bottom);
        double x2_top_b = interpolateX(y_top, b.y_top, b.y_bottom, b.x2_top, b.x2_bottom);

        double x1_bottom_a = interpolateX(y_bottom, a.y_top, a.y_bottom, a.x1_top, a.x1_bottom);
        double x1_bottom_b = interpolateX(y_bottom, b.y_top, b.y_bottom, b.x1_top, b.x1_bottom);

        double x2_bottom_a = interpolateX(y_bottom, a.y_top, a.y_bottom, a.x2_top, a.x2_bottom);
        double x2_bottom_b = interpolateX(y_bottom, b.y_top, b.y_bottom, b.x2_top, b.x2_bottom);

        if(a.x1_bottom <= b.x1_bottom){
            double x1_top = x1_top_a;
            double x2_top =  std::min(x2_top_a, x1_top_b);
            double x1_bottom = x1_bottom_a;
            double x2_bottom = std::min(x2_bottom_a, x1_bottom_b);
            if(x1_top <= x2_top && x1_bottom < x2_bottom || x1_top < x2_top && x1_bottom <= x2_bottom)
                result.emplace_back(x1_top, x2_top, x1_bottom, x2_bottom, y_top, y_bottom);
        }
        else if(a.x2_bottom >= b.x2_bottom){
            double x1_top = std::max(x1_top_a, x2_top_b);
            double x2_top =  x2_top_a;
            double x1_bottom = std::max(x1_bottom_a, x2_bottom_b);
            double x2_bottom = x2_bottom_a;
            if(x1_top <= x2_top && x1_bottom < x2_bottom || x1_top < x2_top && x1_bottom <= x2_bottom)
                result.emplace_back(x1_top, x2_top, x1_bottom, x2_bottom, y_top, y_bottom);
        }
        return result;
    }


    std::vector<Trapezoid> unite(const std::vector<Trapezoid>& trapezoids1, const std::vector<Trapezoid>& trapezoids2) {
        std::vector<Trapezoid> result;

        for (const auto& a : trapezoids1) {
            bool intersected = false;

            for (const auto& b : trapezoids2) {
                std::pair<double, double> y_overlap_result = overlapY(a, b);
                double y_top = y_overlap_result.first;
                double y_bottom = y_overlap_result.second;

                if (y_top > y_bottom) { // Есть пересечение по y
                    // 1. Добавляем верхнюю часть трапецоида до области пересечения
                    if (a.y_bottom < y_bottom) {
                        result.push_back(create_trapezoid(y_bottom, a.y_bottom, a, a, true));
                    } else if (b.y_bottom < y_bottom) {
                        result.push_back(create_trapezoid(y_bottom, b.y_bottom, b, b, true));
                    }

                    // 2. Добавляем общую часть пересечения (объединение трапецоидов)
                    result.push_back(create_trapezoid(y_top, y_bottom, a, b, true));

                    // 3. Добавляем нижнюю часть трапецоида после области пересечения
                    if (a.y_top > y_top) {
                        result.push_back(create_trapezoid(a.y_top, y_top, a, a, true));
                    } else if (b.y_top > y_top) {
                        result.push_back(create_trapezoid(b.y_top, y_top, b, b, true));
                    }

                    intersected = true;
                    break;
                }
            }

            // Если не было пересечения, добавляем весь трапецоид
            if (!intersected) {
                result.push_back(a);
            }
        }

        // Добавляем трапецоиды из второго множества, которые не пересеклись с первым
        for (const auto& b : trapezoids2) {
            bool intersected = false;

            for (const auto& a : trapezoids1) {
                std::pair<double, double> y_overlap_result = overlapY(b, a);
                if (y_overlap_result.first > y_overlap_result.second) {
                    intersected = true;
                    break;
                }
            }

            if (!intersected) {
                result.push_back(b);
            }
        }

        return result;
    }

    std::vector<Trapezoid> intersect(const std::vector<Trapezoid>& trapezoids1, const std::vector<Trapezoid>& trapezoids2) {
        std::vector<Trapezoid> result;

        for (const auto& a : trapezoids1) {
            for (const auto& b : trapezoids2) {
                // Проверяем пересечение по y
                std::pair<double, double> y_overlap_result = overlapY(a, b);
                double y_top = y_overlap_result.first;
                double y_bottom = y_overlap_result.second;

                if (y_top > y_bottom) { // Есть пересечение по y
                    // Добавляем только общую часть, используя `create_trapezoid`
                    result.push_back(create_trapezoid(y_top, y_bottom, a, b, false));  // false для пересечения
                }
            }
        }

        return result;
    }


    // Функция для вычитания двух векторов трапезоидов
    std::vector<Trapezoid> subtract(const std::vector<Trapezoid>& trapezoids1, const std::vector<Trapezoid>& trapezoids2) {
        std::vector<Trapezoid> result;

        for (const auto& a : trapezoids1) {
            bool intersected = false;

            for (const auto& b : trapezoids2) {
                // Проверяем пересечение по y
                std::pair<double, double> y_overlap_result = overlapY(a, b);
                double y_top = y_overlap_result.first;
                double y_bottom = y_overlap_result.second;

                if (y_top > y_bottom) { // Есть пересечение по y
                    intersected = true;

                    // 1. Добавляем часть до пересечения
                    if (a.y_bottom < y_bottom) {
                        result.push_back(create_trapezoid(y_bottom, a.y_bottom, a, a, true));
                    }

                    // 2. Переcечение
                    auto trapezoids = create_trapezoids_substructed(y_top, y_bottom, a, b, true);
                    for(const auto& trap : trapezoids)
                        result.push_back(trap);

                    // 3. Добавляем часть после пересечения
                    if (a.y_top > y_top) {
                        result.push_back(create_trapezoid(a.y_top, y_top, a, a, true));
                    }
                    break;
                }
            }

            // Если не было пересечения, добавляем весь трапецоид из первого множества
            if (!intersected) {
                result.push_back(a);
            }
        }

        return result;
    }
} // namespace TrapezoidOperations



namespace PolygonOperations {

     std::vector<Polygon> modifyPolygon(const std::vector<Polygon>& polygons, float size) {
        std::vector<Polygon> modifiedPolygons;

        size = size > 0 ? size : 1 / std::abs(size); // Если size - отрицаетльный, то делим на модуль size

        for (const Polygon& polygon : polygons) {
            std::vector<Point> newVertices;

            // Изменяем каждую вершину, масштабируя её относительно центра полигона
            Point center = Point(0, 0);
            for (const Point& vertex : polygon.get_points()) {
                center = center + vertex;
            }
            center.x /= polygon.get_points().size();
            center.y /= polygon.get_points().size();

            for (const Point& vertex : polygon.get_points()) {
                Point scaledVertex = center + (vertex - center) * size;
                newVertices.push_back(scaledVertex);
            }
            // Масштабируем каждую вершину для каждой дырки относительно центра полигона
            std::vector<Hole> newHoles;
            for (const Hole& hole : polygon.get_holes()) {
                std::vector<Point> newHoleVertices;
                for (const Point& vertex : hole.get_points()) {
                    double newX = center.x + (vertex.x - center.x) * size;
                    double newY = center.y + (vertex.y - center.y) * size;
                    newHoleVertices.emplace_back(newX, newY);
                }
                newHoles.emplace_back(newHoleVertices);
            }

            // Создаём новый полигон с модифицированными вершинами и дырками
            Polygon modifiedPolygon(newVertices, newHoles);
            modifiedPolygons.push_back(modifiedPolygon);
        }

        return modifiedPolygons;
    }
}   // namespace PolygonOperations

namespace LayerOperations {

    // Копирование слоя внутри одного LayerPack
    void copyLayerFromLayerPack(LayerPack& layerpack, const std::string& sourceLayerName, const std::string& targetLayerName) {

        const Layer& sourceLayer = layerpack[sourceLayerName];
        Layer copiedLayer = sourceLayer;
        copiedLayer.rename(targetLayerName);

        layerpack.append_layer(copiedLayer);
    }

    // Копирование слоя из одного LayerPack в другой
    void copyLayerFromLayerPack(const LayerPack& layerpack1, LayerPack& layerpack2, const std::string& sourceLayerName, const std::string& targetLayerName) {
        const Layer& sourceLayer = layerpack1[sourceLayerName];
        Layer copiedLayer = sourceLayer;
        copiedLayer.rename(targetLayerName);

        layerpack2.append_layer(copiedLayer);
    }

    // Проверка наличия фигур в слое
    bool layerIsEmpty(const Layer& layer) {
        return layer.get_polygons().empty();
    }
}  // namespace LayerOperations