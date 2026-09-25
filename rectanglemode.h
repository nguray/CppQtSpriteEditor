#ifndef RECTANGLEMODE_H
#define RECTANGLEMODE_H

#include "editmode.h"
#include "selectrect.h"
#include <QPainter>

class RectangleMode : public EditMode
{
public:
    RectangleMode();
    ~RectangleMode();

    void init();
    bool mousePressEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event, QPainter *painter);

private:
    CornerRect *selCorner=NULL;
    SelectRect  selectRect;
    bool fMoveSelectRect = false;
    QPoint startPt;

    CornerRect *hitCorner(QPoint pt);
    void drawSelectRect(QPainter *painter);
    void drawRectangle();
    void fillRectangle();

};

#endif // RECTANGLEMODE_H
