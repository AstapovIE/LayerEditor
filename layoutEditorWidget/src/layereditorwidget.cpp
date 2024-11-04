#include "layereditorwidget.h"

#include <iostream>
#include <iomanip>
#include <ctime>


namespace {
    bool isInsideSimplePolygon(const std::vector<Point>& polygon, Point point) {
        QPolygonF qpolygon;
        for (const auto& point : polygon) {
            qpolygon.append(QPointF(point.x, point.y));
        }
        return qpolygon.containsPoint(QPointF(point.x, point.y), Qt::OddEvenFill);
    }
    
    bool isInsidePolygon(const Polygon& polygon, Point point) {
        for (const auto& hole : polygon.get_holes()) {
            if (isInsideSimplePolygon(hole.get_vertices(), point)) {
                return false;
            }
        }
        return isInsideSimplePolygon(polygon.get_vertices(), point);
    }
    
    void movePolygon(Polygon& polygon, Point delta) {
        for (int i=0; i < polygon.get_vertices().size(); ++i) {
            polygon[i] = polygon[i] + delta;
        }
        for (auto& hole : polygon.get_holes()) {
            for (int i=0; i < hole.get_vertices().size(); ++i) {
                hole[i] = hole[i] + delta;
            }
        }
    }
}


LayerEditorWidget::LayerEditorWidget(QWidget* parent)
    : QGraphicsView(parent)
    , scaleFactor(1.0)
    , scene{new QGraphicsScene(this)}
    , layerPack(converter.getLayerPack())
{
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setSceneRect(-1500, -1500, 3000, 3000);
}

void LayerEditorWidget::setSelectedLayer(std::string layerName) {
    selectedPolygon = -1;
    currentLayerName = layerName;
    update();
}

std::string LayerEditorWidget::getSelectedLayer() const {
    return currentLayerName;
}

void LayerEditorWidget::setCurrentTool(ToolType tool) {
    if (tool == SELECT) {
        setDragMode(QGraphicsView::ScrollHandDrag);
    } else {
        setDragMode(QGraphicsView::NoDrag);
    }

    isDrawingNewPolygon = true;
    holeDrawingPolygon = -1;
    currentToolType = tool;
}

ToolType LayerEditorWidget::getCurrentTool() const {
    return currentToolType;
}

void LayerEditorWidget::update() {
    scene->clear();

    for (const auto& [layerName, Layer] : layerPack.get_layers_map()) {
        if (layerName != currentLayerName) {
            drawLayer(layerName, 50);
        }
    }
    drawLayer(currentLayerName);

    saveAll();
}

void LayerEditorWidget::drawLayer(const std::string& layerName, int alpha) {
    for (int i=0; i<layerPack[layerName].get_polygons().size(); ++i) {
        QPainterPath path;
        drawSimplePolygon(path, layerPack[layerName][i].get_vertices());
        for (const auto& hole : layerPack[layerName][i].get_holes()) {
            drawSimplePolygon(path, hole.get_vertices());
        }
        path.setFillRule(Qt::OddEvenFill);

        QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);

        int layerIdx = 0;
        for (int i=0; i<layerPack.get_layers().size(); ++i) {
            if (layerPack[i].get_name() == layerName) {
                layerIdx = i;
                break;
            }
        }
        QColor color = layerColors[layerIdx % layerColors.size()];
        color.setAlpha(alpha);
        if (i == selectedPolygon && layerName == currentLayerName) {
            color = color.darker(120);
        }
        pathItem->setBrush(QBrush(color));

        scene->addItem(pathItem);
    }
}


LayerEditorWidget::~LayerEditorWidget() {
    delete scene;
}

void LayerEditorWidget::drawSimplePolygon(QPainterPath& path, const std::vector<Point>& polygon) {
    if (polygon.empty()) return;
    path.moveTo(polygon[0].x, polygon[0].y);
    for (int i=1; i<polygon.size(); ++i) {
        path.lineTo(polygon[i].x, polygon[i].y);
    }
    path.closeSubpath();
}

int LayerEditorWidget::insidePolygonIdx(Point point) {
    for (int i=0; i<layerPack[currentLayerName].get_polygons().size(); ++i) {
        if (isInsidePolygon(layerPack[currentLayerName][i], point)) {
            return i;
        }
    }
    return -1;
}

void LayerEditorWidget::mousePressEvent(QMouseEvent* event) {
    QPointF mousePosQ = mapToScene(event->pos());
    Point mousePos(mousePosQ.x(), mousePosQ.y());
    switch (currentToolType){
        case SELECT: {
            if (event->button() == Qt::LeftButton) {
                selectedPolygon = insidePolygonIdx(mousePos);
                if (selectedPolygon != -1) {
                    update();
                }
            }
            break;
        }
        case DRAW: {
            if (event->button() == Qt::LeftButton) {
                int polygonsCount = layerPack[currentLayerName].get_polygons().size();
                if (!isDrawingNewPolygon) {
                    layerPack[currentLayerName][polygonsCount-1].append(mousePos);
				} else if (holeDrawingPolygon != -1) {
                    if (isInsidePolygon(layerPack[currentLayerName][holeDrawingPolygon], mousePos)) {
                        layerPack[currentLayerName][holeDrawingPolygon].get_holes().back().append(mousePos);
                    } else {
                        std::cout << "cant place point here" << std::endl;
                    }
                } else {
                    holeDrawingPolygon = insidePolygonIdx(mousePos);
                    if (holeDrawingPolygon == -1) {
                        layerPack[currentLayerName].append(Polygon({mousePos}));
                        isDrawingNewPolygon = false;
                    } else {
                        layerPack[currentLayerName][holeDrawingPolygon].add_hole(Hole({mousePos}));
                    }
                }

                update();
            } else if (event->button() == Qt::RightButton) {
                isDrawingNewPolygon = true;
                holeDrawingPolygon = -1;
            }
            break;
        }
        case MOVE: {
            if (event->button() == Qt::LeftButton) {
                selectedPolygon = insidePolygonIdx(mousePos);
                if (selectedPolygon != -1) {
                    isMovingPolygon = true;
                    update();
                }
            }
            break;
        }
        case ERASE: {
            if (event->button() == Qt::LeftButton) {
                selectedPolygon = insidePolygonIdx(mousePos);
                if (selectedPolygon != -1) {
                    isDeletingPolygon = true;
                    update();
                }
            }
            break;
        }
    }
    QGraphicsView::mousePressEvent(event);
    lastMousePos = mousePos;
}

void LayerEditorWidget::mouseMoveEvent(QMouseEvent* event) {
    QPointF mousePosQ = mapToScene(event->pos());
    Point mousePos(mousePosQ.x(), mousePosQ.y());
    switch (currentToolType) {
        case MOVE: {
            if (isMovingPolygon && selectedPolygon != -1) {
                movePolygon(layerPack[currentLayerName][selectedPolygon], mousePos - lastMousePos);
                update();
            }
        }
        case SELECT:
        case DRAW:
        case ERASE:
            break;
    }
    QGraphicsView::mouseMoveEvent(event);
    lastMousePos = mousePos;
}

void LayerEditorWidget::mouseReleaseEvent(QMouseEvent* event) {
    QPointF mousePosQ = mapToScene(event->pos());
    Point mousePos(mousePosQ.x(), mousePosQ.y());
    switch (currentToolType) {
        case MOVE:
            isMovingPolygon = false;
            break;
        case ERASE: {
            if (isDeletingPolygon && selectedPolygon != -1) {
                layerPack[currentLayerName].remove(selectedPolygon);
                isMovingPolygon = false;
                selectedPolygon = -1;
                update();
            }
            break;
        }
        case SELECT:
        case DRAW:
            break;
    }
    QGraphicsView::mouseReleaseEvent(event);
    lastMousePos = mousePos;
}

void LayerEditorWidget::wheelEvent(QWheelEvent* event) {
    const qreal scaleFactorStep = 1.15;
    const qreal minScaleFactor = 0.3333;
    const qreal maxScaleFactor = 5;

    if (event->angleDelta().y() > 0 && scaleFactor <= maxScaleFactor) {
        scale(scaleFactorStep, scaleFactorStep);
        scaleFactor *= scaleFactorStep;
    } else if (event->angleDelta().y() < 0 && scaleFactor >= minScaleFactor) {
        scale(1 / scaleFactorStep, 1 / scaleFactorStep);
        scaleFactor /= scaleFactorStep;
    }
}

void LayerEditorWidget::addLayer(const std::string& name) {
    layerPack.append_layer(Layer(name.c_str()));
}

void LayerEditorWidget::copyLayer(const std::string& name, const std::string& copyName) {
    Layer layer = layerPack[name];
    layer.rename(copyName);
    layerPack.append_layer(layer);
}

void LayerEditorWidget::deleteLayer(const std::string& name) {
    layerPack.remove_layer(name);
}

std::vector<std::string> LayerEditorWidget::getLayerNames() const {
    return layerPack.get_layers_names();
}

void LayerEditorWidget::autoSaveMode(bool isEnabled) {
    isAutoSaveModeEnabled = isEnabled;
}

void LayerEditorWidget::setFile(const std::string& filename) {
//    converter.loadJson(filename);
    converter.loadJson("/home/k1ps/QtProjects/LayerEditor/database/filebuffer/layout_in.json");
}

void LayerEditorWidget::saveAll(std::string filename) {
    if (filename.empty()) {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::stringstream transTime;
        transTime << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
        filename = "layerpack_" + transTime.str();
    }
    filename = "/home/k1ps/QtProjects/LayerEditor/database/filebuffer/layout_out.json";
    converter.saveToJson(filename);
}

