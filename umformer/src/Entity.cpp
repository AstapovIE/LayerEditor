#include "Entity.h"
#include <unordered_map>
#include <stdexcept>


// Реализация класса Point
Point::Point(double x, double y) : x(x), y(y) {}

Point Point::operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
}

Point Point::operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
}

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}


std::unordered_map<std::string, double> Point::ravel() const {
    return {{"x", x}, {"y", y}};
}

// Реализация класса Hole
Hole::Hole(const std::vector<Point>& vertices) {
    for (const auto& vertex : vertices) {
        append(vertex);
    }
}

void Hole::append(const Point& point) {
    vertices.push_back(point);
}

void Hole::insert(const Point& point, size_t index) {
    if (index >= vertices.size()) {
        throw std::out_of_range("Индекс выходит за пределы допустимого диапазона");
    }
    vertices.insert(vertices.begin() + index, point);
}

void Hole::remove(size_t index) {
    if (index >= vertices.size()) {
        throw std::out_of_range("Индекс " + std::to_string(index) + " выходит за пределы допустимого диапазона [0, " + std::to_string(vertices.size() - 1) + "]");
    }
    vertices.erase(vertices.begin() + index);
}


const std::vector<Point>& Hole::get_vertices() const {
    return vertices;
}

Point& Hole::operator[](size_t index) {
    if (index >= vertices.size()) {
        throw std::out_of_range("Индекс выходит за пределы допустимого диапазона");
    }
    return vertices[index];
}

const Point& Hole::operator[](size_t index) const {
    if (index >= vertices.size()) {
        throw std::out_of_range("Индекс выходит за пределы допустимого диапазона");
    }
    return vertices[index];
}

Polygon::Polygon(const std::vector<Point>& vertices, const std::vector<Hole>& holes) {
    for (const auto& vertex : vertices) {
        append(vertex);
    }
    this->holes = holes;
}

void Polygon::append(const Point& point) {
    vertices.push_back(point);
}

void Polygon::insert(const Point& point, size_t index) {
    if (index >= vertices.size()) {
        throw std::out_of_range("Индекс выходит за пределы допустимого диапазона");
    }
    vertices.insert(vertices.begin() + index, point);
}

void Polygon::remove(size_t index) {
    if (index >= vertices.size()) {
        throw std::out_of_range("Индекс " + std::to_string(index) + " выходит за пределы допустимого диапазона [0, " + std::to_string(vertices.size() - 1) + "]");
    }
    vertices.erase(vertices.begin() + index);
}


const std::vector<Point>& Polygon::get_vertices() const {
    return vertices;
}

Point& Polygon::operator[](size_t index) {
    if (index >= vertices.size()) {
        throw std::out_of_range("Индекс выходит за пределы допустимого диапазона");
    }
    return vertices[index];
}

const Point& Polygon::operator[](size_t index) const {
    if (index >= vertices.size()) {
        throw std::out_of_range("Индекс выходит за пределы допустимого диапазона");
    }
    return vertices[index];
}

void Polygon::add_hole(const Hole& hole) {
    holes.push_back(hole);
}

void Polygon::remove_hole(size_t index) {
    if (index >= holes.size()) {
        throw std::out_of_range("Индекс выходит за пределы допустимого диапазона");
    }
    holes.erase(holes.begin() + index);
}

const std::vector<Hole>& Polygon::get_holes() const {
    return holes;
}

std::vector<Hole> &Polygon::get_holes()
{
    return holes;
}

Layer::Layer(const std::string& name, const std::vector<Polygon>& polygons)
    : name(name), polygons(polygons) {
    // Здесь можно добавить валидацию имени, если нужно
    if (name.empty()) {
        throw std::invalid_argument("Имя слоя не может быть пустым");
    }
}

const std::string& Layer::get_name() const {
    return name;
}

void Layer::rename(const std::string& new_name) {
    if (new_name.empty()) {
        throw std::invalid_argument("Новое имя слоя не может быть пустым");
    }
    name = new_name;
}

void Layer::append(const Polygon& polygon) {
    polygons.push_back(polygon);
}

void Layer::insert(const Polygon& polygon, size_t index) {
    if (index >= polygons.size()) {
        throw std::out_of_range("Индекс выходит за пределы допустимого диапазона");
    }
    polygons.insert(polygons.begin() + index, polygon);
}

void Layer::remove(size_t index) {
    if (index >= polygons.size()) {
        throw std::out_of_range("Индекс выходит за пределы допустимого диапазона");
    }
    polygons.erase(polygons.begin() + index);
}

const std::vector<Polygon>& Layer::get_polygons() const {
    return polygons;
}

Polygon& Layer::operator[](size_t index) {
    if (index >= polygons.size()) {
        throw std::out_of_range("Индекс выходит за пределы допустимого диапазона");
    }
    return polygons[index];
}

const Polygon& Layer::operator[](size_t index) const {
    if (index >= polygons.size()) {
        throw std::out_of_range("Индекс выходит за пределы допустимого диапазона");
    }
    return polygons[index];
}

LayerPack::LayerPack(const std::vector<Layer>& layers) {
    for (const auto& layer : layers) {
        append_layer(layer);
    }
}

void LayerPack::append_layer(const Layer& layer) {
    std::string layer_name = layer.get_name();
    
    // Проверка на существование слоя с тем же именем
    if (layers_by_name.find(layer_name) != layers_by_name.end()) {
        throw std::invalid_argument("Слой с именем \"" + layer_name + "\" уже существует.");
    }

    layers_by_name.emplace(layer_name, layer);  // Используем emplace
    layers_by_index.push_back(layer);
}

void LayerPack::insert_layer(const Layer& layer, size_t index) {
    if (index > layers_by_index.size()) {
        throw std::out_of_range("Индекс выходит за границы");
    }

    std::string layer_name = layer.get_name();

    // Проверка на существование слоя с тем же именем
    if (layers_by_name.find(layer_name) != layers_by_name.end()) {
        throw std::invalid_argument("Слой с именем \"" + layer_name + "\" уже существует.");
    }

    layers_by_index.insert(layers_by_index.begin() + index, layer);
    layers_by_name[layer_name] = layer;  // Обновление словаря имен
}

void LayerPack::remove_layer(const std::string& name) {
    auto it = layers_by_name.find(name);
    if (it == layers_by_name.end()) {
        throw std::out_of_range("Слой с таким именем не найден");
    }

    // Получите индекс слоя в layers_by_index
    size_t index = layers_by_index.size(); // Инициализируем так, чтобы гарантированно выйти за пределы
    for (size_t i = 0; i < layers_by_index.size(); ++i) {
        if (layers_by_index[i].get_name() == name) {
            index = i; // Найден индекс
            break;
        }
    }

    if (index == layers_by_index.size()) {
        throw std::out_of_range("Слой с таким именем не найден в списке слоев");
    }

    // Удалите слой из обеих структур
    layers_by_index.erase(layers_by_index.begin() + index);
    layers_by_name.erase(it);
}

void LayerPack::remove_layer(size_t index) {
    if (index >= layers_by_index.size()) {
        throw std::out_of_range("Индекс выходит за границы");
    }

    layers_by_name.erase(layers_by_index[index].get_name());
    layers_by_index.erase(layers_by_index.begin() + index);
}

// Методы доступа
const std::vector<Layer>& LayerPack::get_layers() const {
    return layers_by_index;
}

std::vector<std::string> LayerPack::get_layers_names() const {
    std::vector<std::string> names;
    for (const auto& pair : layers_by_name) {
        names.push_back(pair.first);
    }
    return names;
}

const std::unordered_map<std::string, Layer>& LayerPack::get_layers_map() const {
    return layers_by_name;
}

// Перегрузка операторов
Layer& LayerPack::operator[](size_t index) {
    if (index >= layers_by_index.size()) {
        throw std::out_of_range("Индекс выходит за границы");
    }
    return layers_by_index[index];
}

const Layer& LayerPack::operator[](size_t index) const {
    if (index >= layers_by_index.size()) {
        throw std::out_of_range("Индекс выходит за границы");
    }
    return layers_by_index[index];
}

Layer& LayerPack::operator[](const std::string& name) {
    auto it = layers_by_name.find(name);
    if (it == layers_by_name.end()) {
        throw std::out_of_range("Слой с таким именем не найден");
    }
    return it->second;
}

const Layer& LayerPack::operator[](const std::string& name) const {
    auto it = layers_by_name.find(name);
    if (it == layers_by_name.end()) {
        throw std::out_of_range("Слой с таким именем не найден");
    }
    return it->second;
}