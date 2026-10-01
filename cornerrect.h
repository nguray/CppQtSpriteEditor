#ifndef CORNERRECT_H
#define CORNERRECT_H

#include <QRect>

class CornerRect : public QRect
{
public:
    CornerRect(int *ix,int *iy,int dx,int dy);

    int *x;
    int *y;
    int offsetX=0;
    int offsetY=0;

};

#endif // CORNERRECT_H
