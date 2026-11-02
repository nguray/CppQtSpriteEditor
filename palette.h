#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QTimer>
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

    void save(std::string pathName, std::string fileName);
    bool load(std::string pathName, std::string fileName);

signals:
    void foreGroundColorChanged(QColor c);
    void backGroundColorChanged(QColor c);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void onSetForeGroundColor(QColor c);

private slots:
    void handleSingleLeftClick();

private:

    ColorRect *hitColors(QPoint p);
    void drawColors(QPainter *painter);

    unsigned char getRGBAlpha(unsigned int rgb);
    unsigned char getRGBRed(unsigned int rgb);
    unsigned char getRGBGreen(unsigned int rgb);
    unsigned char getRGBBlue(unsigned int rgb);
    unsigned int  RGBA(unsigned char r,unsigned char g,unsigned char b,unsigned char a);
    void          setDefaultPalette();

    QColor myGridColor = Qt::black;
    bool   fHitColor;
    QColor hitColor;

    int nbColumns = 16;
    int nbRows = 2;
    int cellSize = 16;
    QPoint startPt;

    ColorRect   foreGroundColor;
    ColorRect   backGroundColor;

    ColorRect   *dragColor = nullptr;

    std::vector<ColorRect *> tblColors;

    QTimer  *clickTimer=nullptr;
};
