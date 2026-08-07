#include "colorrect.h"

ColorRect::ColorRect()
{}

ColorRect::ColorRect(int x,int y,int s,QColor c):
    QRect(x,y,s,s),color(c)
{}

ColorRect::~ColorRect()
{}

void ColorRect::setColor(QColor c)
{
    color = c;
}

QColor ColorRect::getColor()
{
    return color;
}