
#include "pencilmode.h"


PencilMode::PencilMode():
    lastPixel(PixelRect())
{
    vertexHandleColor1 = QColor(255,0,0,255);
    vertexHandleColor2 = QColor(255,255,255,255);

}

PencilMode::~PencilMode()
{
    freePolygon();
}


void PencilMode::freePolygon()
{
    //--
    for (auto p:polygon){
        delete p;
    }
    polygon.clear();

}

void PencilMode::init()
{
    lastPixel = QPoint();
    freePolygon();

}

void PencilMode::toggleFlash()
{

    QColor tmpColor = vertexHandleColor1;
    vertexHandleColor1 = vertexHandleColor2;
    vertexHandleColor2 = tmpColor;

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

                if (r.contains(pix)){ // Keep actions inside image limits

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
            if (r.contains(pix)){ // Keep actions inside image limits
                backupImage();
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
        QRect r = image->rect();

        if (r.contains(pix)){ // Keep movement inside image limits
            if (fShiftKey){
                if (selVertex){
                    // Move selected vertex
                    selVertex->pix =  pix;
                    restoreImage();
                    drawPolygon();
                    return true;
                }
            }else{
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
        if (r.contains(pix)){ // Keep things inside image limits
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

        painter->setBrush(QBrush(foregroundColor));

        painter->setPen(QPen(vertexHandleColor1, 1.0, Qt::SolidLine, Qt::RoundCap,
                             Qt::RoundJoin));
        painter->drawRect(r);

        r.adjust(2,2,-2,-2);
        painter->setPen(QPen(vertexHandleColor2, 1.0, Qt::SolidLine, Qt::RoundCap,
                             Qt::RoundJoin));
        painter->drawRect(r);

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

}
