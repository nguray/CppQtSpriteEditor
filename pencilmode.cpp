
#include "pencilmode.h"


PencilMode::PencilMode()
{

}

PencilMode::~PencilMode()
{
    for (auto p:listPts){
        delete p;
    }
    listPts.clear();

}


bool PencilMode::mousePressEvent(QMouseEvent *event)
{
    
    if (event->button() == Qt::LeftButton) {
        auto pix = Pos2Pixel(event->position().toPoint());

        if ((pix.x()>=0)&&(pix.x()<image.width())&&
             (pix.y()>=0)&&(pix.y()<image.height())){

            if (event->modifiers() & Qt::ShiftModifier) {
                if (listPts.size()){
                    QPainter painter(&EditMode::image);
                    painter.setPen(QPen(EditMode::foregroundColor, 1.0, Qt::SolidLine, Qt::RoundCap,
                                        Qt::RoundJoin));
                    auto lastPt = listPts.back();
                    painter.drawLine(*lastPt, pix);
                }
            }else{
                image.setPixelColor(pix.x(),pix.y(),foregroundColor);
            }

            qDebug() << pix.x() << "," << pix.y();
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

    }

    return false;
}

bool PencilMode::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton)){
        // Check if the Shift key was held down at the moment of release
        auto pix = Pos2Pixel(event->position().toPoint());
        if ((pix.x()>=0)&&(pix.x()<image.width())&&
            (pix.y()>=0)&&(pix.y()<image.height())){
            if (listPts.size()==0){
                auto pt = new QPoint(pix.x(),pix.y());
                listPts.push_back(pt);
            }else{
                auto lastPt = listPts.back();
                lastPt->setX(pix.x());
                lastPt->setY(pix.y());
            }
        }

    }
    return false;
}

void PencilMode::paintEvent(QPaintEvent *event)
{

}
