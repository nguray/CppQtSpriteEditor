#include "selectrect.h"

SelectRect::SelectRect()
{
    corners[0] = new CornerRect(&pixLeft,&pixTop);
    corners[1] = new CornerRect(&pixRight,&pixTop);
    corners[2] = new CornerRect(&pixRight,&pixBottom);
    corners[3] = new CornerRect(&pixLeft,&pixBottom);

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

