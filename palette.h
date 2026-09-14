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

    enum
    {
        RGB_R_MASK = 0xFF << 24,
        RGB_G_MASK = 0xFF << 16,
        RGB_B_MASK = 0xFF << 8,
        RGB_A_MASK = 0xFF
    };


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

    unsigned char getRGBAlpha(unsigned int rgb);
    unsigned char getRGBRed(unsigned int rgb);
    unsigned char getRGBGreen(unsigned int rgb);
    unsigned char getRGBBlue(unsigned int rgb);
    unsigned int  RGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a);
    void save(std::string pathName, std::string fileName);
    bool load(std::string pathName, std::string fileName);

    QColor myGridColor = Qt::black;

    int nbColumns = 16;
    int nbRows = 2;
    int cellSize = 16;

    ColorRect   foreGroundColor;
    ColorRect   backGroundColor;

    std::vector<ColorRect *> tblColors;

};
