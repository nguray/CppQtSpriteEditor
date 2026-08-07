
#pragma once

#include <QRect>
#include <QColor>

class ColorRect : public QRect
{

private:
    /* data */
    QColor color;
    
public:
    ColorRect();
    ColorRect(int x,int y,int s,QColor c);
    ~ColorRect();

    void setColor(QColor c);
    QColor getColor();

};

