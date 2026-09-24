
#pragma once
#include "editmode.h"
#include <vector>
#include "pixelrect.h"

class PencilMode : public EditMode
{
public:
    PencilMode();
    ~PencilMode();

    PixelRect lastPixel;
    PixelRect *selVertex=NULL;

    void freePolygon();
    void drawPolygon();
    void drawPolygonVertices(QPainter *painter);
    PixelRect *hitVertex(QPoint pt);

    void toggleFlash();

    bool mousePressEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event, QPainter *painter);

private:
    std::vector<PixelRect *> polygon;

    QColor  vertexHandleColor1;
    QColor  vertexHandleColor2;


};