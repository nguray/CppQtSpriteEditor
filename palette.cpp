#include "palette.h"

Palette::Palette(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setMinimumHeight(34);   


    foreGroundColor = ColorRect(1,1,1.2*cellSize,QColor(0x80, 0x80, 0x80, 0xFF));
    backGroundColor = ColorRect(1,1,2*cellSize,QColor(0x00, 0x00, 0x00, 0x00));

    //-- Fill colors table
    for (int i = 0; i < nbRows; i++) {
        for (int j = 0; j < nbColumns; j++) {
            int x = 2 + 2*cellSize + j*cellSize;
            int y = 1 + i*cellSize;
            tblColors.push_back(new ColorRect(x+1,y+1,cellSize-1,QColor(0,0,0,0)));
        }
    }

    int i = 0;
    tblColors[i++]->setColor(QColor(0x00, 0x00, 0x00, 0x00));
    // tblColors[i++].color = QColor(0x00,0x00,0x00,0xFF);
    tblColors[i++]->setColor(QColor(0xFF, 0x00, 0x00, 0xFF));
    tblColors[i++]->setColor(QColor(0xFF, 0xFF, 0xFF, 0xFF));
    tblColors[i++]->setColor(QColor(0x80, 0x80, 0x80, 0xFF));
    tblColors[i++]->setColor(QColor(0xC0, 0xC0, 0xC0, 0xFF));
    tblColors[i++]->setColor(QColor(0x80, 0x00, 0x00, 0xFF));
    tblColors[i++]->setColor(QColor(0xFF, 0x00, 0x00, 0xFF));
    tblColors[i++]->setColor(QColor(0x80, 0x80, 0x00, 0xFF));
    tblColors[i++]->setColor(QColor(0xFF, 0xFF, 0x00, 0xFF));
    tblColors[i++]->setColor(QColor(0x00, 0x80, 0x00, 0xFF));
    tblColors[i++]->setColor(QColor(0x00, 0xFF, 0x00, 0xFF));
    tblColors[i++]->setColor(QColor(0x00, 0x80, 0x80, 0xFF));
    tblColors[i++]->setColor(QColor(0x00, 0xFF, 0xFF, 0xFF));
    tblColors[i++]->setColor(QColor(0x00, 0x00, 0x80, 0xFF));
    tblColors[i++]->setColor(QColor(0x00, 0x00, 0xFF, 0xFF));
    tblColors[i++]->setColor(QColor(0x80, 0x00, 0x80, 0xFF));
//    tblColors[i++]->setColor(QColor(0xFF, 0x00, 0xFF, 0xFF));


}

Palette::~Palette()
{
    for(auto r : tblColors){
        delete r;
    }
    tblColors.clear();

}

ColorRect *Palette::hitColors(QPoint p)
{
    for(auto r : tblColors){
        if (r->contains(p)){
            return r;
        }
    }
    return NULL;
}

void Palette::mousePressEvent(QMouseEvent *event)
{
    QPoint p = event->position().toPoint();
    if (event->button() == Qt::LeftButton){
        if (auto cr = hitColors(p) ){
            foreGroundColor.setColor(cr->getColor());
            update();
        }
    }else if (event->button() == Qt::RightButton){
        if (auto cr = hitColors(p) ){
            backGroundColor.setColor(cr->getColor());
            update();
        }

    }

}



void Palette::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton)){
    //
    }
}

void Palette::mouseReleaseEvent(QMouseEvent *event)
{
    // if (event->button() == Qt::LeftButton && scribbling) {
    //     drawLineTo(event->position().toPoint());
    //     scribbling = false;
    // }
}

void Palette::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void Palette::paintEvent(QPaintEvent *event)
{ 
    QPainter painter(this);

    QRect r = event->rect();
    painter.fillRect(r,QColor(200,220,220));

    painter.setPen(QPen(myGridColor, 0.5, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));

    backGroundColor.draw(&painter);
    foreGroundColor.draw(&painter);
    drawColors(&painter);

}


void Palette::drawColors(QPainter *painter)
{
    int ic = 0;
    for (int i = 0; i < nbRows; i++) {
        for (int j = 0; j < nbColumns; j++) {
            if (ic<tblColors.size()){
                auto cr = tblColors[ic];
                cr->draw(painter);
            }
            ic++;
        }
    }

}