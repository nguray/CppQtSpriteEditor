#ifndef ELLIPSEMODE_H
#define ELLIPSEMODE_H

#include "editmode.h"

class EllipseMode : public EditMode
{
public:
    EllipseMode();
    ~EllipseMode();

    bool mousePressEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event, QPainter *painter);


};

#endif // ELLIPSEMODE_H
