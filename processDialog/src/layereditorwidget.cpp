#include "layereditorwidget.h"

//LayerEditorWidget::LayerEditorWidget(LayerPack& layers, QWidget* parent)
//    : QWidget(parent), layerPack{layers}
//{}
LayerEditorWidget::LayerEditorWidget(QWidget* parent)
    : QWidget(parent) {}

void LayerEditorWidget::setSelectedLayer(std::string layerName) {
    currentLayerName = layerName;
}

std::string LayerEditorWidget::getSelectedLayer() const {
    return currentLayerName;
}

void LayerEditorWidget::setCurrentTool(ToolType tool) {
    currentTool = tool;
}

ToolType LayerEditorWidget::getCurrentTool() const {
    return currentTool;
}

void LayerEditorWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.setBrush(Qt::blue);
    painter.drawRect(200, 100, 150, 70);

    painter.setBrush(Qt::red);
    painter.drawEllipse(160, 130, 70, 70);

    painter.setBrush(Qt::green);
    painter.drawRect(300, 140, 90, 110);
}

void LayerEditorWidget::mousePressEvent(QMouseEvent* e) {}
void LayerEditorWidget::mouseDoubleClickEvent(QMouseEvent* e) {}
void LayerEditorWidget::mouseMoveEvent(QMouseEvent* e) {}
void LayerEditorWidget::mouseReleaseEvent(QMouseEvent* e) {}
