
#include "pencilmode.h"


PencilMode::PencilMode()
{
    lastPixel = PixelRect();
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
    int x,y;
    int prev_x,prev_y;

    QPainter painter(EditMode::image.get());
    painter.setPen(QPen(EditMode::foregroundColor, 1.0, Qt::SolidLine, Qt::RoundCap,
                    Qt::RoundJoin));

    for(auto v : polygon){
        if (fFirst){
            fFirst = false;
            prev_x = v->pix_x;
            prev_y = v->pix_y;
        }else{
            x = v->pix_x;
            y = v->pix_y;
            //qDebug() << "(" << prev_x << "," << prev_y << ") -> (" << x << "," << y << ") ";
            painter.drawLine(QPoint(prev_x,prev_y), QPoint(x,y));
            prev_x = x;
            prev_y = y;
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
        QSize s = image->size();

        if (event->modifiers() & Qt::ShiftModifier) {

            if (selVertex=hitVertex(event->position().toPoint())){
                qDebug() << ">>> Hit Vertex " << polygon.size();
                return false;
            }else{

                if ((pix.x()>=0)&&(pix.x()<s.width())&&
                    (pix.y()>=0)&&(pix.y()<s.height())){

                    if (polygon.size()==0){
                        backupImage();
                        //-- Add lastPixel
                        polygon.push_back(new PixelRect(lastPixel));
                    }

                    polygon.push_back(new PixelRect(pix.x(),pix.y()));

                    //qDebug() << ">>> Nbre Vertice " << polygon.size();

                    // Draw polygon in curent image
                    drawPolygon();

                    //qDebug() << pix.x() << "," << pix.y();
                    return true;
                }
            }

        }else{
            freePolygon();
            if ((pix.x()>=0)&&(pix.x()<s.width())&&
                (pix.y()>=0)&&(pix.y()<s.height())){
                image->setPixelColor(pix.x(),pix.y(),foregroundColor);
                return true;
            }
        }

    }else if (event->button() == Qt::RightButton) {
        if (polygon.size()){
            auto v = polygon.back();
            lastPixel = PixelRect(v->pix_x,v->pix_y);
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
                selVertex->pix_x = pix.x();
                selVertex->pix_y = pix.y();
                restoreImage();
                drawPolygon();
                return true;
            }else{
                if (polygon.size()){
                    restoreImage();
                    auto v = polygon.back();
                    v->pix_x = pix.x();
                    v->pix_y = pix.y();
                    drawPolygon();
                    return true;
                }
            }
        }else{
            QSize s = image->size();
            if ((pix.x()>=0)&&(pix.x()<s.width())&&
                 (pix.y()>=0)&&(pix.y()<s.height())){
                image->setPixelColor(pix.x(),pix.y(),foregroundColor);
                return true;
            }
        }


    }

    return false;
}

bool PencilMode::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << ">>> Hit Vertex " << polygon.size();
    if ((event->button() == Qt::LeftButton)){
        auto pix = Pos2Pixel(event->position().toPoint());
        QSize s = image->size();
        if ((pix.x()>=0)&&(pix.x()<s.width())&&
            (pix.y()>=0)&&(pix.y()<s.height())){
            if (fShiftKey){
                if (selVertex){
                    selVertex = NULL;
                }
            }else{
                lastPixel = PixelRect(pix.x(),pix.y());
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

        auto r = Pixel2Rect(v->pix_x,v->pix_y);
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
