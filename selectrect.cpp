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

}

SelectRect::~SelectRect()
{
    for (auto i=0;i<4;i++){
        if (auto c = corners[i]){
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
    for (auto i=0;i<4;i++){
        if (auto c = corners[i]){
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
    CornerRect *c;
    for (auto i=0;i<4;i++){
        if ((c=corners[i]) && (c->contains(pt))){
            return c;
        }
    }

    return NULL;
}

void SelectRect::draw(QPainter *painter,int margin,int cellSize, bool fDrawHandles)
{
    //-- Rect frame
    int xLeft = pixLeft*cellSize + margin;
    int yTop  = pixTop*cellSize + margin;
    int xRight = pixRight*cellSize + margin + cellSize;
    int yBottom = pixBottom*cellSize + margin + cellSize;

    setRect(xLeft,yTop,xRight-xLeft,yBottom-yTop);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(0,0,64,64), 1.0, Qt::SolidLine, Qt::RoundCap,
                         Qt::RoundJoin));
    painter->drawRect(*this);

    //-- Set corner's handles fill color
    painter->setBrush(QBrush(QColor(0,0,128,255)));

    if (fDrawHandles){
        // Create and draw corners handles
        CornerRect *pC;
        auto drawCorner = [painter](CornerRect *pC,int x,int y)
        {
            pC->setCoords(x-5,y-5,x+5,y+5);
            pC->translate(pC->offsetX,pC->offsetY);
            painter->drawRect(*pC);
        };

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
    }

}
