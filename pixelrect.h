#ifndef PIXELRECT_H
#define PIXELRECT_H

#include <QRect>
#include <QPoint>

class PixelRect : public QRect
{
public:
    PixelRect();
    PixelRect(PixelRect &pr);
    PixelRect(QPoint p);
    ~PixelRect();
    QPoint pix;
};

#endif // PIXELRECT_H
