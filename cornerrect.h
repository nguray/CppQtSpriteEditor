#ifndef CORNERRECT_H
#define CORNERRECT_H

#include <QRect>

class CornerRect : public QRect
{
public:
    CornerRect(int *ix,int *iy,int dx,int dy,Qt::CursorShape curshape);

    int *x;
    int *y;
    int offsetX=0;
    int offsetY=0;
    Qt::CursorShape cursorShape=Qt::ArrowCursor;

};

#endif // CORNERRECT_H
