#ifndef ENTITY_H
#define ENTITY_H

#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>

class Point {
public:
    double x; // Координата точки по оси X
    double y; // Координата точки по оси Y

    Point(double x = 0.0, double y = 0.0);

    Point operator+(const Point& other) const;
    Point operator-(const Point& other) const;
    bool operator==(const Point& other) const;

    std::unordered_map<std::string, double> ravel() const;
};

class AbstractPolygon {
protected:
    std::vector<Point> vertices;

public:
    AbstractPolygon() = default;

    virtual ~AbstractPolygon() = default;
    virtual void append(const Point& point) = 0;
    virtual void insert(const Point& point, size_t index) = 0;
    virtual void remove(size_t index) = 0;
    virtual const std::vector<Point>& get_vertices() const = 0;

    virtual Point& operator[](size_t index) = 0;
    virtual const Point& operator[](size_t index) const = 0;
};


class Hole : public AbstractPolygon {
public:
    Hole(const std::vector<Point>& vertices = {});

    void append(const Point& point) override;
    void insert(const Point& point, size_t index) override;
    void remove(size_t index) override;
    const std::vector<Point>& get_vertices() const override;

    Point& operator[](size_t index) override;
    const Point& operator[](size_t index) const override;
};


class Polygon : public AbstractPolygon {
private:
    std::vector<Hole> holes;

public:
    Polygon(const std::vector<Point>& vertices = {}, const std::vector<Hole>& holes = {}); // Default arguments

    void append(const Point& point) override;
    void insert(const Point& point, size_t index) override;
    void remove(size_t index) override;
    const std::vector<Point>& get_vertices() const override;

    Point& operator[](size_t index) override;
    const Point& operator[](size_t index) const override;

    void add_hole(const Hole& hole);
    void remove_hole(size_t index);
    const std::vector<Hole>& get_holes() const;
    std::vector<Hole>& get_holes();
};


class Layer {
private:
    std::string name;
    std::vector<Polygon> polygons;

public:
    Layer() : name("Unnamed Layer"), polygons() {}  // Конструктор по умолчанию
    Layer(const char* name) : name(name), polygons() {} // Конструктор с const char*
    Layer(const std::string& name, const std::vector<Polygon>& polygons);
    Layer(const Layer& other) = default;          // Конструктор копирования
    Layer(Layer&& other) noexcept = default;       // Перемещающий конструктор
    Layer& operator=(const Layer& other) = default; // Оператор копирования
    Layer& operator=(Layer&& other) noexcept = default; // Оператор перемещения

    const std::string& get_name() const;
    void rename(const std::string& new_name);
    void append(const Polygon& polygon);
    void insert(const Polygon& polygon, size_t index);
    void remove(size_t index);
    const std::vector<Polygon>& get_polygons() const;

    Polygon& operator[](size_t index);
    const Polygon& operator[](size_t index) const;
};



class LayerPack {
private:
    std::unordered_map<std::string, Layer> layers_by_name;   // Хранит слои по имени
    std::vector<Layer> layers_by_index;                      // Хранит слои по индексу

public:
    // Конструктор
    LayerPack(const std::vector<Layer>& layers = {});

    // Методы управления слоями
    void append_layer(const Layer& layer);
    void insert_layer(const Layer& layer, size_t index);
    void remove_layer(const std::string& name);
    void remove_layer(size_t index);

    // Методы доступа к слоям
    const std::vector<Layer>& get_layers() const;
    std::vector<std::string> get_layers_names() const;
    const std::unordered_map<std::string, Layer>& get_layers_map() const;

    // Перегрузка операторов
    Layer& operator[](size_t index);
    const Layer& operator[](size_t index) const;
    Layer& operator[](const std::string& name);
    const Layer& operator[](const std::string& name) const;
};


#endif // ENTITY_H
