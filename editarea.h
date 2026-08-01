#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QImage>
#include <QPainter>

class EditArea : public QWidget
{

    Q_OBJECT

public:

    EditArea(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;    

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void drawGrid(QPainter *painter);
    QPoint Pos2Pixel(QPoint p);
 

    bool modified = false;
    bool scribbling = false;
    int myPenWidth = 1;

    int cellSize = 10;

    QColor myPenColor = Qt::blue;
    QColor myGridColor = Qt::black;
    QImage image;
    QPoint lastPoint;


};