#include "spriteimage.h"

SpriteImage::SpriteImage():QImage(),fileName(""),fModified(false)
{

}

SpriteImage::SpriteImage(int width, int height, QImage::Format format):
    QImage(width,height,format),fileName(""),fModified(false)
{

}
