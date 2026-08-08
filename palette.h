#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <vector>
#include "colorrect.h"

class Palette : public QWidget
{

   Q_OBJECT

public:

    Palette(QWidget *parent = nullptr);
    ~Palette();


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;    

private:

    ColorRect *hitColors(QPoint p);
    void drawColors(QPainter *painter);

    QColor myGridColor = Qt::black;

    int nbColumns = 16;
    int nbRows = 2;
    int cellSize = 16;

    ColorRect   foreGroundColor;
    ColorRect   backGroundColor;

    std::vector<ColorRect *> tblColors;

};
