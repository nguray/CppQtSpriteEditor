#pragma once
#include <QPoint>
#include <QColor>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include "spriteimage.h"
#include "selectrect.h"

class EditMode {

public:
    EditMode();
    ~EditMode();

    static QColor foregroundColor;
    static QColor backgroundColor;
    static QSharedPointer<SpriteImage> image;
    static QImage image_cpy;
    static QImage image_bak;
    static Qt::CursorShape mousePointer;

    static  SelectRect  *selectRect;

    static int margin; 
    static int cellSize;

    static bool fShiftKey;
    static bool fControlKey;

    static QPoint Pos2Pixel(QPoint p);

    QRect  Pixel2Rect(int px,int py);
    QRect  Pixel2Rect(QPoint p);

    static void   backupImage();
    static void   restoreImage();

    static void setImage(QSharedPointer<SpriteImage> image);
    static std::tuple<bool,QColor> getPixelColor(QPoint p);

    virtual void init()=0;
    virtual void updateImage()=0;
    virtual bool mousePressEvent(QMouseEvent *event)=0;
    virtual bool mouseMoveEvent(QMouseEvent *event)=0;
    virtual bool mouseReleaseEvent(QMouseEvent *event)=0;
    virtual void paintEvent(QPaintEvent *event, QPainter *painter)=0;



};