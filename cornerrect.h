#ifndef CORNERRECT_H
#define CORNERRECT_H

#include <QRect>

class CornerRect : public QRect
{
public:
    CornerRect(int *ix,int *iy);

    int *x;
    int *y;

};

#endif // CORNERRECT_H
