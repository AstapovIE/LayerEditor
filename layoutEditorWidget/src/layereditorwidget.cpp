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
    currentToolType = tool;
}

ToolType LayerEditorWidget::getCurrentTool() const {
    return currentToolType;
}

//void LayerEditorWidget::paintEvent(QPaintEvent* event) {
//    QPainter painter(this);

//    painter.setBrush(Qt::blue);
//    painter.drawRect(200, 100, 150, 70);

//    painter.setBrush(Qt::red);
//    painter.drawEllipse(160, 130, 70, 70);

//    painter.setBrush(Qt::green);
//    painter.drawRect(300, 140, 90, 110);
//}

//void LayerEditorWidget::mousePressEvent(QMouseEvent* e) {}
//void LayerEditorWidget::mouseDoubleClickEvent(QMouseEvent* e) {}
//void LayerEditorWidget::mouseMoveEvent(QMouseEvent* e) {}
//void LayerEditorWidget::mouseReleaseEvent(QMouseEvent* e) {}
