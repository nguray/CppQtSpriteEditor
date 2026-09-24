#include "rectanglemode.h"

RectangleMode::RectangleMode()
{

}

RectangleMode::~RectangleMode()
{

}

CornerRect *RectangleMode::hitCorner(QPoint pt)
{
    CornerRect *c;
    for (auto i=0;i<4;i++){
        if ((c=selectRect.corners[i]) && (c->contains(pt))){
            return c;
        }
    }

    return NULL;
}


bool RectangleMode::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton) {


        QPoint pt = event->position().toPoint();
        auto pix = Pos2Pixel(pt);

        if (selCorner=hitCorner(pt)){
            c1 = pix;
            return false;
        }else if (selectRect.contains(pt)){
            c1 = pix;
            selectRect.BackupSubImageRect();
            fMoveSelectRect = true;
            return false;
        }else{
            QRect r = image->rect();
            if (r.contains(pix)){ // Keep actions inside image limits
                if (selectRect.isNull()){
                    backupImage();
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

void RectangleMode::drawRectangle()
{
    QPainter painter(EditMode::image.get());
    painter.setPen(QPen(EditMode::foregroundColor, 1.0, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawRect(QRect(selectRect.subImageLeft,selectRect.subImageTop,
                            selectRect.subImageRight-selectRect.subImageLeft,
                            selectRect.subImageBottom-selectRect.subImageTop));
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
                    restoreImage();
                    drawRectangle();
                    return true;
                }
            }
        }else if (selCorner){
            if (r.contains(pix)){ // Keep actions inside image limits
                int savX = *(selCorner->x);
                int savY = *(selCorner->y);
                *(selCorner->x) = pix.x();
                *(selCorner->y) = pix.y();
                if ((selectRect.subImageLeft<selectRect.subImageRight) &&
                    (selectRect.subImageTop<selectRect.subImageBottom)){
                    restoreImage();
                    drawRectangle();
                    return true;
                }else{
                    *(selCorner->x) = savX;
                    *(selCorner->y) = savY;
                    return false;
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
                restoreImage();
                drawRectangle();
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
        selCorner = NULL;
        selectRect.fDefined = !selectRect.isSubImageNULL();
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
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(0,0,64,64), 1.0, Qt::SolidLine, Qt::RoundCap,
                         Qt::RoundJoin));
    painter->drawRect(selectRect);

    //-- Draw corner handles
    painter->setBrush(QBrush(QColor(0,0,128,255)));

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
