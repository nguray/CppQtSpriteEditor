
#pragma once
#include "editmode.h"

class PencilMode : public EditMode
{
public:
    PencilMode();
    ~PencilMode();

    bool mousePressEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

};