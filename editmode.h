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
    static QSharedPointer<QImage> image;
    static QImage image_bak;

    static int margin; 
    static int cellSize;

    static bool fShiftKey;

    QPoint Pos2Pixel(QPoint p);
    QRect  Pixel2Rect(int px,int py);
    void   backupImage();
    void   restoreImage();

    static void setImage(QSharedPointer<QImage> image);


    virtual bool mousePressEvent(QMouseEvent *event)=0;
    virtual bool mouseMoveEvent(QMouseEvent *event)=0;
    virtual bool mouseReleaseEvent(QMouseEvent *event)=0;
    virtual void paintEvent(QPaintEvent *event, QPainter *painter)=0;



};