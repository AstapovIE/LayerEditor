#include "layereditorwidget.h"

#include <filesystem>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <math.h>


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
            if (isInsideSimplePolygon(hole.get_points(), point)) {
                return false;
            }
        }
        return isInsideSimplePolygon(polygon.get_points(), point);
    }
    
    void movePolygon(Polygon& polygon, Point delta) {
        for (int i=0; i < polygon.get_points().size(); ++i) {
            polygon[i] = polygon[i] + delta;
        }
        for (auto& hole : polygon.get_holes()) {
            for (int i=0; i < hole.get_points().size(); ++i) {
                hole[i] = hole[i] + delta;
            }
        }
    }

    std::string generateFileName(bool isForRedoUndo) {
        std::filesystem::path currentSrcFile = __FILE__;
        std::filesystem::path currentDir = currentSrcFile.parent_path().parent_path().parent_path();
        if (isForRedoUndo) {
            static int redoUndoFileNumber = 0;
            currentDir += "/database/filebuffer";
            return std::string(currentDir.generic_string()) + "/layout_" + std::to_string(redoUndoFileNumber++) + ".json";

        } else {
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);
            std::stringstream transTime;
            transTime << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
            return std::string(currentDir.generic_string()) + "/layout_" + transTime.str() + ".json";
        }
    }

    bool isSegmentsIntersect(Point a, Point b, Point c, Point d) {
        auto area = [](Point a, Point b, Point c) {
            return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        };
        auto intersect2d = [](double a, double b, double c, double d) {
            if (a > b)  swap (a, b);
            if (c > d)  swap (c, d);
            return max(a,c) <= min(b,d);
        };
        return intersect2d(a.x, b.x, c.x, d.x) && intersect2d(a.y, b.y, c.y, d.y)
            && area(a,b,c) * area(a,b,d) <= 0 && area(c,d,a) * area(c,d,b) <= 0;
    }

    bool isPolygonHaveIntesections(const std::vector<Point>& polygon, Point newPoint) {
        if (polygon.size() < 3) return false;
        const auto& firstPoint = polygon.front();
        const auto& lastPoint = polygon.back();
        for (int i=0; i < polygon.size() - 1; ++i) {
            if (i < polygon.size()-2 && isSegmentsIntersect(polygon[i], polygon[i+1], lastPoint, newPoint)) {
                return true;
            }
            if (i > 0 && isSegmentsIntersect(polygon[i], polygon[i+1], firstPoint, newPoint)) {
                return true;
            }
        }
        return false;
    }

    Point getStraightPoint(Point center, Point p) {
        static auto calculateDistance = [](Point a, Point b, Point c) {
            return std::abs((b.y-a.y)*c.x + (a.x-b.x)*c.y + b.x*a.y - b.y*a.x)
                 / std::sqrt((b.y-a.y)*(b.y-a.y) + (b.x-a.x)*(b.x-a.x));
        };

        static auto calculateProjection = [](Point a, Point b) {
            return a.x * b.x + a.y * b.y;
        };

        static const std::vector<Point> lineVectors = {
            Point(1, 0),
            Point(0, 1),
            Point(std::sqrt(2) / 2, std::sqrt(2) / 2),
            Point(std::sqrt(2) / 2, -std::sqrt(2) / 2),
        };

        double minDist = 1e9;
        Point closestLine = lineVectors[0];
        for (const auto& lineVector : lineVectors) {
            double dist = calculateDistance(center, center + lineVector, p);
            if (dist < minDist) {
                closestLine = lineVector;
                minDist = dist;
            }
        }

        Point vec = p - center;
        double projection = calculateProjection(closestLine, vec);
        return center + Point(closestLine.x * projection, closestLine.y * projection);
    }
}


WillLine::WillLine(QGraphicsScene* s)
    : scene(s), drawItem{new QGraphicsPathItem}
{
    scene->addItem(drawItem);
}

WillLine::~WillLine() {
    delete drawItem;
}

void WillLine::addLastPoint(Point p) {
    points.clear();
    points.push_back(p);
}

void WillLine::updateLastPoint(Point p) {
    if (points.size() == 1)
        points.push_back(p);
    else if (points.size() == 2)
        points.back() = p;

    if (points.size() == 2 && isDrawFlag) {
        QPainterPath path;
        path.moveTo(points[0].x, points[0].y);
        path.lineTo(points[1].x, points[1].y);
        path.closeSubpath();
        drawItem->setPath(path);

        scene->removeItem(drawItem);
        scene->addItem(drawItem);
    }
}

void WillLine::clear() {
    points.clear();

    QPainterPath path;
    drawItem->setPath(path);

    scene->removeItem(drawItem);
    scene->addItem(drawItem);
}

QGraphicsPathItem* WillLine::getDrawnItem() {
    return drawItem;
}

void WillLine::setIsDraw(bool f) {
    isDrawFlag = f;
}



LayerEditorWidget::LayerEditorWidget(QWidget* parent)
    : QGraphicsView(parent)
    , scaleFactor(1.0)
    , scene{new QGraphicsScene(this)}
    , layerPack(converter.getLayerPack())
    , willLine(scene)
{
    setMouseTracking(true);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setSceneRect(-1500, -1500, 3000, 3000);
}

void LayerEditorWidget::setSelectedLayer(std::string layerName) {
    willLine.clear();
    selectedPolygon = -1;
    currentLayerName = layerName;
    setCurrentTool(PAN);
    update(false);
}

std::string LayerEditorWidget::getSelectedLayer() const {
    return currentLayerName;
}

void LayerEditorWidget::setCurrentTool(ToolType tool) {
    switch (tool) {
        case PAN:
            setDragMode(QGraphicsView::ScrollHandDrag);
            willLine.setIsDraw(false);
            break;
        case DRAW:
        case DRAW_STRAIGHT:
            setDragMode(QGraphicsView::NoDrag);
            willLine.setIsDraw(true);
            break;
        case MOVE:
        case ERASE:
            setDragMode(QGraphicsView::NoDrag);
            willLine.setIsDraw(false);
            break;
    }

    if ( !((tool == DRAW || tool == DRAW_STRAIGHT) && (currentToolType == DRAW || currentToolType == DRAW_STRAIGHT)) ) {
        willLine.clear();
        isDrawingNewPolygon = true;
    }
    currentToolType = tool;
}

ToolType LayerEditorWidget::getCurrentTool() const {
    return currentToolType;
}

void LayerEditorWidget::update(bool saveForRedoUndo) {
    scene->removeItem(willLine.getDrawnItem());
    scene->clear();

    for (const auto& layer : layerPack.get_layers()) {
        if (layer.get_name() != currentLayerName) {
            drawLayer(layer.get_name(), 50);
        }
    }
    if (!currentLayerName.empty()) {
        drawLayer(currentLayerName);
    }
    scene->addItem(willLine.getDrawnItem());

    if (isAutoSaveModeEnabled) {
        saveAll();
    }
    if (saveForRedoUndo) {
        auto filename = saveAll("", true);
        undoRedoManager.doChanges(QString(filename.c_str()));
    }
}

void LayerEditorWidget::drawLayer(const std::string& layerName, int alpha) {
    for (int i=0; i<layerPack[layerName].get_polygons().size(); ++i) {
        QPainterPath path;
        drawSimplePolygon(path, layerPack[layerName][i].get_points());
        for (const auto& hole : layerPack[layerName][i].get_holes()) {
            drawSimplePolygon(path, hole.get_points());
        }
        path.setFillRule(Qt::OddEvenFill);

        QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);

        int layerIdx = 0;
        for (const auto& layer : layerPack.get_layers()) {
            if (layer.get_name() == layerName)
                break;
            layerIdx++;
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
    if (currentLayerName.empty()) return -1;
    int polygonIdx = -1;
    for (int i=0; i<layerPack[currentLayerName].get_polygons().size(); ++i) {
        if (isInsidePolygon(layerPack[currentLayerName][i], point)) {
            polygonIdx = i;
        }
    }
    return polygonIdx;
}

void LayerEditorWidget::mousePressEvent(QMouseEvent* event) {
    QPointF mousePosQ = mapToScene(event->pos());
    Point mousePos(mousePosQ.x(), mousePosQ.y());

    auto addPoint = [&event, this](Point mousePos) {
        auto& curLayer = this->layerPack[this->currentLayerName];
        if (event->button() == Qt::LeftButton) {
            int polygonsCount = curLayer.get_polygons().size();
            if (!(this->isDrawingNewPolygon)) {
                if (isPolygonHaveIntesections(curLayer[polygonsCount-1].get_points(), mousePos)) {
                    std::cout << "Error: can`t place point here: resulting polygon will have self-intersections" << std::endl;
                } else {
                    curLayer[polygonsCount-1].append(mousePos);
                    willLine.addLastPoint(mousePos);
                }
            } else {
                curLayer.append(Polygon({mousePos}));
                this->isDrawingNewPolygon = false;

                this->willLine.addLastPoint(mousePos);
            }
            this->update();
        } else if (event->button() == Qt::RightButton) {
            this->willLine.clear();
            this->isDrawingNewPolygon = true;
        }
    };

    switch (currentToolType){
        case PAN:
            break;
        case DRAW:
            addPoint(mousePos);
            break;
        case DRAW_STRAIGHT:
            if (event->button() == Qt::LeftButton && !isDrawingNewPolygon) {
                addPoint(getStraightPoint(layerPack[currentLayerName].get_polygons().back().get_points().back(), mousePos));
            } else {
                addPoint(mousePos);
            }
            break;
        case MOVE:
            if (event->button() == Qt::LeftButton) {
                selectedPolygon = insidePolygonIdx(mousePos);
                if (selectedPolygon != -1) {
                    isMovingPolygon = true;
                    update(false);
                }
            }
            break;
        case ERASE:
            if (event->button() == Qt::LeftButton) {
                selectedPolygon = insidePolygonIdx(mousePos);
                if (selectedPolygon != -1) {
                    isDeletingPolygon = true;
                    update(false);
                }
            }
            break;
    }
    QGraphicsView::mousePressEvent(event);
    lastMousePos = mousePos;
}

void LayerEditorWidget::mouseMoveEvent(QMouseEvent* event) {
    QPointF mousePosQ = mapToScene(event->pos());
    Point mousePos(mousePosQ.x(), mousePosQ.y());
    switch (currentToolType) {
        case MOVE:
            if (isMovingPolygon && selectedPolygon != -1) {
                movePolygon(layerPack[currentLayerName][selectedPolygon], mousePos - lastMousePos);
                update(false);
                haveMovedPolygon = true;
            }
            break;
        case DRAW:
            willLine.updateLastPoint(mousePos);
            break;
        case DRAW_STRAIGHT:
            if (!isDrawingNewPolygon && !currentLayerName.empty() && !layerPack[currentLayerName].get_polygons().empty()) {
                const auto& lastPolygon = layerPack[currentLayerName].get_polygons().back().get_points();
                if (!lastPolygon.empty())
                    willLine.updateLastPoint(getStraightPoint(lastPolygon.back(), mousePos));
            }
            break;
        case PAN:
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
            if (haveMovedPolygon) {
                update();
                haveMovedPolygon = false;
            }
            break;
        case ERASE:
            if (isDeletingPolygon && selectedPolygon != -1) {
                layerPack[currentLayerName].remove(selectedPolygon);
                isMovingPolygon = false;
                selectedPolygon = -1;
                update();
            }
            break;
        case PAN:
        case DRAW:
        case DRAW_STRAIGHT:
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
    update(true);
}

void LayerEditorWidget::copyLayer(const std::string& name, const std::string& copyName) {
    Layer layer = layerPack[name];
    layer.rename(copyName);
    layerPack.append_layer(layer);
    update(true);
}

void LayerEditorWidget::deleteLayer(const std::string& name) {
    layerPack.remove_layer(name);
    if (currentLayerName == name) {
        setSelectedLayer("");
    }
    update(true);
}

std::vector<std::string> LayerEditorWidget::getLayerNames() const {
    return layerPack.get_layers_names();
}

void LayerEditorWidget::autoSaveMode(bool isEnabled) {
    isAutoSaveModeEnabled = isEnabled;
    if (isEnabled) {
        saveAll();
    }
}

void LayerEditorWidget::setFile(const std::string& filename) {
    converter.loadJson(filename);
    currentFileName = filename;
//    undoRedoManager.clearHistory();
    update();
}

std::string LayerEditorWidget::saveAll(std::string filename, bool isForRedoUndo) {
    if (isForRedoUndo) {
        filename = generateFileName(true);
    } else if (filename.empty() && !currentFileName.empty()) {
        filename = currentFileName;
    } else if (filename.empty() && currentFileName.empty()) {
        filename = generateFileName(false);
    }
    converter.saveToJson(filename);
    return filename;
}

void LayerEditorWidget::redo() {
    auto filename = undoRedoManager.redo();
    converter.loadJson(filename.toStdString());
    update(false);
}

void LayerEditorWidget::undo() {
    auto filename = undoRedoManager.undo();
    converter.loadJson(filename.toStdString());
    update(false);
}
