#include "palette.h"

Palette::Palette(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setMinimumHeight(30);   

    //-- Fill colors table
    for (int i = 0; i < nbRows; i++) {
        for (int j = 0; j < nbColumns; j++) {
            tblColors.push_back(new ColorRect());
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

void Palette::mousePressEvent(QMouseEvent *event)
{

}

void Palette::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton)){


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

    painter.drawLine(QPoint(0,0), QPoint(width(),height()));    
    painter.drawLine(QPoint(0,height()), QPoint(width(),0));    


}