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
};

#endif // SPRITEIMAGE_H
