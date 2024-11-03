#ifndef LAYEREDITORWIDGET_H
#define LAYEREDITORWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QPolygonF>
#include <QStyleOptionGraphicsItem>

#include <iostream>

#include "umformer/src/Entity.h"
//#include "umformer/src/Converter.h"


enum ToolType{
    SELECT,
    DRAW,
    MOVE,
    ERASE
};


class LayerEditorWidget : public QGraphicsView {
public:
    LayerEditorWidget(QWidget* parent = nullptr);
    virtual ~LayerEditorWidget();

    void setSelectedLayer(std::string layerName);
    std::string getSelectedLayer() const;

    void setCurrentTool(ToolType tool);
    ToolType getCurrentTool() const;

    void setFile(const std::string& filename);

    void addLayer(const std::string& name);
    void copyLayer(const std::string& name, const std::string& copyName);
    void deleteLayer(const std::string& name);

    void autoSaveMode(bool isEnabled);

    std::vector<std::string> getLayerNames() const;

    void saveAll(std::string filename = "");

    void update();

private:
    void drawSimplePolygon(QPainterPath& path, const std::vector<Point>& polygon);
    int insidePolygonIdx(Point point);

    void drawLayer(const std::string& layerName, int alpha = 255);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    QGraphicsScene* scene;

//    Converter converter;
//    LayerPack& layerPack;
    LayerPack layerPack;

    qreal scaleFactor = 1.0;
    bool isDrawingNewPolygon = true;
    int holeDrawingPolygon = -1;
    int selectedPolygon = -1;

    bool isMovingPolygon = false;
    Point lastMousePos;

    bool isDeletingPolygon = false;

    std::string currentLayerName;
    ToolType currentToolType;

    bool isAutoSaveModeEnabled = false;

    const std::vector<QColor> layerColors = {
        Qt::green, Qt::blue, Qt::red, Qt::cyan, Qt::magenta, Qt::yellow, Qt::black
    };
    const QColor selectColor = Qt::lightGray;
};



#endif // LAYEREDITORWIDGET_H
