#include "sprites.h"
#include <QMenu>
#include "newspritedlg.h"
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QFileInfo>

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
            int iNewSelectCell = p.y()/cellSize;
            if (iNewSelectCell!=iSelectCell){
                auto img = images[iNewSelectCell];
                if (img==nullptr){

                    QMenu menu(this);
                    QAction *newAct = menu.addAction("New Image");
                    QAction *loadAct = menu.addAction("Load Image");

                    iSelectCellPopupMenu = iNewSelectCell;

                    // Connect actions to functions
                    connect(newAct, &QAction::triggered, this, &Sprites::newImageTriggered);
                    connect(loadAct, &QAction::triggered, this, &Sprites::loadImageTriggered);


                    // Display the menu at the exact click location
                    menu.exec(event->globalPosition().toPoint());


                }else{
                    iSelectCell = iNewSelectCell;
                    auto spr = images[iSelectCell];
                    emit spriteChanged(spr);
                    QString filename = QFileInfo(spr->fileName).fileName();
                    emit(fileNameChanged(filename));
                    update();
                }
            }
        }
    }

}

void Sprites::newSprite(int w, int h)
{
    auto spr = QSharedPointer<SpriteImage>::create(w, h, QImage::Format_ARGB32);
    spr->fill(QColor(0, 0, 0, 0));
    images[iSelectCell] = spr;
    emit spriteChanged(spr);
    emit(fileNameChanged(""));
    update();
}

void Sprites::newImageTriggered()
{
    NewSpriteDlg dlg(this);

    if (dlg.exec() == QDialog::Accepted) {
        // User clicked OK - extract data here if needed
        iSelectCell = iSelectCellPopupMenu;
        qDebug() << "New Sprite OK";
        newSprite(dlg.getSpriteWidth(), dlg.getSpriteHeight());
    } else {
        // User clicked Cancel or closed the window
        qDebug() << "New Sprite Cancel";
    }

}

void Sprites::newCurrentSprite()
{
    NewSpriteDlg dlg(this);

    if (dlg.exec() == QDialog::Accepted) {
        // User clicked OK - extract data here if needed
        qDebug() << "New Sprite OK";
        newSprite(dlg.getSpriteWidth(), dlg.getSpriteHeight());
    } else {
        // User clicked Cancel or closed the window
        qDebug() << "New Sprite Cancel";
    }

}

void Sprites::loadCurrentSprite(QString fullPathName)
{
    auto spr = QSharedPointer<SpriteImage>::create();
    spr->load( fullPathName);
    images[iSelectCell] = spr;
    spr->fileName = fullPathName;
    emit spriteChanged(spr);
    emit(enableSave(false));
    QString filename = QFileInfo(fullPathName).fileName();
    emit(fileNameChanged(filename));
    qDebug() << "FileName : " << filename;
    update();
}

void Sprites::loadImageTriggered()
{
    // Open the file dialog
    QString fileName = QFileDialog::getOpenFileName(
        this,                           // Parent widget
        tr("Open Document"),            // Dialog window title
        ".",         // Starting directory
        tr("png (*.png);;All Files (*.*)") // File filters
        );

    // Check if the user selected a file or cancelled
    if (!fileName.isEmpty()) {
        qDebug() << "Selected file path:" << fileName;
        // Proceed with reading the file...
        iSelectCell = iSelectCellPopupMenu;
        loadCurrentSprite(fileName);

    } else {
        qDebug() << "File selection cancelled.";
    }

}

void Sprites::SaveCurrentSprite()
{
    if (images[iSelectCell]->fModified){
        auto currentFileName = images[iSelectCell]->fileName;
        if (currentFileName.isEmpty()){
            SaveAsCurrentSprite();
        }else{
            auto spr = images[iSelectCell];
            if (!spr.isNull()){
                spr->save(currentFileName,"PNG");
            }
            spr->fModified = false;
            emit(enableSave(false));
            //QString filename = QFileInfo(currentFileName).fileName();
            //emit(fileNameChanged(filename));
        }
    }
}

void Sprites::SaveAsCurrentSprite()
{
    auto spr = images[iSelectCell];
    QString fileName = spr->fileName;
    if (fileName.isEmpty()){
        fileName = "./untitled.png";
    }
    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Save File"),
        fileName,
        tr("png (*.png);;All Files (*)")
        );

    if (!filePath.isEmpty()) {
        if (!filePath.endsWith(".PNG", Qt::CaseInsensitive)) {
            filePath += ".png";
            qDebug() << "Selected file path:" << filePath;
        }
        if (!spr.isNull()){
            spr->save(filePath,"PNG");
            spr->fileName = filePath;
            spr->fModified = false;
            emit(enableSave(false));
            QString filename = QFileInfo(spr->fileName).fileName();
            emit(fileNameChanged(filename));
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
    cellSize = s.height()/NB_SPRITES;
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
    for (int i = 0; i<=NB_SPRITES; i++){
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

void Sprites::drawSprites(QPainter *painter)
{
    int imgWidth,imgHeight;
    int yTop = 0;
    for (auto &img : images){
        if (!img.isNull()){
            imgWidth  = img->width();
            imgHeight = img->height();
            painter->drawImage(QRect(cellSize/2 - imgWidth/2,yTop + cellSize/2 - imgHeight/2,imgWidth,imgHeight),
                            *img, img->rect());
        }
        yTop += cellSize;
    }
}

void Sprites::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect r = event->rect();
    painter.fillRect(r,QColor(246,246,240));

    drawCells(&painter);

    drawSelectMark(&painter);

    drawSprites(&painter);

}

void Sprites::on_imageChanged()
{
    images[iSelectCell]->fModified = true;
    update();
}
