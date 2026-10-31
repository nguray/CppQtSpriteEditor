#include "pixelrect.h"


PixelRect::PixelRect():
    QRect(),pix(QPoint())
{

}

PixelRect::PixelRect(PixelRect &pr):
    QRect(pr.left(),pr.top(),pr.width(),pr.height()),pix(pr.pix)
{

}

PixelRect::PixelRect(QPoint p):
    QRect(),pix(p)
{

}


PixelRect::~PixelRect()
{

}

