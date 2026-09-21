
#include "editmode.h"


QColor EditMode::foregroundColor = Qt::blue;
QColor EditMode::backgroundColor = QColor(0,0,0,0);
QImage EditMode::image = QImage(32, 32, QImage::Format_ARGB32);

bool EditMode::fShiftKey = false;
int EditMode::margin = 4; 
int EditMode::cellSize = 10;


EditMode::EditMode()
{

}

EditMode::~EditMode()
{

}

QPoint EditMode::Pos2Pixel(QPoint p)
{
    int x = (p.x()-margin) / cellSize;
    int y = (p.y()-margin) / cellSize;
    return QPoint(x,y);
}

