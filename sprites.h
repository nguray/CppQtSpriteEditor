#ifndef SPRITES_H
#define SPRITES_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QRect>
#include <QPoint>
#include <array>

class Sprites : public QWidget
{
    Q_OBJECT
public:

    enum {
        NB_SPRITES = 5
    };

    explicit Sprites(QWidget *parent = nullptr);
    ~Sprites();

    void newSprite(int w, int h);



public slots:
    void updateDisplay();

signals:
    void spriteChanged(QSharedPointer<QImage> spr);


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    int cellSize      = 64;
    int iSelectCell   = 0;
    int iSelectCellPopupMenu = 0;
    QRect cellsRect;

    std::array<QSharedPointer<QImage>,NB_SPRITES> sprites;

    void drawCells(QPainter *painter);
    void drawSelectMark(QPainter *painter);
    void drawSprites(QPainter *painter);

    void newImageTriggered();
    void loadImageTriggered();


};

#endif // SPRITES_H
