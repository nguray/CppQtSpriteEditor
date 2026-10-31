#ifndef PASTEMODE_H
#define PASTEMODE_H

#include "editmode.h"

class PasteMode : public EditMode
{
public:
    PasteMode();
    ~PasteMode();

    void pasteImage();

    void init();
    void updateImage();
    bool mousePressEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event, QPainter *painter);

private:
    CornerRect *selCorner=NULL;
    bool fMoveSelectRect = false;
    bool fDoNotDrawHandles = false;
    QPoint startPt;

};

#endif // PASTEMODE_H
