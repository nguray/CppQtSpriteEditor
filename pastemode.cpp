/*--------------------------------------------------------------*\
  FILE........: pastemode.cpp
  AUTHOR......: Raymond NGUYEN THANH
  DATE CREATED: 31.10.2026
  DESCRIPTION :
  PasteMode class implementation for SpriteEditor.
  This class handles the paste action,
\*--------------------------------------------------------------*/

#include "pastemode.h"

PasteMode::PasteMode() {}


PasteMode::~PasteMode()
{

}

void PasteMode::pasteImage()
{
    QPainter painter(EditMode::image.get());
    int w = selectRect->pixRight - selectRect->pixLeft + 1;
    int h = selectRect->pixBottom - selectRect->pixTop + 1;
    painter.drawImage(QRect(selectRect->pixLeft,selectRect->pixTop,w,h), image_cpy, image_cpy.rect());

}

void PasteMode::init()
{
    mousePointer = Qt::ArrowCursor;
    int cpy_w = image_cpy.size().width();
    int cpy_h = image_cpy.size().height();

    selectRect->pixTop = 0;
    selectRect->pixLeft = 0;
    selectRect->pixRight = cpy_w - 1;
    selectRect->pixBottom = cpy_h - 1;
    selectRect->fDefined = true;
    backupImage();
    pasteImage();

}

void PasteMode::updateImage()
{
    restoreImage();
    pasteImage();

}



bool PasteMode::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton) {

        QPointF canvasPos = EditMode::canvasInverseTransform.map(event->position());
        auto pt = canvasPos.toPoint();
        auto pix = Pos2Pixel(pt);

        if (selCorner=selectRect->hitCorner(pt)){
            startPt = pix;
            fDoNotDrawHandles = true;
            return false;
        }else if (selectRect->contains(pt)){
            startPt = pix;
            selectRect->BackupPixLimits();
            fMoveSelectRect = true;
            fDoNotDrawHandles = true;
            return false;
         }
    }
    return false;

}


bool PasteMode::mouseMoveEvent(QMouseEvent *event)
{
    QPointF canvasPos = EditMode::canvasInverseTransform.map(event->position());
    auto pt = canvasPos.toPoint();
    auto pix = Pos2Pixel(pt);

    if ((event->buttons() & Qt::LeftButton)){

        QRect r = image->rect();

        if (fMoveSelectRect){
            if (startPt!=pix){
                int dx = pix.x()-startPt.x();
                int dy = pix.y()-startPt.y();
                int tLeft  = selectRect->pixLeftBak + dx;
                int tRight = selectRect->pixRightBak + dx;
                int tTop   = selectRect->pixTopBak + dy;
                int tBottom   = selectRect->pixBottomBak + dy;
                if (r.contains(QPoint(tLeft,tTop)) && r.contains(QPoint(tRight,tBottom))){
                    selectRect->setPixLimits(tLeft,tTop,tRight,tBottom);
                    updateImage();
                    fDoNotDrawHandles = true;
                    return true;
                }
            }
        }else if (selCorner){
            if (r.contains(pix)){ // Keep actions inside image limits
                QPoint v = pix - startPt;
                if (v.x()||v.y()){
                    if (selectRect->moveHandle(selCorner,startPt+v)){
                        updateImage();
                        fDoNotDrawHandles = true;
                        return true;
                    }
                    return false;
                }
            }
        }

    }else{

        if (auto c = selectRect->hitCorner(pt)){
            mousePointer = c->cursorShape;
        }else if (selectRect->contains(pt)){
            mousePointer = Qt::SizeAllCursor;
        }else{
            mousePointer = Qt::ArrowCursor;
        }

    }

    return false;

}

bool PasteMode::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton)){
        fMoveSelectRect = false;
        selCorner = NULL;
    }
    fDoNotDrawHandles = false;
    mousePointer = Qt::ArrowCursor;
    return true;
}

void PasteMode::paintEvent(QPaintEvent *event, QPainter *painter)
{
    if (!selectRect->isPixNULL()){
        selectRect->draw(painter,cellSize,!fDoNotDrawHandles);
    }
}
