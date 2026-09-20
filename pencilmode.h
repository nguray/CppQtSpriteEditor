
#pragma once
#include "editmode.h"
#include <vector>
#include "QPoint"

class PencilMode : public EditMode
{
public:
    PencilMode();
    ~PencilMode();

    bool mousePressEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    std::vector<QPoint *> listPts;

};