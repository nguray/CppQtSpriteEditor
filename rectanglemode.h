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
    SelectRect  selectRect;
    bool fMoveSelectRect = false;
    QPoint c1;
    QPoint c2;

    void drawSelectRect(QPainter *painter);


};

#endif // RECTANGLEMODE_H
