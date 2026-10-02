#ifndef ELLIPSEMODE_H
#define ELLIPSEMODE_H

#include "editmode.h"
#include "selectrect.h"
#include <QPainter>

class EllipseMode : public EditMode
{
public:
    EllipseMode();
    ~EllipseMode();

    void init();
    void updateImage();
    bool mousePressEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event, QPainter *painter);

private:
    CornerRect *selCorner=NULL;
    SelectRect  selectRect;
    bool fMoveSelectRect = false;
    bool fDoNotDrawHandles = false;
    QPoint startPt;

    void drawSelectRect(QPainter *painter);
    void drawEllipse();
    void fillEllipse();

};

#endif // ELLIPSEMODE_H
