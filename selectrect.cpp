#include "selectrect.h"

SelectRect::SelectRect()
{
    // TopLeft
    corners[0] = new CornerRect(&pixLeft,&pixTop,-1,-1,Qt::SizeFDiagCursor);
    // TopRight
    corners[1] = new CornerRect(&pixRight,&pixTop,1,-1,Qt::SizeBDiagCursor);
    // BottomRight
    corners[2] = new CornerRect(&pixRight,&pixBottom,1,1,Qt::SizeFDiagCursor);
    // BottomLeft
    corners[3] = new CornerRect(&pixLeft,&pixBottom,-1,1,Qt::SizeBDiagCursor);

    // MiddleTop
    corners[4] = new CornerRect(nullptr,&pixTop,0,0,Qt::SizeVerCursor);
    // MiddleRight
    corners[5] = new CornerRect(&pixRight,nullptr,0,0,Qt::SizeHorCursor);
    // MiddleBottom
    corners[6] = new CornerRect(nullptr,&pixBottom,0,0,Qt::SizeVerCursor);
    // MiddleBottom
    corners[7] = new CornerRect(&pixLeft,nullptr,0,0,Qt::SizeHorCursor);


}

SelectRect::~SelectRect()
{
    for (auto c : corners){
        if (c){
            delete c;
        }
    }
}

bool SelectRect::isPixNULL()
{
    return ((pixLeft==0) && (pixTop==0) && (pixLeft==pixRight) && (pixTop==pixBottom));
}

void SelectRect::setPixNULL()
{
    pixLeft = 0;
    pixRight = 0;
    pixTop = 0;
    pixBottom = 0;

}

void SelectRect::resetHandles()
{
    // Avoid unwilling handles selection
    for (auto c : corners){
        if (c){
            c->setRect(0,0,0,0);
        }
    }

}

QRect SelectRect::getPixRect()
{
    return QRect(pixLeft,pixTop,pixRight-pixLeft,pixBottom-pixTop);
}


void SelectRect::setPixLimits(int left,int top,int right,int bottom)
{
    pixLeft   = left;
    pixTop    = top;
    pixRight  = right;
    pixBottom = bottom;
}

void SelectRect::BackupPixLimits()
{
     pixLeftBak    = pixLeft;
     pixRightBak   = pixRight;
     pixTopBak     = pixTop;
     pixBottomBak = pixBottom;
}

CornerRect *SelectRect::hitCorner(QPoint pt)
{
    for (auto c : corners){
        if ((c!=nullptr) && (c->contains(pt))){
            return c;
        }
    }

    return NULL;
}

bool SelectRect::moveHandle(CornerRect *ptC, QPoint v)
{
    int savX=0,savY=0;
    if (ptC->x){
        savX = *(ptC->x);
        *(ptC->x) = v.x();
    }
    if (ptC->y){
        savY = *(ptC->y);
        *(ptC->y) = v.y();
    }
    if ((pixLeft<pixRight) &&(pixTop<pixBottom)){
        // Validate move
        return true;
    }else{
        // Restore start position
        if (ptC->x){
            *(ptC->x) = savX;
        }
        if (ptC->y){
            *(ptC->y) = savY;
        }
        return false;
    }

}

void SelectRect::draw(QPainter *painter,int cellSize, bool fDrawHandles)
{
    // 1. Activer le mode XOR pour inverser la couleur de destination
    //auto savComMod = painter->compositionMode();
    //painter->setCompositionMode(QPainter::RasterOp_SourceXorDestination);

    if ((pixLeft==pixRight) && (pixTop==pixBottom)) return;

    //-- Rect frame
    int xLeft = pixLeft*cellSize;
    int yTop  = pixTop*cellSize;
    int xRight = pixRight*cellSize + cellSize;
    int yBottom = pixBottom*cellSize + cellSize;

    setRect(xLeft,yTop,xRight-xLeft,yBottom-yTop);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(0,0,64,64), 1.5, Qt::DashLine, Qt::RoundCap,
                         Qt::RoundJoin));
    painter->drawRect(*this);

    //-- Set corner's handles fill color
    painter->setBrush(QBrush(QColor(0,0,0,255)));

    if (fDrawHandles){


        // Create and draw corners handles
        CornerRect *pC;
        auto drawCorner = [painter,cellSize](CornerRect *pC,int x,int y)
        {
            int s = cellSize/3;
            if (s < 3){
                s = 3;
            }
            pC->setCoords(x-s,y-s,x+s,y+s);
            pC->translate(pC->offsetX,pC->offsetY);
            painter->drawRect(*pC);
        };

        //-- Corner Handles
        //--TopLet
        if (pC = corners[0]){
            drawCorner(pC,xLeft,yTop);
        }
        //--TopRight
        if (pC = corners[1]){
            drawCorner(pC,xRight,yTop);
        }
        //--BottomRight
        if (pC = corners[2]){
            drawCorner(pC,xRight,yBottom);
        }
        //--BottomRight
        if (pC = corners[3]){
            drawCorner(pC,xLeft,yBottom);
        }

        //-- Middle Handles
        int xMiddle = (xLeft+xRight)/2;
        int yMiddle = (yTop+yBottom)/2;
        //-- MiddleTop
        if (pC = corners[4]){
            drawCorner(pC,xMiddle,yTop);
        }
        //-- MiddleRight
        if (pC = corners[5]){
            drawCorner(pC,xRight,yMiddle);
        }
        //-- MiddleBottom
        if (pC = corners[6]){
            drawCorner(pC,xMiddle,yBottom);
        }
        //-- MiddleLeft
        if (pC = corners[7]){
            drawCorner(pC,xLeft,yMiddle);
        }


    }
    //painter->setCompositionMode(savComMod);

}
