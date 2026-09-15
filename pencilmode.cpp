
#include "pencilmode.h"


PencilMode::PencilMode()
{

}

PencilMode::~PencilMode()
{

}

bool PencilMode::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        auto pix = Pos2Pixel(event->position().toPoint());

        if ((pix.x()>=0)&&(pix.x()<image.width())&&
             (pix.y()>=0)&&(pix.y()<image.height())){
            image.setPixelColor(pix.x(),pix.y(),foregroundColor);
            return true;
        }

        qDebug() << pix.x() << "," << pix.y();

    }else if (event->buttons() & Qt::RightButton){
        auto pix = Pos2Pixel(event->position().toPoint());

        if ((pix.x()>=0)&&(pix.x()<image.width())&&
             (pix.y()>=0)&&(pix.y()<image.height())){
            image.setPixelColor(pix.x(),pix.y(),backgroundColor);
            return true;
        }
    }

    return false;

}

bool PencilMode::mouseMoveEvent(QMouseEvent *event)
{
   if ((event->buttons() & Qt::LeftButton)){
        auto pix = Pos2Pixel(event->position().toPoint());

        if ((pix.x()>=0)&&(pix.x()<image.width())&&
             (pix.y()>=0)&&(pix.y()<image.height())){
            image.setPixelColor(pix.x(),pix.y(),foregroundColor);
            return true;
        }

    }else if ((event->buttons() & Qt::RightButton)){
        auto pix = Pos2Pixel(event->position().toPoint());

        if ((pix.x()>=0)&&(pix.x()<image.width())&&
             (pix.y()>=0)&&(pix.y()<image.height())){
            image.setPixelColor(pix.x(),pix.y(),backgroundColor);
            return true;
        }
    }

    return false;
}

bool PencilMode::mouseReleaseEvent(QMouseEvent *event)
{

    return false;
}

void PencilMode::paintEvent(QPaintEvent *event)
{

}
