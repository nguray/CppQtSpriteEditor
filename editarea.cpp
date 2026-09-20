
#include "editarea.h"

EditArea::EditArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    

    EditMode::image = QImage(32, 32, QImage::Format_ARGB32);
    EditMode::image.fill(QColor(0, 0, 0, 0));


    pencilMode = new PencilMode();
    rectangleMode = new RectangleMode();
    ellipseMode = new EllipseMode();

    setPencilMode();


    // QPainter painter(&EditMode::image);
    // painter.setPen(QPen(EditMode::foregroundColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
    //                     Qt::RoundJoin));
    // painter.drawLine(QPoint(0,0), QPoint(31,31));



}

EditArea::~EditArea()
{
    if (pencilMode) delete pencilMode;
    if (rectangleMode) delete rectangleMode;
    if (ellipseMode) delete ellipseMode;

}

void EditArea::drawPixels(QPainter *painter)
{
    int w = EditMode::image.width();
    int h = EditMode::image.height();
    int s = EditMode::cellSize - 2;
    for (int row = 0; row < h; row++)
    {
        for (int col = 0; col < w; col++)
        {
            
            QColor c = EditMode::image.pixelColor(col,row);

            if (c.alpha()){
                int x = EditMode::margin + col*EditMode::cellSize+1;
                int y = EditMode::margin + row*EditMode::cellSize+1;

                painter->setBrush(QBrush(c));
                painter->setPen(QPen(c));
                painter->drawRect(x,y,s,s);
            }
        }
        
    }
    
}

void EditArea::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    if (curEditMode->mousePressEvent(event)){
        update();
    }

}

void EditArea::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
    if (curEditMode->mouseMoveEvent(event)){
        update();
    }

}

void EditArea::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    if (curEditMode->mouseReleaseEvent(event)){
        update();
    }

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
    int xLeft = EditMode::margin;
    int yTop = EditMode::margin;
    painter->setPen(QPen(myGridColor, 0.2, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));
    for (int i=0;i<=EditMode::image.height();i++){
        for (int j=0;j<=EditMode::image.width();j++){
            x = xLeft + j * EditMode::cellSize;
            y = yTop + i * EditMode::cellSize;

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
    EditMode::cellSize = (minDim-4) / EditMode::image.width();

    painter.fillRect(r,QColor(220,200,220));

    drawGrid(&painter);

    drawPixels(&painter);

    painter.drawImage(QRect(EditMode::image.width()*EditMode::cellSize+10,4,32,32), EditMode::image, QRect(0,0,32,32));


    curEditMode->paintEvent(event);

}

void EditArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&EditMode::image);
    painter.setPen(QPen(EditMode::foregroundColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
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

void EditArea::setForegroundColor(QColor newColor) {
    EditMode::foregroundColor = newColor;
}

void EditArea::setBackgroundColor(QColor newColor) {
    EditMode::backgroundColor = newColor;
}

void EditArea::setPencilMode()
{
    curEditMode = pencilMode;
}

void EditArea::setRectangleMode()
{
    curEditMode = rectangleMode;
}

void EditArea::setEllipseMode()
{
    curEditMode = ellipseMode;
}
