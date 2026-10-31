#ifndef SELECTRECTMODE_H
#define SELECTRECTMODE_H

#include "editmode.h"
#include <QPainter>
#include "selectrect.h"

class SelectRectMode : public EditMode
{
public:
    SelectRectMode();
    ~SelectRectMode();

    void init();
    void updateImage();
    bool mousePressEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event, QPainter *painter);

private:
    CornerRect *selCorner=NULL;
    bool fMoveSelectRect = false;
    bool fDoNotDrawHandles = false;
    QPoint startPt;

};

#endif // SELECTRECTMODE_H
