#ifndef SELECTRECT_H
#define SELECTRECT_H

#include <QRect>
#include "cornerrect.h"

class SelectRect : public QRect
{
public:
    SelectRect();
    ~SelectRect();

    bool fDefined = false;

    CornerRect *corners[4];

    bool isSubImageNULL();
    void setSubImageNULL();
    void setSubImageRect(int left,int top,int right,int bottom);

    void BackupSubImageRect();
    void RestoreSubImageRect();



    int subImageLeft   = 0;
    int subImageTop    = 0;
    int subImageRight  = 0;
    int subImageBottom = 0;

    int subImageLeftBak   = 0;
    int subImageTopBak    = 0;
    int subImageRightBak  = 0;
    int subImageBottomBak = 0;


};

#endif // SELECTRECT_H
