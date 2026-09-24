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

    bool mousePressEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event, QPainter *painter);

private:
    CornerRect *selCorner=NULL;
    SelectRect  selectRect;
    bool fMoveSelectRect = false;
    QPoint c1;
    QPoint c2;

    CornerRect *hitCorner(QPoint pt);
    void drawSelectRect(QPainter *painter);
    void drawRectangle();

};

#endif // RECTANGLEMODE_H
