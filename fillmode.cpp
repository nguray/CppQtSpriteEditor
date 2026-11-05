#include "fillmode.h"
#include <stack>

FillMode::FillMode() {
    floodFillPixmap = QPixmap(":/icons/FloodFill.png");
    floodFillCursor = QCursor(floodFillPixmap,5,25);

}

FillMode::~FillMode() {}

void FillMode::init()
{
    mouseCursor = floodFillCursor;
}

void FillMode::updateImage()
{

}

void FillMode::FloodFill(int fillX, int fillY, QColor fillColor) {
    int width, height;
    QColor targetColor, curColor;
    QPoint pt;
    std::stack<QPoint> stk;
    int x, y;
    int startX, startY;
    int xEndLine, xStartLine;
    bool fStartNord, fNord, fStartSud, fSud;
    //-----------------------------------------------------------------------

    targetColor = image->pixelColor(QPoint(fillX, fillY));

    if ((!targetColor.isValid()) || (targetColor == fillColor))
        return;

    width = image->width();
    height = image->height();

    stk.push(QPoint(fillX, fillY));

    while (!stk.empty()) {

        startX = stk.top().x();
        startY = stk.top().y();
        stk.pop();

        //-- Vérifier au Nord
        fStartNord = false;

        if (startY > 0) {
            curColor = image->pixelColor(QPoint(startX, startY - 1));
            if (curColor.isValid()) {
                if (curColor == targetColor) {
                    stk.push(QPoint(startX, startY - 1));
                    fStartNord = true;
                }
            }
        }

        //-- Vérifier au sud
        fStartSud = false;
        if ((startY + 1) < height) {
            curColor = image->pixelColor(QPoint(startX, startY + 1));
            if (curColor.isValid()) {
                if (curColor == targetColor) {
                    stk.push(QPoint(startX, startY + 1));
                    fStartSud = true;
                }
            }
        }

        y = startY;
        //-- Aller vers l'est
        xEndLine = startX;
        fNord = fStartNord;
        fSud = fStartSud;
        if (xEndLine < (width - 1)) {

            x = xEndLine + 1;
            while (x < width) {
                curColor = image->pixelColor(QPoint(x, y));
                if (!curColor.isValid())
                    break;
                if (curColor != targetColor) {
                    break;
                }

                //-- Vérifier au Nord
                if ((y - 1) > 0) {
                    curColor = image->pixelColor(QPoint(x, y - 1));
                    if ((curColor.isValid()) && (curColor == targetColor)) {
                        if (!fNord) {
                            stk.push(QPoint(x, y - 1));
                            fNord = true;
                        }
                    } else {
                        fNord = false;
                    }
                } else {
                    fNord = false;
                }

                //-- Vérifier au sud
                if (y < (height - 1)) {
                    curColor = image->pixelColor(QPoint(x, y + 1));
                    if (curColor.isValid() && (curColor == targetColor)) {
                        if (!fSud) {
                            stk.push(QPoint(x, y + 1));
                            fSud = true;
                        }
                    } else {
                        fSud = false;
                    }
                } else {
                    fSud = false;
                }
                xEndLine = x;
                x++;
            }

        } else {
            xEndLine = width - 1;
        }

        //-- Aller vers l'ouest
        xStartLine = startX;
        fNord = fStartNord;
        fSud = fStartSud;
        if (xStartLine > 0) {
            x = xStartLine - 1;
            while (x >= 0) {

                curColor = image->pixelColor(QPoint(x, y));
                if (!curColor.isValid())
                    break;
                if (curColor != targetColor)
                    break;

                //-- Vérifier au Nord
                if ((y - 1) > 0) {
                    curColor = image->pixelColor(QPoint(x, y - 1));
                    if (curColor.isValid() && (curColor == targetColor)) {
                        if (!fNord) {
                            stk.push(QPoint(x, y - 1));
                            fNord = true;
                        }
                    } else {
                        fNord = false;
                    }
                } else {
                    fNord = false;
                }

                //-- Vérifier au sud
                if (y < (height - 1)) {
                    curColor = image->pixelColor(QPoint(x, y + 1));
                    if (curColor.isValid() && (curColor == targetColor)) {
                        if (!fSud) {
                            stk.push(QPoint(x, y + 1));
                            fSud = true;
                        }
                    } else {
                        fSud = false;
                    }
                } else {
                    fSud = false;
                }
                xStartLine = x;
                x--;
            }

        } else {
            xStartLine = 0;
        }
        //-- Tracer la line
        for (x = xStartLine; x <= xEndLine; x++) {
            image->setPixelColor(QPoint(x, y), fillColor);
        }
    }
}


bool FillMode::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton) {

        QPointF canvasPos = EditMode::canvasInverseTransform.map(event->position());
        auto pt = canvasPos.toPoint();
        auto pix = Pos2Pixel(pt);

        QRect r = image->rect();
        if (r.contains(pix)){
            backupImage();
            FloodFill(pix.x(), pix.y(), foregroundColor);
            return true;
        }

    }

    return false;

}


bool FillMode::mouseMoveEvent(QMouseEvent *event)
{
    QPointF canvasPos = EditMode::canvasInverseTransform.map(event->position());
    auto pt = canvasPos.toPoint();
    auto pix = Pos2Pixel(pt);

    QRect r = image->rect();
    if (r.contains(pix)){
        mouseCursor = floodFillCursor;
    }else{
        mouseCursor = QCursor(Qt::ArrowCursor);
    }


    return true;
}

bool FillMode::mouseReleaseEvent(QMouseEvent *event)
{
     return false;
}

void FillMode::paintEvent(QPaintEvent *event, QPainter *painter)
{
    //
}
