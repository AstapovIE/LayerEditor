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


enum ToolType{
    SELECT,
    DRAW,
    MOVE,
    ERASE
};

struct Polygon {
    std::vector<QPointF> vertices;
    std::vector<std::vector<QPointF>> holes;
};

bool isInsideSimplePolygon(const std::vector<QPointF>& polygon, QPointF point);
bool isInsidePolygon(const Polygon& polygon, QPointF point);

void moveSimplePolygon(std::vector<QPointF>& polygon, QPointF delta);
void movePolygon(Polygon& polygon, QPointF delta);

class LayerEditorWidget : public QGraphicsView {
public:
    LayerEditorWidget(QWidget* parent = nullptr)
        : QGraphicsView(parent)
        , scaleFactor(1.0)
        , scene{new QGraphicsScene(this)}
    {
        setScene(scene);
        setRenderHint(QPainter::Antialiasing);
        setSceneRect(-1500, -1500, 3000, 3000);
    }

    void setSelectedLayer(std::string layerName);
    std::string getSelectedLayer() const;

    void setCurrentTool(ToolType tool);
    ToolType getCurrentTool() const;

    void update() {
        scene->clear();

        for (int i=0; i<polygons.size(); ++i) {
            QPainterPath path;
            drawSimplePolygon(path, polygons[i].vertices);
            for (const auto& hole : polygons[i].holes) {
                drawSimplePolygon(path, hole);
            }
            path.setFillRule(Qt::OddEvenFill);

            QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
            if (i == selectedPolygon) {
                pathItem->setBrush(QBrush(Qt::yellow));
            } else {
                pathItem->setBrush(QBrush(Qt::green));
            }
            scene->addItem(pathItem);
        }
    }

    virtual ~LayerEditorWidget() {
        delete scene;
    }

private:
    void drawSimplePolygon(QPainterPath& path, const std::vector<QPointF>& polygon) {
        if (polygon.empty()) return;
        path.moveTo(polygon[0]);
        for (int i=1; i<polygon.size(); ++i) {
            path.lineTo(polygon[i]);
        }
        path.closeSubpath();
    }

    int insidePolygonIdx(QPointF point) {
        for (int i=0; i<polygons.size(); ++i) {
            if (isInsidePolygon(polygons[i], point)) {
                return i;
            }
        }
        return -1;
    }

protected:
    void mousePressEvent(QMouseEvent* event) override {
        QPointF mousePos = mapToScene(event->pos());
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
                    if (!isDrawingNewPolygon) {
                        polygons.back().vertices.push_back(mousePos);
                    } else if (holeDrawingPolygon != -1) {
                        if (isInsidePolygon(polygons[holeDrawingPolygon], mousePos)) {
                            polygons[holeDrawingPolygon].holes.back().push_back(mousePos);
                        } else {
                            std::cout << "cant place point here" << std::endl;
                        }
                    } else {
                        holeDrawingPolygon = insidePolygonIdx(mousePos);
                        if (holeDrawingPolygon == -1) {
                            polygons.push_back(Polygon());
                            polygons.back().vertices.push_back(mousePos);
                            isDrawingNewPolygon = false;
                        } else {
                            polygons[holeDrawingPolygon].holes.push_back({});
                            polygons[holeDrawingPolygon].holes.back().push_back(mousePos);
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

    void mouseMoveEvent(QMouseEvent* event) override {
        QPointF mousePos = mapToScene(event->pos());
        switch (currentToolType) {
            case MOVE: {
                if (isMovingPolygon && selectedPolygon != -1) {
                    movePolygon(polygons[selectedPolygon], mousePos - lastMousePos);
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

    void mouseReleaseEvent(QMouseEvent* event) override {
        QPointF mousePos = mapToScene(event->pos());
        switch (currentToolType) {
            case MOVE:
                isMovingPolygon = false;
                break;
            case ERASE: {
                if (isDeletingPolygon && selectedPolygon != -1) {
                    polygons.erase(polygons.begin() + selectedPolygon);
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

    void wheelEvent(QWheelEvent* event) override {
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

private:
    QGraphicsScene* scene;

    qreal scaleFactor;

    ToolType currentToolType;

    std::vector<Polygon> polygons;
    bool isDrawingNewPolygon = true;
    int holeDrawingPolygon = -1;
    int selectedPolygon = -1;

    bool isMovingPolygon = false;
    QPointF lastMousePos;

    bool isDeletingPolygon = false;

    std::string currentLayerName;
};



#endif // LAYEREDITORWIDGET_H
