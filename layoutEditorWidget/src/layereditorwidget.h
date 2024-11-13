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
#include "umformer/src/umformer.h"
#include "database/src/UndoRedoManager.h"


enum ToolType{
    PAN,
    DRAW,
    DRAW_STRAIGHT,
    MOVE,
    ERASE
};


class WillLine{
public:
    WillLine(QGraphicsScene* s);
    ~WillLine();

    void addLastPoint(Point p);
    void updateLastPoint(Point p);
    void clear();
    QGraphicsPathItem* getDrawnItem();
    void setIsDraw(bool f);

private:
    QGraphicsScene* scene;

    QGraphicsPathItem* drawItem;
    std::vector<Point> points;
    bool isDrawFlag = true;
};


class LayerEditorWidget : public QGraphicsView {
public:
    LayerEditorWidget(QWidget* parent = nullptr);
    virtual ~LayerEditorWidget();

    void setSelectedLayer(std::string layerName);
    std::string getSelectedLayer() const;

    void setCurrentTool(ToolType tool);
    ToolType getCurrentTool() const;

    void redo();
    void undo();

    void setFile(const std::string& filename);
    std::string saveAll(std::string filename = "", bool isForRedoUndo = false);

    void addLayer(const std::string& name);
    void copyLayer(const std::string& name, const std::string& copyName);
    void deleteLayer(const std::string& name);

    void autoSaveMode(bool isEnabled);

    std::vector<std::string> getLayerNames() const;

    void update(bool saveForRedoUndo = true);

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

    Converter converter;
    LayerPack& layerPack;

    UndoRedoManager undoRedoManager;

    WillLine willLine;

    qreal scaleFactor = 1.0;
    bool isDrawingNewPolygon = true;
    int selectedPolygon = -1;

    bool isMovingPolygon = false;
    bool haveMovedPolygon = false;
    Point lastMousePos = Point(0,0);

    bool isDeletingPolygon = false;

    std::string currentLayerName;
    ToolType currentToolType = PAN;

    bool isAutoSaveModeEnabled = false;
    std::string currentFileName;

    const std::vector<QColor> layerColors = {
        Qt::green, Qt::blue, Qt::red, Qt::cyan, Qt::magenta, Qt::yellow,
        Qt::darkGreen, Qt::darkBlue, Qt::darkRed, Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow
    };
};



#endif // LAYEREDITORWIDGET_H
