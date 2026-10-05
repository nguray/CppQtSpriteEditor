#include "sprites.h"

Sprites::Sprites(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    setMinimumWidth(64);

}

Sprites::~Sprites()
{

}

void Sprites::mousePressEvent(QMouseEvent *event)
{
    QPoint p = event->position().toPoint();
    if (event->button() == Qt::LeftButton){
        //--
        if (cellsRect.contains(p)){
            iSelectCell = p.y()/cellSize;
            update();
        }
    }

}


void Sprites::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton)){
        //--
    }
}

void Sprites::mouseReleaseEvent(QMouseEvent *event)
{

    QWidget::mouseReleaseEvent(event);
}

void Sprites::resizeEvent(QResizeEvent *event)
{
    QSize s = event->size();
    cellSize = s.height()/nbCells;
    setMinimumWidth(cellSize);

    QWidget::resizeEvent(event);
}

void Sprites::drawCells(QPainter *painter)
{
    painter->setPen(QPen(QColor(128,128,128,255), 1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    int y;
    int xLeft = 0;
    int xRight = cellSize - 1;
    for (int i = 0; i<=nbCells; i++){
        y = i * cellSize;
        painter->drawLine(QPoint(xLeft,y),QPoint(xRight,y));
    }

    painter->drawLine(QPoint(xLeft,0),QPoint(xLeft,y));
    painter->drawLine(QPoint(xRight,0),QPoint(xRight,y));

    cellsRect.setCoords(xLeft,0,xRight,y);

}

void Sprites::drawSelectMark(QPainter *painter)
{
    int xLeft = 1;
    int xRight = cellSize - 2;
    int yTop = iSelectCell * cellSize + 1;
    int yBottom = yTop + cellSize - 2;

    painter->setPen(QPen(QColor(255,0,0,255), 2, Qt::SolidLine, Qt::RoundCap,
                         Qt::RoundJoin));

    //-- TopLeft Corner
    painter->drawLine(QPoint(xLeft,yTop+8),QPoint(xLeft,yTop));
    painter->drawLine(QPoint(xLeft,yTop),QPoint(xLeft+8,yTop));

    //-- TopRight Corner
    painter->drawLine(QPoint(xRight-8,yTop),QPoint(xRight,yTop));
    painter->drawLine(QPoint(xRight,yTop),QPoint(xRight,yTop+8));

    //-- Bottom Right Corner
    painter->drawLine(QPoint(xRight,yBottom-8),QPoint(xRight,yBottom));
    painter->drawLine(QPoint(xRight,yBottom),QPoint(xRight-8,yBottom));

    //-- BottomLeft Corner
    painter->drawLine(QPoint(xLeft+8,yBottom),QPoint(xLeft,yBottom));
    painter->drawLine(QPoint(xLeft,yBottom),QPoint(xLeft,yBottom-8));

}

void Sprites::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect r = event->rect();
    painter.fillRect(r,QColor(200,200,200));

    drawCells(&painter);

    drawSelectMark(&painter);

}