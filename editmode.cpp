
#include "editmode.h"


QColor EditMode::foregroundColor = Qt::blue;
QColor EditMode::backgroundColor = QColor(0,0,0,0);
QSharedPointer<QImage> EditMode::image = QSharedPointer<QImage>::create(32, 32, QImage::Format_ARGB32);
QImage EditMode::image_bak = QImage(32, 32, QImage::Format_ARGB32);

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

QRect EditMode::Pixel2Rect(int px,int py)
{
    int xLeft = px*cellSize + margin;
    int yTop  = py*cellSize + margin;
    return QRect(xLeft,yTop,cellSize,cellSize);
}

void EditMode::setImage(QSharedPointer<QImage> new_image)
{
    image = new_image;
    image_bak = QImage(image->size(),image->format());
    image_bak.fill(QColor(128, 0, 0, 255));
}

void EditMode::backupImage()
{
    image_bak = image->copy();
}

void EditMode::restoreImage()
{

    QPainter painter(image.get());

    // Disable blending by forcing the source pixels to overwrite the destination
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    painter.drawImage(0, 0, image_bak);

    painter.end();

}
