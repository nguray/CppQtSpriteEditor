
#include "pencilmode.h"


PencilMode::PencilMode():
    lastPixel(PixelRect())
{

}

PencilMode::~PencilMode()
{
    freePolygon();
}


void PencilMode::freePolygon()
{
    for (auto p:polygon){
        delete p;
    }
    polygon.clear();

}

void PencilMode::drawPolygon()
{
    bool fFirst = true;
    QPoint cur;
    QPoint prev;

    QPainter painter(EditMode::image.get());
    painter.setPen(QPen(EditMode::foregroundColor, 1.0, Qt::SolidLine, Qt::RoundCap,
                    Qt::RoundJoin));

    for(auto v : polygon){
        if (fFirst){
            fFirst = false;
            prev = v->pix;
        }else{
            cur = v->pix;
            painter.drawLine(prev, cur);
            prev = cur;
        }

    }

}

PixelRect *PencilMode::hitVertex(QPoint pt)
{
    for (auto v: polygon){
        if (v->contains(pt)){
            return v;
        }
    }
    return NULL;
}

bool PencilMode::mousePressEvent(QMouseEvent *event)
{
    
    if (event->button() == Qt::LeftButton) {

        auto pix = Pos2Pixel(event->position().toPoint());
        QRect r = image->rect();

        if (event->modifiers() & Qt::ShiftModifier) {

            if (selVertex=hitVertex(event->position().toPoint())){
                return false;
            }else{

                if (r.contains(pix)){

                    if (polygon.size()==0){
                        backupImage();
                        //-- Add starting vertex
                        polygon.push_back(new PixelRect(lastPixel));
                    }

                    selVertex = new PixelRect(pix);
                    polygon.push_back(selVertex);

                    // Draw polygon in curent image
                    drawPolygon();

                    return true;
                }
            }

        }else{
            freePolygon();
            if (r.contains(pix)){
                image->setPixelColor(pix,foregroundColor);
                return true;
            }
        }

    }else if (event->button() == Qt::RightButton) {
        // Validate polygon draw
        if (polygon.size()){
            auto v = polygon.back();
            lastPixel = PixelRect(v->pix);
            freePolygon();
        }
        return true;
    }

    return false;

}

bool PencilMode::mouseMoveEvent(QMouseEvent *event)
{
   if ((event->buttons() & Qt::LeftButton)){

        auto pix = Pos2Pixel(event->position().toPoint());

        if (fShiftKey){
            if (selVertex){
                // Move selected vertex
                selVertex->pix =  pix;
                restoreImage();
                drawPolygon();
                return true;
            }
        }else{
            QRect r = image->rect();
            if (r.contains(pix)){
                image->setPixelColor(pix,foregroundColor);
                return true;
            }
        }


    }

    return false;
}

bool PencilMode::mouseReleaseEvent(QMouseEvent *event)
{

    if ((event->button() == Qt::LeftButton)){
        auto pix = Pos2Pixel(event->position().toPoint());
        QRect r = image->rect();
        if (r.contains(pix)){
            if (fShiftKey){
                if (selVertex){
                    selVertex = NULL;
                }
            }else{
                lastPixel = PixelRect(pix);
            }
            return true;
        }
    }
    return false;
}


void PencilMode::drawPolygonVertices(QPainter *painter)
{
    int xLeft,yTop;
    for (auto v : polygon){

        auto r = Pixel2Rect(v->pix);
        v->setRect(r.left(),r.top(),cellSize,cellSize);

        painter->setPen(QPen(QColor(255,0,0,255), 1.0, Qt::SolidLine, Qt::RoundCap,
                             Qt::RoundJoin));
        painter->drawRect(r);

        QRect rect1(r.left()+2,r.top()+2,r.width()-4,r.height()-4);
        painter->setPen(QPen(QColor(255,255,255,255), 1.0, Qt::SolidLine, Qt::RoundCap,
                             Qt::RoundJoin));
        painter->drawRect(rect1);

    }

}

void PencilMode::paintEvent(QPaintEvent *event, QPainter *painter)
{
    //if (QGuiApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)) {
        // Shift key is currently pressed
    //}
    if (fShiftKey){
        drawPolygonVertices(painter);

    }

    // if ((lastPixel.pix_x!=0)||(lastPixel.pix_y!=0)){

    //     auto r = Pixel2Rect(lastPixel.pix_x,lastPixel.pix_y);
    //     qDebug() << "Left : " << r.left() << ", Top : " << r.top();
    //     lastPixel.setRect(r.left(),r.top(),r.width(),r.height());

    //     painter->setPen(QPen(QColor(255,0,0,255), 1.0, Qt::SolidLine, Qt::RoundCap,
    //                          Qt::RoundJoin));
    //     painter->drawRect(lastPixel);

    //     QRect rect = QRect(lastPixel.left()+2,lastPixel.top()+2,lastPixel.width()-4,lastPixel.height()-4);
    //     painter->setPen(QPen(QColor(255,255,255,255), 1.0, Qt::SolidLine, Qt::RoundCap,
    //                          Qt::RoundJoin));
    //     painter->drawRect(rect);

    // }

}
