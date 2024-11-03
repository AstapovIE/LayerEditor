#include "layereditorwidget.h"

bool isInsideSimplePolygon(const std::vector<QPointF>& polygon, QPointF point) {
    QPolygonF qpolygon(QVector<QPointF>(polygon.begin(), polygon.end()));
    return qpolygon.containsPoint(point, Qt::OddEvenFill);
}

bool isInsidePolygon(const Polygon& polygon, QPointF point) {
    for (const auto& hole : polygon.holes) {
        if (isInsideSimplePolygon(hole, point)) {
            return false;
        }
    }
    return isInsideSimplePolygon(polygon.vertices, point);
}

void moveSimplePolygon(std::vector<QPointF>& polygon, QPointF delta) {
    for (auto& point : polygon) {
        point += delta;
    }
}

void movePolygon(Polygon& polygon, QPointF delta) {
    moveSimplePolygon(polygon.vertices, delta);
    for (auto& hole : polygon.holes) {
        moveSimplePolygon(hole, delta);
    }
}


void LayerEditorWidget::setSelectedLayer(std::string layerName) {
    currentLayerName = layerName;
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

