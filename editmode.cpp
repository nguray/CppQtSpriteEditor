
#include "editmode.h"


QColor EditMode::foregroundColor = Qt::black;
QColor EditMode::backgroundColor = QColor(0,0,0,0);
QSharedPointer<SpriteImage> EditMode::image = QSharedPointer<SpriteImage>::create(32, 32, QImage::Format_ARGB32);
QImage EditMode::image_bak = QImage(32, 32, QImage::Format_ARGB32);
QImage EditMode::image_cpy = QImage();

bool EditMode::fShiftKey = false;
bool EditMode::fControlKey = false;
int EditMode::cellSize = 10;

float EditMode::zoom = 1.0f;
QPointF EditMode::zoomCenter = QPointF();
QPointF EditMode::origin = QPointF(0.0f,0.0f);


QTransform EditMode::canvasTransform = QTransform();
QTransform EditMode::canvasInverseTransform = QTransform();

Qt::CursorShape EditMode::mousePointer = Qt::ArrowCursor;

SelectRect  *EditMode::selectRect = nullptr;


EditMode::EditMode()
{
    selectRect = new SelectRect();
}

EditMode::~EditMode()
{
    if (selectRect){
        delete selectRect;
        selectRect = nullptr;
    }

}

QPoint EditMode::Pos2Pixel(QPoint p)
{
    int x = (p.x()) / cellSize;
    int y = (p.y()) / cellSize;
    return QPoint(x,y);
}

QRect EditMode::Pixel2Rect(int px,int py)
{
    return Pixel2Rect(QPoint(px,py));
}

QRect EditMode::Pixel2Rect(QPoint p)
{
    int xLeft = p.x()*cellSize;
    int yTop  = p.y()*cellSize;
    return QRect(xLeft,yTop,cellSize,cellSize);
}

void EditMode::setImage(QSharedPointer<SpriteImage> new_image)
{
    image = new_image;
    image_bak = QImage(image->size(),image->format());
    image_bak.fill(QColor(128, 0, 0, 255));
}

std::tuple<bool,QColor> EditMode::getPixelColor(QPoint pt)
{
    auto pix = Pos2Pixel(pt);
    QRect rect = image->rect();
    if (rect.contains(pix)){
        return {true, image->pixelColor(pix)};
    }
    return {false, QColor(0,0,0,0)};
}

void EditMode::backupImage()
{
    image_bak = image->copy();
}

void EditMode::restoreImage()
{

    QPainter painter(image.get());

    // Disable blending by forcing the source pixels to overwrite the destination
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    painter.drawImage(0, 0, image_bak);

    painter.end();

}
