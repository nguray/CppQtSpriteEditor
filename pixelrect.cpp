#include "pixelrect.h"


PixelRect::PixelRect():
    QRect(),pix_x(0),pix_y(0)
{

}

PixelRect::PixelRect(PixelRect &pr):
    QRect(pr.left(),pr.top(),pr.width(),pr.height()),pix_x(pr.pix_x),pix_y(pr.pix_y)
{

}

PixelRect::PixelRect(int px,int py):
    QRect(),pix_x(px),pix_y(py)
{

}


PixelRect::~PixelRect()
{

}

