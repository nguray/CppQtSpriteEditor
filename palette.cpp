#include "palette.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "qcolorpicker.h"
#include <QApplication>

Palette::Palette(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setMinimumHeight(34);   

    //-- Fill colors table
    for (int i = 0; i < nbRows; i++) {
        for (int j = 0; j < nbColumns; j++) {
            int x = 2 + 2*cellSize + j*cellSize;
            int y = 1 + i*cellSize;
            tblColors.push_back(new ColorRect(x+1,y+1,cellSize-1,QColor(0,0,0,0)));
        }
    }

    setDefaultPalette();

    // Set up the single-shot timer
    clickTimer = new QTimer(this);
    clickTimer->setSingleShot(true);

    // Connect the timer timeout to your single click action
    connect(clickTimer, &QTimer::timeout, this, &Palette::handleSingleLeftClick);


}

Palette::~Palette()
{
    for(auto r : tblColors){
        delete r;
    }
    tblColors.clear();

    if (clickTimer){
        delete clickTimer;
    }

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

    fHitColor = false;
    if (event->button() == Qt::LeftButton){
        startPt = p;
        clickTimer->start(QApplication::doubleClickInterval());

        if (auto cr = hitColors(p) ){
            fHitColor = true;
            hitColor = cr->getColor();
        }else if (foreGroundColor.contains(p)){
            fHitColor = true;
            hitColor = foreGroundColor.getColor();
        }

    }else if (event->button() == Qt::RightButton){
        if (auto cr = hitColors(p) ){
            backGroundColor.setColor(cr->getColor());
            update();
        }

    }

}

void Palette::handleSingleLeftClick() {
    qDebug() << "Single Left click Detected!";
    if (auto cr = hitColors(startPt) ){
        // Select foreground color
        foreGroundColor.setColor(cr->getColor());
        emit foreGroundColorChanged(cr->getColor());
        update();
    }else if (startPt.x()<2*cellSize){
        //-- Swap foreground <-> background
        auto c = backGroundColor.getColor();
        backGroundColor.setColor(foreGroundColor.getColor());
        foreGroundColor.setColor(c);
        emit foreGroundColorChanged(c);
        update();
    }
}


void Palette::mouseDoubleClickEvent(QMouseEvent *event)
{

    // Disable single clich handler
    clickTimer->stop();

    if (event->button() == Qt::LeftButton) {

        QPoint p = event->position().toPoint();

        // Handle left double-click
        qDebug() << "Left mouse double-clicked at:" << event->pos();

        if (auto cr = hitColors(p) ){
            if (cr!=tblColors.front()){
                QColorPicker picker(this);
                picker.setColor(cr->getColor());

                if (picker.exec() == QDialog::Accepted) {
                    QColor color = picker.color();
                    cr->setColor(color);
                    qDebug() << "Linux user picked color:" << color.name();
                    update();
                }
            }
        }else if (foreGroundColor.contains(p)){
            QColorPicker picker(this);
            picker.setColor(foreGroundColor.getColor());

            if (picker.exec() == QDialog::Accepted) {
                QColor color = picker.color();
                foreGroundColor.setColor(color);
                qDebug() << "Linux user picked color:" << color.name();
                emit foreGroundColorChanged(foreGroundColor.getColor());
                update();

            }
        }
    }

}


void Palette::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton)){
        QPoint p = event->position().toPoint();
        if (fHitColor){
            QPoint v = p - startPt;
            if (v!=QPoint(0,0)){
                if (!dragColor){
                    dragColor = new ColorRect(startPt.x()-cellSize/2,startPt.y()-cellSize,cellSize,hitColor);
                }
                dragColor->moveCenter(startPt+v);
                update();
            }
        }
    }
}

void Palette::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "Mouse Release button";
    qDebug() << "--------------";
    QPoint p = event->position().toPoint();
    if (event->button() == Qt::LeftButton){
        if (dragColor) {
            if (auto cr = hitColors(p) ){
                if (cr!= tblColors.front()){
                    cr->setColor(dragColor->getColor());
                }
            }
            delete dragColor;
            dragColor = nullptr;
            update();
        }
    }

}

void Palette::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void Palette::onSetForeGroundColor(QColor c)
{
    foreGroundColor.setColor(c);
    update();
}

void Palette::paintEvent(QPaintEvent *event)
{ 
    QPainter painter(this);

    QRect r = event->rect();
    painter.fillRect(r,QColor(246,246,240));

    painter.setPen(QPen(myGridColor, 0.5, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));

    backGroundColor.draw(&painter);
    foreGroundColor.draw(&painter);
    drawColors(&painter);

    //--
    if (dragColor){
        dragColor->draw(&painter);
        // painter.setBrush(Qt::NoBrush);
        // painter.setPen(QPen(Qt::red, 0, Qt::DashLine, Qt::RoundCap,
        //                      Qt::RoundJoin));
        // painter.drawRect(*dragColor);

    }

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

unsigned char Palette::getRGBAlpha(unsigned int rgb) {
  //-----------------------------------------------------------
  return (rgb & RGB_A_MASK);
}

unsigned char Palette::getRGBRed(unsigned int rgb) {
  //-----------------------------------------------------------
  return (rgb & RGB_R_MASK) >> 24;
}

unsigned char Palette::getRGBGreen(unsigned int rgb) {
  //-----------------------------------------------------------
  return (rgb & RGB_G_MASK) >> 16;
}

unsigned char Palette::getRGBBlue(unsigned int rgb) {
  //-----------------------------------------------------------
  return (rgb & RGB_B_MASK) >> 8;
}

unsigned int Palette::RGBA(unsigned char r, unsigned char g, unsigned char b,
                             unsigned char a) {
  //-----------------------------------------------------------
  return (b << 8) | (g << 16) | (r << 24) | (a);
}

void Palette::save(std::string pathName, std::string fileName) {
  std::string fullPathName;
  //-------------------------------------------------
  if (pathName != "") {
    fullPathName = pathName + "/" + fileName;
  } else {
    fullPathName = fileName;
  }
  std::ofstream f(fullPathName);
  if (f.is_open()) {
    //--
    QColor qc;
    unsigned int ic;
    qc = foreGroundColor.getColor();
    ic = RGBA(qc.red(), qc.green(), qc.blue(), qc.alpha());
    f << "FOREGROUND " << ic << std::endl;
    //--
    qc = backGroundColor.getColor();
    ic = RGBA(qc.red(), qc.green(), qc.blue(), qc.alpha());
    f << "BACKGROUND " << ic << std::endl;
    //--
    for (int i = 0; i < nbRows * nbColumns; i++) {
      qc = tblColors[i]->getColor();
      ic = RGBA(qc.red(), qc.green(), qc.blue(), qc.alpha());
      f << ic << std::endl;
    }
    f.close();
  }
}

bool Palette::load(std::string pathName, std::string fileName) {
  unsigned int ic;
  std::string strline;
  std::string strWord;
  std::string fullPathName;
  //-------------------------------------------------
  if (pathName != "") {
    fullPathName = pathName + "/" + fileName;
  } else {
    fullPathName = fileName;
  }

  std::ifstream f(fullPathName);
  if (f.is_open()) {
    int i = 0;
    //--
    while (!f.eof()) {
      std::getline(f, strline);
      std::stringstream ss(strline);
      ss >> strWord;
      if (strWord == "FOREGROUND") {
        ss >> strWord;
        ic = std::atoi(strWord.c_str());
        foreGroundColor.setColor(QColor(getRGBRed(ic), getRGBGreen(ic),
                                       getRGBBlue(ic), getRGBAlpha(ic)));
        emit foreGroundColorChanged(foreGroundColor.getColor());
      } else if (strWord == "BACKGROUND") {
        ss >> strWord;
        ic = std::atoi(strWord.c_str());
        backGroundColor.setColor(QColor(getRGBRed(ic), getRGBGreen(ic),
                                       getRGBBlue(ic), getRGBAlpha(ic)));
      } else {
        ic = std::atoi(strWord.c_str());
        if (i < nbRows * nbColumns) {
          tblColors[i]->setColor(QColor(getRGBRed(ic), getRGBGreen(ic),
                                      getRGBBlue(ic), getRGBAlpha(ic)));
          i++;
        } else {
          break;
        }
      }
    }
    f.close();
    return true;
  }else{

      setDefaultPalette();

  }
  return false;
}


void Palette::setDefaultPalette()
{
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
    tblColors[i++]->setColor(QColor(0xFF, 0x00, 0xFF, 0xFF));

    foreGroundColor = ColorRect(1,1,1.2*cellSize,QColor(0x80, 0x80, 0xF0, 0xFF));
    backGroundColor = ColorRect(1,1,2*cellSize,QColor(0x00, 0x00, 0x00, 0x00));

    emit foreGroundColorChanged(foreGroundColor.getColor());

}
