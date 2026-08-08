
#pragma once

#include <QRect>
#include <QColor>
#include <QPainter>

class ColorRect : public QRect
{

private:
    /* data */
    QColor color;
    
public:
    ColorRect();
    ColorRect(int x,int y,int s,QColor c);
    ~ColorRect();

    void    setColor(QColor c);
    QColor  getColor();
    void    draw(QPainter *p);
    void    drawFill(QPainter *p);
    void    drawEmpty(QPainter *p);

};

