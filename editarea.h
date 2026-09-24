#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QImage>
#include <QPainter>
#include <QTimer>

#include "editmode.h"
#include "pencilmode.h"
#include "rectanglemode.h"
#include "ellipsemode.h"


class EditArea : public QWidget
{

    Q_OBJECT

public:

    EditArea(QWidget *parent = nullptr);
    ~EditArea();

    void setPencilMode();
    void setRectangleMode();
    void setEllipseMode();


public slots:
    void setForegroundColor(QColor newColor);
    void setBackgroundColor(QColor newColor);
    void toggleFlash();


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;    
    void keyReleaseEvent(QKeyEvent *event) override;    

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void drawGrid(QPainter *painter);
    void drawPixels(QPainter *painter);


    QTimer *m_timer;

    bool modified = false;
    int myPenWidth = 1;


    QColor myGridColor = Qt::black;
    QPoint lastPoint;

    EditMode *curEditMode=NULL;
    PencilMode      *pencilMode;
    RectangleMode   *rectangleMode;
    EllipseMode     *ellipseMode;

};