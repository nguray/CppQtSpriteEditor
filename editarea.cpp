
#include "editarea.h"

EditArea::EditArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    
    image = QImage(32, 32, QImage::Format_ARGB32);
    image.fill(QColor(255, 0, 255));


    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(QPoint(0,0), QPoint(31,31));


}

void EditArea::drawPixels(QPainter *painter)
{
    int w = image.width();
    int h = image.height();
    int s = cellSize - 2;
    for (int row = 0; row < h; row++)
    {
        for (int col = 0; col < w; col++)
        {
            
            QColor c = image.pixel(col,row);
            int x = margin + col*cellSize+1;
            int y = margin + row*cellSize+1;

            painter->setBrush(QBrush(c));
            painter->drawRect(x,y,s,s);

        }
        
    }
    
}

QPoint EditArea::Pos2Pixel(QPoint p)
{
    int x = (p.x()-margin) / cellSize;
    int y = (p.y()-margin) / cellSize;
    return QPoint(x,y);
}


void EditArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        auto pix = Pos2Pixel(event->position().toPoint());

        if ((pix.x()>=0)&&(pix.x()<image.width())&&
             (pix.y()>=0)&&(pix.y()<image.height())){
            image.setPixelColor(pix.x(),pix.y(),myPenColor);
            update();
        }

        scribbling = true;
        qDebug() << pix.x() << "," << pix.y();
    }
}

void EditArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling){
        auto pix = Pos2Pixel(event->position().toPoint());

        if ((pix.x()>=0)&&(pix.x()<image.width())&&
             (pix.y()>=0)&&(pix.y()<image.height())){
            image.setPixelColor(pix.x(),pix.y(),myPenColor);
            update();
        }

    }
}

void EditArea::mouseReleaseEvent(QMouseEvent *event)
{
    // if (event->button() == Qt::LeftButton && scribbling) {
    //     drawLineTo(event->position().toPoint());
    //     scribbling = false;
    // }
}

void EditArea::resizeEvent(QResizeEvent *event)
{
    // if (width() > image.width() || height() > image.height()) {
    //     int newWidth = qMax(width() + 128, image.width());
    //     int newHeight = qMax(height() + 128, image.height());
    //     resizeImage(&image, QSize(newWidth, newHeight));
    //     update();
    //     qDebug() << "Resize Event";
    // }
    QWidget::resizeEvent(event);
}

void EditArea::drawGrid(QPainter *painter)
{
    int x,y;
    int xLeft = margin;
    int yTop = margin;
    painter->setPen(QPen(myGridColor, 0.2, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));
    for (int i=0;i<=image.height();i++){
        for (int j=0;j<=image.width();j++){
            x = xLeft + j * cellSize;
            y = yTop + i * cellSize;

            //painter->drawRect(QRect(x+1,y+1,3,3));
            //painter->drawPoint(QPoint(x,y));
            painter->drawLine(QPoint(x-2,y), QPoint(x+2,y));    
            painter->drawLine(QPoint(x,y-2), QPoint(x,y+2));    
        }
    }

}

void EditArea::paintEvent(QPaintEvent *event)
{ 
    QPainter painter(this);

    QRect r = event->rect();
    int minDim = r.height();
    if (minDim>r.width()){
        minDim = r.width();
    }
    cellSize = (minDim-4) / image.width();

    painter.fillRect(r,QColor(200,200,200));

    drawGrid(&painter);

    drawPixels(&painter);

    painter.drawImage(QRect(image.width()*cellSize+10,4,32,32), image, QRect(0,0,32,32));

}

void EditArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void EditArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}