#pragma once
#include <QColor>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>

class EditMode {

public:
    EditMode();
    ~EditMode();

    static QColor foregroundColor;
    static QColor backgroundColor;
    static QImage image;

    static int margin; 
    static int cellSize;

    QPoint Pos2Pixel(QPoint p);


    virtual bool mousePressEvent(QMouseEvent *event)=0;
    virtual bool mouseMoveEvent(QMouseEvent *event)=0;
    virtual bool mouseReleaseEvent(QMouseEvent *event)=0;
    virtual void paintEvent(QPaintEvent *event)=0;



};