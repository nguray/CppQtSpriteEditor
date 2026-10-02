#include "ellipsemode.h"

EllipseMode::EllipseMode()
{

}

EllipseMode::~EllipseMode()
{

}

void EllipseMode::init()
{
    mousePointer = Qt::ArrowCursor;
    selectRect.setPixNULL();
    selectRect.setRect(0,0,0,0);
    selectRect.fDefined = false;
    backupImage();

}

void EllipseMode::updateImage()
{
    restoreImage();
    if (fShiftKey){
        fillEllipse();
    }else{
        drawEllipse();
    }

}


bool EllipseMode::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        QPoint pt = event->position().toPoint();
        auto pix = Pos2Pixel(pt);

        if (selCorner=selectRect.hitCorner(pt)){
            startPt = pix;
            fDoNotDrawHandles = true;
            return false;
        }else if (selectRect.contains(pt)){
            startPt = pix;
            selectRect.BackupPixLimits();
            fMoveSelectRect = true;
            fDoNotDrawHandles = true;
            return false;
        }else{
            QRect r = image->rect();
            if (r.contains(pix)){ // Keep actions inside image limits
                if (!selectRect.fDefined){
                    backupImage();
                    selectRect.setPixLimits(pix.x(),pix.y(),pix.x(),pix.y());
                    startPt = pix;
                    return true;
                }else{
                    backupImage();
                    init();
                    selectRect.setPixLimits(pix.x(),pix.y(),pix.x(),pix.y());
                    startPt = pix;
                    return true;
                }

            }
            fDoNotDrawHandles = false;
        }
    }else if (event->button() == Qt::RightButton) {
        init();
        return true;

    }
    return false;

}

void EllipseMode::drawEllipse()
{
    QPainter painter(EditMode::image.get());
    painter.setPen(QPen(EditMode::foregroundColor, 1.0, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawEllipse(selectRect.getPixRect());
}

void EllipseMode::fillEllipse()
{
    QPainter painter(EditMode::image.get());
    painter.setBrush(QBrush(EditMode::foregroundColor));
    painter.setPen(QPen(EditMode::foregroundColor, 1.0, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawEllipse(selectRect.getPixRect());
}

bool EllipseMode::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pt = event->position().toPoint();
    auto pix = Pos2Pixel(pt);

    if ((event->buttons() & Qt::LeftButton)){

        QRect r = image->rect();

        if (fMoveSelectRect){
            if (startPt!=pix){
                int dx = pix.x()-startPt.x();
                int dy = pix.y()-startPt.y();
                int tLeft  = selectRect.pixLeftBak + dx;
                int tRight = selectRect.pixRightBak + dx;
                int tTop   = selectRect.pixTopBak + dy;
                int tBottom   = selectRect.pixBottomBak + dy;
                if (r.contains(QPoint(tLeft,tTop)) && r.contains(QPoint(tRight,tBottom))){
                    selectRect.setPixLimits(tLeft,tTop,tRight,tBottom);
                    updateImage();
                    fDoNotDrawHandles = true;
                    return true;
                }
            }
        }else if (selCorner){
            if (r.contains(pix)){ // Keep actions inside image limits
                QPoint v = pix - startPt;
                if (v.x()||v.y()){
                    int savX = *(selCorner->x);
                    int savY = *(selCorner->y);
                    *(selCorner->x) = startPt.x() + v.x();
                    *(selCorner->y) = startPt.y() + v.y();
                    if ((selectRect.pixLeft<selectRect.pixRight) &&
                        (selectRect.pixTop<selectRect.pixBottom)){
                        updateImage();
                        fDoNotDrawHandles = true;
                        return true;
                    }else{
                        *(selCorner->x) = savX;
                        *(selCorner->y) = savY;
                        return false;
                    }
                }
            }
        }else if (!selectRect.fDefined){

            if (r.contains(pix)){ // Keep actions inside image limits
                int l,t,r,b;
                if (pix.x()>startPt.x()){
                    l = startPt.x();
                    r = pix.x();
                }else{
                    l = pix.x();
                    r = startPt.x();
                }
                if (pix.y()>startPt.y()){
                    t = startPt.y();
                    b = pix.y();
                }else{
                    t = pix.y();
                    b = startPt.y();
                }
                selectRect.setPixLimits(l,t,r,b);
                updateImage();
                fDoNotDrawHandles = true;
                return true;
            }
        }


    }else{

        if (auto c = selectRect.hitCorner(pt)){
            mousePointer = c->cursorShape;
        }else if (selectRect.contains(pt)){
            mousePointer = Qt::SizeAllCursor;
        }else{
            mousePointer = Qt::ArrowCursor;
        }

    }

    return false;

}

bool EllipseMode::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton)){
        fMoveSelectRect = false;
        selCorner = NULL;
        selectRect.fDefined = !selectRect.isPixNULL();
    }
    fDoNotDrawHandles = false;
    mousePointer = Qt::ArrowCursor;
    return true;
}


void EllipseMode::paintEvent(QPaintEvent *event, QPainter *painter)
{
    if (!selectRect.isPixNULL()){
        selectRect.draw(painter,margin,cellSize,!fDoNotDrawHandles);
    }
}

