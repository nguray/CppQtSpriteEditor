#include "rectanglemode.h"

RectangleMode::RectangleMode()
{

}

RectangleMode::~RectangleMode()
{

}

bool RectangleMode::mousePressEvent(QMouseEvent *event)
{
    // if (event->button() == Qt::LeftButton) {
    //     auto pix = Pos2Pixel(event->position().toPoint());

    //     if ((pix.x()>=0)&&(pix.x()<image.width())&&
    //         (pix.y()>=0)&&(pix.y()<image.height())){
    //         image.setPixelColor(pix.x(),pix.y(),foregroundColor);
    //         return true;
    //     }

    //     qDebug() << pix.x() << "," << pix.y();

    // }else if (event->buttons() & Qt::RightButton){
    //     auto pix = Pos2Pixel(event->position().toPoint());

    //     if ((pix.x()>=0)&&(pix.x()<image.width())&&
    //         (pix.y()>=0)&&(pix.y()<image.height())){
    //         image.setPixelColor(pix.x(),pix.y(),backgroundColor);
    //         return true;
    //     }
    // }

    return false;

}

bool RectangleMode::mouseMoveEvent(QMouseEvent *event)
{
    // if ((event->buttons() & Qt::LeftButton)){
    //     auto pix = Pos2Pixel(event->position().toPoint());

    //     if ((pix.x()>=0)&&(pix.x()<image.width())&&
    //         (pix.y()>=0)&&(pix.y()<image.height())){
    //         image.setPixelColor(pix.x(),pix.y(),foregroundColor);
    //         return true;
    //     }

    // }else if ((event->buttons() & Qt::RightButton)){
    //     auto pix = Pos2Pixel(event->position().toPoint());

    //     if ((pix.x()>=0)&&(pix.x()<image.width())&&
    //         (pix.y()>=0)&&(pix.y()<image.height())){
    //         image.setPixelColor(pix.x(),pix.y(),backgroundColor);
    //         return true;
    //     }
    // }

    return false;
}

bool RectangleMode::mouseReleaseEvent(QMouseEvent *event)
{

    return false;
}

void RectangleMode::paintEvent(QPaintEvent *event, QPainter *painter)
{

}
