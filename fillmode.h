#ifndef FILLMODE_H
#define FILLMODE_H

#include "editmode.h"
#include <QPixmap>
#include <QCursor>

class FillMode: public EditMode
{
public:
    FillMode();
    ~FillMode();

    void FloodFill(int fillX, int fillY, QColor fillColor);

    void init();
    void updateImage();
    bool mousePressEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event, QPainter *painter);

    QPixmap floodFillPixmap;
    QCursor floodFillCursor;

};

#endif // FILLMODE_H
