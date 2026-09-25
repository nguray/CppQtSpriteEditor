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
            selectRect.BackupPixLimits();
            fMoveSelectRect = true;
            return false;
        }else{
            QRect r = image->rect();
            if (r.contains(pix)){ // Keep actions inside image limits
                if (!selectRect.fDefined){
                    backupImage();
                    selectRect.setPixLimits(pix.x(),pix.y(),pix.x(),pix.y());
                    c1 = pix;
                    return true;
                }else{
                    backupImage();
                    selectRect.setPixNULL();
                    selectRect.setRect(0,0,0,0);
                    selectRect.fDefined = false;
                    selectRect.setPixLimits(pix.x(),pix.y(),pix.x(),pix.y());
                    c1 = pix;
                    return true;
                }

            }
        }
    }else if (event->button() == Qt::RightButton) {

        selectRect.setPixNULL();
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
    painter.drawRect(selectRect.getPixRect());
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
                int tLeft  = selectRect.pixLeftBak + dx;
                int tRight = selectRect.pixRightBak + dx;
                int tTop   = selectRect.pixTopBak + dy;
                int tBottom   = selectRect.pixBottomBak + dy;
                if (r.contains(QPoint(tLeft,tTop)) && r.contains(QPoint(tRight,tBottom))){
                    selectRect.setPixLimits(tLeft,tTop,tRight,tBottom);
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
                if ((selectRect.pixLeft<selectRect.pixRight) &&
                    (selectRect.pixTop<selectRect.pixBottom)){
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
                selectRect.setPixLimits(l,t,r,b);
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
        selectRect.fDefined = !selectRect.isPixNULL();
    }
    return false;
}


void RectangleMode::drawSelectRect(QPainter *painter)
{
    //-- Draw Rect frame
    int xLeft = selectRect.pixLeft*cellSize + margin;
    int yTop  = selectRect.pixTop*cellSize + margin;
    int xRight = selectRect.pixRight*cellSize + margin + cellSize;
    int yBottom = selectRect.pixBottom*cellSize + margin + cellSize;

    selectRect.setRect(xLeft,yTop,xRight-xLeft,yBottom-yTop);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(0,0,64,64), 1.0, Qt::SolidLine, Qt::RoundCap,
                         Qt::RoundJoin));
    painter->drawRect(selectRect);

    //-- Draw corner handles
    painter->setBrush(QBrush(QColor(0,0,128,255)));

    //--TopLeft
    selectRect.corners[0]->setCoords(xLeft-5,yTop-5,xLeft+5,yTop+5);
    painter->drawRect(*selectRect.corners[0]);

    //--TopRight
    selectRect.corners[1]->setCoords(xRight-5,yTop-5,xRight+5,yTop+5);
    painter->drawRect(*selectRect.corners[1]);

    //--BottomRight
    selectRect.corners[2]->setCoords(xRight-5,yBottom-5,xRight+5,yBottom+5);
    painter->drawRect(*selectRect.corners[2]);

    //--BottomRight
    selectRect.corners[3]->setCoords(xLeft-5,yBottom-5,xLeft+5,yBottom+5);
    painter->drawRect(*selectRect.corners[3]);

}

void RectangleMode::paintEvent(QPaintEvent *event, QPainter *painter)
{
    if (!selectRect.isPixNULL()){
        drawSelectRect(painter);
    }
}
