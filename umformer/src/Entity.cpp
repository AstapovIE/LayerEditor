#include "Entity.h"


// Класс Point
Point::Point(double x, double y) : x(x), y(y) {}

Point Point::operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
}

Point Point::operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
}

Point Point::operator*(double num) const {
    return Point(x * num, y * num);
}

Point Point::operator/(double num) const {
    if (std::abs(num) < EPSILON) {
        throw std::invalid_argument("Division by zero error: Attempted to divide " +
                                    static_cast<std::string>(*this) + " by " + format_double(num));
    }
    return Point(x / num, y / num);
}

Point& Point::operator*=(double num) {
    x *= num;
    y *= num;
    return *this;
}

Point& Point::operator/=(double num) {
    if (std::abs(num) < EPSILON) {
        throw std::invalid_argument("Division by zero error: Attempted to divide " +
                                    static_cast<std::string>(*this) + " by " + format_double(num));
    }
    x /= num;
    y /= num;
    return *this;
}

bool Point::operator==(const Point& other) const {
    return (std::abs(x - other.x) < EPSILON) && (std::abs(y - other.y) < EPSILON);
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}

std::unordered_map<std::string, double> Point::ravel() const {
    return {{"x", x}, {"y", y}};
}

Point::operator std::string() const {
    return "Point(" + format_double(x) + ", " + format_double(y) + ")";
}

// Форматирование числа с плавающей точкой
std::string Point::format_double(double num) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(OUTPUT_PRECISION) << num;
    std::string result = stream.str();

    // Убираем лишние нули в конце числа
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);

    // Если последний символ - точка, убираем ее
    if (result.back() == '.') {
        result.pop_back();
    }

    return result;
}

std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << static_cast<std::string>(point);
    return os;
}


// AbstractPolygon
AbstractPolygon::AbstractPolygon(const std::vector<Point>& points)
    : points(points) {}


void AbstractPolygon::append(const Point& point) {
    points.push_back(point);
}

void AbstractPolygon::insert(const Point& point, size_t index) {
    if (index > points.size()) {
        throw std::out_of_range("Index out of bounds: " + std::to_string(index) +
                                ". Valid range: from 0 to " + std::to_string(points.size()));
    }
    points.insert(points.begin() + index, point);
}

void AbstractPolygon::remove(size_t index) {
    if (index >= points.size()) {
        throw std::out_of_range("Index out of bounds: " + std::to_string(index) +
                                ". Valid range: from 0 to " + std::to_string(points.size() - 1));
    }
    points.erase(points.begin() + index);
}

const std::vector<Point>& AbstractPolygon::get_points() const {
    return points;
}

Point& AbstractPolygon::operator[](size_t index) {
    if (index >= points.size()) {
        throw std::out_of_range("Index out of bounds: " + std::to_string(index) +
                                ". Valid range: 0 to " + std::to_string(points.size() - 1));
    }
    return points[index];
}

const Point& AbstractPolygon::operator[](size_t index) const {
    if (index >= points.size()) {
        throw std::out_of_range("Index out of bounds: " + std::to_string(index) +
                                ". Valid range: 0 to " + std::to_string(points.size() - 1));
    }
    return points[index];
}

bool AbstractPolygon::operator==(const AbstractPolygon& other) const {
    return this->points == other.points;
}

bool AbstractPolygon::operator!=(const AbstractPolygon& other) const {
    return !(*this == other);
}

AbstractPolygon::operator std::string() const {
    std::string result = "Points: [";
    for (size_t i = 0; i < points.size(); ++i) {
        result += static_cast<std::string>(points[i]);
        if (i < points.size() - 1) {
            result += ", ";
        }
    }
    result += "]";
    return result;
}

std::ostream& operator<<(std::ostream& os, const AbstractPolygon& abstract_polygon) {
    os << static_cast<std::string>(abstract_polygon);
    return os;
}


// Hole
Hole::Hole(const std::vector<Point>& points)
    : AbstractPolygon(points) {}


// Polygon
Polygon::Polygon(const std::vector<Point>& points, const std::vector<Hole>& holes)
    : AbstractPolygon(points), holes(holes) {}

void Polygon::add_hole(const Hole& hole) {
    holes.push_back(hole);
}

void Polygon::remove_hole(size_t index) {
    if (index >= holes.size()) {
        throw std::out_of_range("Hole index out of bounds: " + std::to_string(index) +
                                ". Valid range: from 0 to " + std::to_string(holes.size() - 1));
    }
    holes.erase(holes.begin() + index);
}

const std::vector<Hole>& Polygon::get_holes() const {
    return holes;
}

std::vector<Hole>& Polygon::get_holes() {
    return holes;
}

bool Polygon::operator==(const Polygon& other) const {
    return AbstractPolygon::operator==(other) && this->holes == other.holes;
}

bool Polygon::operator!=(const Polygon& other) const {
    return !(*this == other);
}

Polygon::operator std::string() const {
    std::string result = static_cast<std::string>(*this);
    if (!holes.empty()) {
        result += ", Holes: [";
        for (size_t i = 0; i < holes.size(); ++i) {
            result += static_cast<std::string>(holes[i]);
            if (i < holes.size() - 1) {
                result += ", ";
            }
        }
        result += "]";
    }
    return result;
}


// Layer
Layer::Layer(const std::string& name, const std::vector<Polygon>& polygons)
    : name(name), polygons(polygons) {}

const std::string& Layer::get_name() const {
    return name;
}

void Layer::rename(const std::string& new_name) {
    name = new_name;
}

void Layer::append(const Polygon& polygon) {
    polygons.push_back(polygon);
}

void Layer::remove(size_t index) {
    if (index >= polygons.size()) {
        throw std::out_of_range("Index out of bounds: " + std::to_string(index) +
                                ". Valid range: from 0 to " + std::to_string(polygons.size() - 1));
    }
    polygons.erase(polygons.begin() + index);
}

const std::vector<Polygon>& Layer::get_polygons() const {
    return polygons;
}

const Polygon& Layer::operator[](size_t index) const {
    if (index >= polygons.size()) {
        throw std::out_of_range("Index out of bounds: " + std::to_string(index) +
                                ". Valid range: from 0 to " + std::to_string(polygons.size() - 1));
    }
    return polygons[index];
}

Polygon& Layer::operator[](size_t index) {
    if (index >= polygons.size()) {
        throw std::out_of_range("Index out of bounds: " + std::to_string(index) +
                                ". Valid range: from 0 to " + std::to_string(polygons.size() - 1));
    }
    return polygons[index];
}

bool Layer::operator==(const Layer& other) const {
    return this->name == other.name && this->polygons == other.polygons;
}

bool Layer::operator!=(const Layer& other) const {
    return !(*this == other);
}

Layer::operator std::string() const {
    std::string result = this->get_name();
    if (!polygons.empty()) {
        result += " | Polygons: [";
        for (size_t i = 0; i < polygons.size()-1; ++i) {
            result += static_cast<std::string>(polygons[i]) = ", ";
        }
        result += static_cast<std::string>(polygons[polygons.size()-1]);
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Layer& layer) {
    os << static_cast<std::string>(layer);
    return os;
}


LayerPack::LayerPack(const std::vector<Layer>& layers) : layers(layers) {}

// Вспомогательная функция для поиска слоя по имени
size_t LayerPack::find_layer_index(const std::string& name) const {
    for (size_t i = 0; i < layers.size(); ++i) {
        if (layers[i].get_name() == name) {
            return i;  // Возвращаем индекс как int
        }
    }
    throw std::out_of_range("Layer '" + name + "' not found in LayerPack.");
}

void LayerPack::append_layer(const Layer& layer) {
    layers.push_back(layer);
}

void LayerPack::remove_layer(const std::string& name) {
    int layer_index = find_layer_index(name);
    layers.erase(layers.begin() + layer_index);
}

const std::vector<Layer>& LayerPack::get_layers() const {
    return layers;
}

std::vector<Layer>& LayerPack::get_layers() {
    return layers;
}

std::vector<std::string> LayerPack::get_layers_names() const {
    std::vector<std::string> names;
    for (const auto& layer : layers) {
        names.push_back(layer.get_name());
    }
    return names;
}

size_t LayerPack::size() const {
    return layers.size();
}

Layer& LayerPack::operator[](const std::string& name) {
    int layer_index = find_layer_index(name);
    return layers[layer_index];
}

const Layer& LayerPack::operator[](const std::string& name) const {
    int layer_index = find_layer_index(name);
    return layers[layer_index];
}

bool LayerPack::operator==(const LayerPack& other) const {
    return this->layers == other.layers;
}

bool LayerPack::operator!=(const LayerPack& other) const {
    return !(*this == other);
}

LayerPack::operator std::string() const {
    std::string result;
    if (!layers.empty()) {
        result += "Layers: [";
        for (size_t i = 0; i < layers.size()-1; ++i) {
            result += static_cast<std::string>(layers[i]) = ", ";
        }
        result += static_cast<std::string>(layers[layers.size()-1]);
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const LayerPack& layer_pack) {
    os << static_cast<std::string>(layer_pack);
    return os;
}
