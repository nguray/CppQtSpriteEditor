#include "rectanglemode.h"

RectangleMode::RectangleMode()
{

}

RectangleMode::~RectangleMode()
{

}

bool RectangleMode::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        if (selectRect.contains(event->position().toPoint())){
            auto pix = Pos2Pixel(event->position().toPoint());
            c1 = pix;

            selectRect.BackupSubImageRect();
            fMoveSelectRect = true;
            return false;
        }else{
            auto pix = Pos2Pixel(event->position().toPoint());
            QRect r = image->rect();
            if (r.contains(pix)){ // Keep actions inside image limits
                if (selectRect.isNull()){
                    selectRect.setSubImageRect(pix.x(),pix.y(),pix.x(),pix.y());
                    c1 = pix;
                    return true;
                }else{

                }

            }
        }
    }else if (event->button() == Qt::RightButton) {

        selectRect.setSubImageNULL();
        selectRect.setRect(0,0,0,0);
        selectRect.fDefined = false;
        return true;

    }
    return false;

}

bool RectangleMode::mouseMoveEvent(QMouseEvent *event)
{

    if ((event->buttons() & Qt::LeftButton)){

        auto pix = Pos2Pixel(event->position().toPoint());
        QRect r = image->rect();


        if (fMoveSelectRect){
            if (c1!=pix){
                int dx = pix.x()-c1.x();
                int dy = pix.y()-c1.y();
                int tLeft  = selectRect.subImageLeftBak + dx;
                int tRight = selectRect.subImageRightBak + dx;
                int tTop   = selectRect.subImageTopBak + dy;
                int tBottom   = selectRect.subImageBottomBak + dy;
                if (r.contains(QPoint(tLeft,tTop)) && r.contains(QPoint(tRight,tBottom))){
                    selectRect.setSubImageRect(tLeft,tTop,tRight,tBottom);
                    return true;
                }
            }
        }else if (!selectRect.fDefined){

            if (r.contains(pix)){ // Keep actions inside image limits
                int l,t,r,b;
                if (pix.x()>c1.x()){
                    l = c1.x();
                    r = pix.x();
                }else{
                    l = pix.x();
                    r = c1.x();
                }
                if (pix.y()>c1.y()){
                    t = c1.y();
                    b = pix.y();
                }else{
                    t = pix.y();
                    b = c1.y();
                }
                selectRect.setSubImageRect(l,t,r,b);

                return true;
            }
        }

    }

    return false;

}

bool RectangleMode::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton)){
        fMoveSelectRect = false;
        selectRect.fDefined = true;

    }
    return false;
}


void RectangleMode::drawSelectRect(QPainter *painter)
{
    //-- Draw Rect frame
    int xLeft = selectRect.subImageLeft*cellSize + margin;
    int yTop  = selectRect.subImageTop*cellSize + margin;
    int xRight = selectRect.subImageRight*cellSize + margin + cellSize;
    int yBottom = selectRect.subImageBottom*cellSize + margin + cellSize;

    selectRect.setRect(xLeft,yTop,xRight-xLeft,yBottom-yTop);
    painter->setPen(QPen(QColor(255,0,0,255), 1.0, Qt::SolidLine, Qt::RoundCap,
                         Qt::RoundJoin));
    painter->drawRect(selectRect);

    //-- Draw corner handles
    painter->setBrush(QBrush(QColor(255,0,0,255)));

    //--TopLeft
    selectRect.corners[0]->setCoords(xLeft-5,yTop-5,xLeft+4,yTop+4);
    painter->drawRect(*selectRect.corners[0]);

    //--TopRight
    selectRect.corners[1]->setCoords(xRight-5,yTop-5,xRight+4,yTop+4);
    painter->drawRect(*selectRect.corners[1]);

    //--BottomRight
    selectRect.corners[2]->setCoords(xRight-5,yBottom-5,xRight+4,yBottom+4);
    painter->drawRect(*selectRect.corners[2]);

    //--BottomRight
    selectRect.corners[3]->setCoords(xLeft-5,yBottom-5,xLeft+4,yBottom+4);
    painter->drawRect(*selectRect.corners[3]);


}

void RectangleMode::paintEvent(QPaintEvent *event, QPainter *painter)
{
    if (!selectRect.isSubImageNULL()){
        drawSelectRect(painter);
    }
}
