#ifndef PIXELRECT_H
#define PIXELRECT_H

#include <QRect>

class PixelRect : public QRect
{
public:
    PixelRect();
    PixelRect(PixelRect &pr);
    PixelRect(int px,int py);
    ~PixelRect();
    int pix_x;
    int pix_y;
};

#endif // PIXELRECT_H
