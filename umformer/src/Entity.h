#ifndef ENTITY_H
#define ENTITY_H

#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>


const double EPSILON = 1e-6;     // Глобальная константа для точности сравнения
const int OUTPUT_PRECISION = 6;  // Константа для точности вывода


class Point {
public:
    double x; // Координата точки по оси X
    double y; // Координата точки по оси Y

    Point(double x, double y);

    Point operator+(const Point& other) const;
    Point operator-(const Point& other) const;
    Point operator*(double num) const;
    Point operator/(double num) const;
    Point& operator*=(double num);
    Point& operator/=(double num);
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    operator std::string() const;

    std::unordered_map<std::string, double> ravel() const;

    friend std::ostream& operator<<(std::ostream& os, const Point& point);

    static std::string format_double(double num);
};


class AbstractPolygon {
protected:
    AbstractPolygon(const std::vector<Point>& points = {});
    std::vector<Point> points;

public:
    virtual ~AbstractPolygon() = default;
    void append(const Point& point);
    void insert(const Point& point, size_t index);
    void remove(size_t index);
    const std::vector<Point>& get_points() const;

    Point& operator[](size_t index);
    const Point& operator[](size_t index) const;
    bool operator==(const AbstractPolygon& other) const;
    bool operator!=(const AbstractPolygon& other) const;

    operator std::string() const;

    friend std::ostream& operator<<(std::ostream& os, const AbstractPolygon& abstract_polygon);
};


class Hole : public AbstractPolygon {
public:
    Hole(const std::vector<Point>& points = {});
};


class Polygon : public AbstractPolygon {
private:
    std::vector<Hole> holes;

public:
    Polygon(const std::vector<Point>& points = {}, const std::vector<Hole>& holes = {});

    void add_hole(const Hole& hole);
    void remove_hole(size_t index);
    const std::vector<Hole>& get_holes() const;
    std::vector<Hole>& get_holes();

    bool operator==(const Polygon& other) const;
    bool operator!=(const Polygon& other) const;

    operator std::string() const;
};


class Layer {
private:
    std::string name;
    std::vector<Polygon> polygons;

public:
    Layer(const std::string& name = "Unnamed Layer", const std::vector<Polygon>& polygons = {});
    Layer(const Layer& other) = default;          // Конструктор копирования
    Layer(Layer&& other) noexcept = default;       // Перемещающий конструктор
    Layer& operator=(const Layer& other) = default; // Оператор копирования
    Layer& operator=(Layer&& other) noexcept = default; // Оператор перемещения

    const std::string& get_name() const;
    void rename(const std::string& new_name);
    void append(const Polygon& polygon);
    void remove(size_t index);
    const std::vector<Polygon>& get_polygons() const;

    Polygon& operator[](size_t index);
    const Polygon& operator[](size_t index) const;
    bool operator==(const Layer& other) const;
    bool operator!=(const Layer& other) const;

    operator std::string() const;
    friend std::ostream& operator<<(std::ostream& os, const Layer& point);
};



class LayerPack {
private:
    std::vector<Layer> layers;

    // Вспомогательная функция для поиска индекса слоя по имени
    size_t find_layer_index(const std::string& name) const;
public:
    LayerPack(const std::vector<Layer>& layers = {});

    void append_layer(const Layer& layer);
    void remove_layer(const std::string& name);
    const std::vector<Layer>& get_layers() const;
    std::vector<Layer>& get_layers();
    std::vector<std::string> get_layers_names() const;
    size_t size() const;

    Layer& operator[](const std::string& name);
    const Layer& operator[](const std::string& name) const;
    bool operator==(const LayerPack& other) const;
    bool operator!=(const LayerPack& other) const;

    operator std::string() const;
    friend std::ostream& operator<<(std::ostream& os, const LayerPack& point);
};


#endif // ENTITY_H
