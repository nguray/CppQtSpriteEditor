#ifndef SPRITEIMAGE_H
#define SPRITEIMAGE_H

#include <QImage>

class SpriteImage : public QImage
{
public:
    SpriteImage();
    SpriteImage(int width, int height, QImage::Format format);
    QString fileName;
    bool    fModified=0;
    QSize   canvasSize;
    int     cellSize  = 5;
    float   zoomFactor=1.0f;
    QPointF zoomCenter;
    QPointF canvasTranslation;

};

#endif // SPRITEIMAGE_H
