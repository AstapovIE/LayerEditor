#ifndef LAYEREDITORWIDGET_H
#define LAYEREDITORWIDGET_H

#include <QWidget>
#include <QPainter>


enum ToolType {
    NONE,
    SELECT,
    DRAW,
    MOVE,
    ERASE
};

class LayerPack;

class LayerEditorWidget : public QWidget {
    Q_OBJECT

public:
//    LayerEditorWidget(LayerPack& layers, QWidget* parent=nullptr);
    LayerEditorWidget(QWidget* parent=nullptr);

    void setSelectedLayer(std::string layerName);
    std::string getSelectedLayer() const;

    void setCurrentTool(ToolType tool);
    ToolType getCurrentTool() const;

protected:
    void paintEvent(QPaintEvent* event) override;

    virtual void mousePressEvent(QMouseEvent* e) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* e) override;
    virtual void mouseMoveEvent(QMouseEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent* e) override;

private:
//    LayerPack& layerPack;

    ToolType currentTool;
    std::string currentLayerName;
};


#endif // LAYEREDITORWIDGET_H
