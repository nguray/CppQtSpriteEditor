#include "selectrect.h"

SelectRect::SelectRect()
{
    corners[0] = new CornerRect(&subImageLeft,&subImageTop);
    corners[1] = new CornerRect(&subImageRight,&subImageTop);
    corners[2] = new CornerRect(&subImageRight,&subImageBottom);
    corners[3] = new CornerRect(&subImageLeft,&subImageBottom);

}

SelectRect::~SelectRect()
{
    for (auto i=0;i<4;i++){
        if (auto c = corners[i]){
            delete c;
        }
    }
}

bool SelectRect::isSubImageNULL()
{
    return ((subImageLeft==0) && (subImageTop==0) && (subImageLeft==subImageRight) && (subImageTop==subImageBottom));
}

void SelectRect::setSubImageNULL()
{
    subImageLeft = 0;
    subImageRight = 0;
    subImageTop = 0;
    subImageBottom = 0;
}


void SelectRect::setSubImageRect(int left,int top,int right,int bottom)
{
    this->subImageLeft   = left;
    this->subImageTop    = top;
    this->subImageRight  = right;
    this->subImageBottom = bottom;
}

void SelectRect::BackupSubImageRect()
{
    subImageLeftBak   = subImageLeft;
    subImageRightBak  = subImageRight;
    subImageTopBak    = subImageTop;
    subImageBottomBak = subImageBottom;
}

void SelectRect::RestoreSubImageRect()
{
    subImageLeft   = subImageLeftBak;
    subImageRight  = subImageRightBak;
    subImageTop    = subImageTopBak;
    subImageBottom = subImageBottomBak;
}

