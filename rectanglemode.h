#ifndef RECTANGLEMODE_H
#define RECTANGLEMODE_H

#include "editmode.h"

class RectangleMode : public EditMode
{
public:
    RectangleMode();
    ~RectangleMode();

    bool mousePressEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);


};

#endif // RECTANGLEMODE_H
