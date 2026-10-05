#ifndef SPRITES_H
#define SPRITES_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QRect>
#include <QPoint>

class Sprites : public QWidget
{
    Q_OBJECT
public:
    explicit Sprites(QWidget *parent = nullptr);
    ~Sprites();

signals:


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    const int nbCells = 5;
    int cellSize      = 64;
    int iSelectCell   = 0;
    QRect cellsRect;

    void drawCells(QPainter *painter);
    void drawSelectMark(QPainter *painter);


};

#endif // SPRITES_H
