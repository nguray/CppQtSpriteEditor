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

    bool isPixNULL();
    void setPixNULL();
    void setPixLimits(int left,int top,int right,int bottom);
    QRect getPixRect();


    void BackupPixLimits();

    int pixLeft = 0;
    int pixTop = 0;
    int pixRight = 0;
    int pixBottom = 0;

    int pixLeftBak = 0;
    int pixTopBak = 0;
    int pixRightBak = 0;
    int pixBottomBak = 0;


};

#endif // SELECTRECT_H
