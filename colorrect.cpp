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

void ColorRect::drawFill(QPainter *p)
{
    auto c = color;
    p->fillRect(*this,c);
}

void ColorRect::drawEmpty(QPainter *p)
{
    int s = width();
    p->setBrush(Qt::NoBrush);
    p->setPen(QPen(QBrush(QColor(0, 0, 0)), 0));
    int left = x();
    int top = y();
    int right = x() + s - 1;
    int bottom = y() + s - 1;
    p->drawLine(left, top, right, top);
    p->drawLine(right, top, right, bottom);
    p->drawLine(right, bottom, left, bottom);
    p->drawLine(left, bottom, left, top);
    p->drawLine(left, top, right, bottom);
    p->drawLine(left, bottom, right, top);

}

void ColorRect::draw(QPainter *p)
{
    if (color.alpha()>0){
        drawFill(p);
    }else{
        drawEmpty(p);
    }

}