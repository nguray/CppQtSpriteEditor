
#include "editarea.h"
#include <QMainWindow>

EditArea::EditArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    // 1. Allow the widget to get focus by clicking or tabbing
    setFocusPolicy(Qt::StrongFocus);
    // Enable tracking to get move events without clicking
    setMouseTracking(true);

    //EditMode::image = QImage(32, 32, QImage::Format_ARGB32);
    EditMode::setImage(QSharedPointer<SpriteImage>::create(32, 32, QImage::Format_ARGB32));
    EditMode::image->fill(QColor(0, 0, 0, 0));
    EditMode::backupImage();

    pencilMode = new PencilMode();
    rectangleMode = new RectangleMode();
    ellipseMode = new EllipseMode();
    selectRectMode = new SelectRectMode();
    pasteMode = new PasteMode();

    setPencilMode();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &EditArea::toggleFlash);


    pipettePixmap = QPixmap("://icons/pipette.png");
    pipetteCursor = QCursor(pipettePixmap, 4, 26);

    EditMode::canvasTransform.reset();

}

EditArea::~EditArea()
{
    if (pencilMode) delete pencilMode;
    if (rectangleMode) delete rectangleMode;
    if (ellipseMode) delete ellipseMode;
    if (selectRectMode) delete selectRectMode;
    if (pasteMode) delete pasteMode;

}

bool EditArea::isInSelectRectMode()
{
    return curEditMode==selectRectMode;
}

void EditArea::toggleFlash()
{
    pencilMode->toggleFlash();
    update();
}

void EditArea::drawPixels(QPainter *painter)
{
    int w = EditMode::image->size().width();
    int h = EditMode::image->size().height();
    int s = EditMode::cellSize - 2;
    for (int row = 0; row < h; row++)
    {
        for (int col = 0; col < w; col++)
        {
            
            QColor c = EditMode::image->pixelColor(col,row);

            if (c.alpha()){
                int x = col*EditMode::cellSize+1;
                int y = row*EditMode::cellSize+1;

                painter->setBrush(QBrush(c));
                painter->setPen(QPen(c));
                painter->drawRect(x,y,s,s);
            }
        }
        
    }
    
}

void EditArea::mousePressEvent(QMouseEvent *event)
{
    setFocus(Qt::MouseFocusReason);
    event->accept();

    if (event->button() == Qt::MiddleButton){
        // Store Starting position for computing canvas translation when mouse is moved
        lastMousePos = event->position();

    }else if (EditMode::fControlKey){
        auto pt = event->position().toPoint();
        auto [fOk, c] = EditMode::getPixelColor(pt);
        if (fOk){
            EditMode::foregroundColor = c;
            emit(pickImageColor(c));
        }
        setCursor(pipetteCursor);
    }else{
        if (curEditMode->mousePressEvent(event)){
            update();
            emit(imageChanged());
            updateSaveMenuItem();
        }
        // Change EditArea widget cursor if required
        if (cursor().shape()!=curEditMode->mousePointer){
            setCursor(curEditMode->mousePointer);
        }
    }

}

void EditArea::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // Handle left double-click
        qDebug() << "Left mouse double-clicked at:" << event->pos();
    }

    // Always call the base class implementation if you want standard behavior
    QWidget::mouseDoubleClickEvent(event);
}

void EditArea::mouseMoveEvent(QMouseEvent *event)
{
    setFocus(Qt::MouseFocusReason);
    event->accept();

    if ((event->buttons() & Qt::MiddleButton)){
        // Check whether the mouse cursor is within the widget
        if (rect().contains(event->position().toPoint())){
            // Compute canvas translation
            EditMode::origin += event->position() - lastMousePos;
            lastMousePos = event->position();
            update();
        }
    }else if (EditMode::fControlKey){
        setCursor(pipetteCursor);
    }else{
        if (curEditMode->mouseMoveEvent(event)){
            update();
            emit(imageChanged());
            updateSaveMenuItem();
        }

        // Change EditArea widget cursor if required
        if (cursor().shape()!=curEditMode->mousePointer){
            setCursor(curEditMode->mousePointer);
        }
    }

}

void EditArea::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();

    if (event->button() == Qt::MiddleButton){
        //
    }else if (EditMode::fControlKey){
        setCursor(pipetteCursor);
    }else{
        if (curEditMode->mouseReleaseEvent(event)){
            update();
        }
        // Change EditArea widget cursor if required
        if (cursor().shape()!=curEditMode->mousePointer){
            setCursor(curEditMode->mousePointer);
        }
    }
}

void EditArea::wheelEvent(QWheelEvent *event)
{
    // Vertical scroll delta (positive = up/forward, negative = down/backward)
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        if (numSteps.y()>0){
            float nv = EditMode::zoomFactor + 0.04f;
            if (nv<10.0f){
                EditMode::zoomFactor = nv;
                update();
            }
        }else{
            float nv = EditMode::zoomFactor - 0.04f;
            if (nv>0.1f){
                EditMode::zoomFactor = nv;
                update();
            }
        }
        //qDebug() << "Canvas Scale: " << EditMode::zoom;
    }

    // Horizontal scroll delta
    //int xDelta = event->angleDelta().x();

    // Mark event as accepted
    event->accept();

}

void EditArea::keyPressEvent(QKeyEvent *event)
{
    //--
    event->accept();
    if (event->key()==Qt::Key_Shift){
        EditMode::fShiftKey = true;
        m_timer->start(400); // Toggle every 400 milliseconds
        update();
    }else if (event->key()==Qt::Key_Control){
        EditMode::fControlKey = true;
        setCursor(pipetteCursor);
        emit showStatusMessage("Pipette Mode", 3000); // 3 seconds

    }
    QWidget::keyPressEvent(event);

}    

void EditArea::keyReleaseEvent(QKeyEvent *event)
{
    //--
    if (event->key()==Qt::Key_Shift){
        EditMode::fShiftKey = false;
         m_timer->stop();
        update();
    }else if (event->key()==Qt::Key_Control){
        EditMode::fControlKey = false;
        setCursor(Qt::ArrowCursor);
    }
    QWidget::keyReleaseEvent(event);

}    

void EditArea::resizeEvent(QResizeEvent *event)
{
    initZoomPan(event->size(), EditMode::image->size());

    QWidget::resizeEvent(event);
}

void EditArea::initZoomPan(QSize canvasSize, QSize imageSize)
{

    int minDim = canvasSize.height();
    if (minDim>canvasSize.width()){
        minDim = canvasSize.width();
    }
    EditMode::cellSize = (minDim-4) / imageSize.width();

    EditMode::zoomFactor = 1.0f;

    auto w = EditMode::cellSize*imageSize.width();
    auto h = EditMode::cellSize*imageSize.height();
    EditMode::origin.setX(canvasSize.width()/2.0f - w/2.0f);
    EditMode::origin.setY(canvasSize.height()/2.0f - h/2.0f);

    EditMode::zoomCenter.setX(w/2.0f);
    EditMode::zoomCenter.setY(h/2.0f);

}



void EditArea::drawGrid(QPainter *painter)
{
    int x,y;
    int xLeft = 0;
    int yTop = 0;
    painter->setPen(QPen(myGridColor, 0, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));
    for (int i=0;i<=EditMode::image->size().height();i++){
        for (int j=0;j<=EditMode::image->size().width();j++){
            x = xLeft + j * EditMode::cellSize;
            y = yTop + i * EditMode::cellSize;

            painter->drawPoint(QPoint(x,y));
        }
    }

}

void EditArea::paintEvent(QPaintEvent *event)
{ 
    QPainter painter(this);
    int x,y;

    QRect r = event->rect();
    painter.fillRect(r,QColor(220,220,204));

    //-- Canvas Zoom Pan settings of the edit image

    EditMode::image->canvasSize = size(); // Store canvas size to trigger init Zoom Pan
    EditMode::image->cellSize   = EditMode::cellSize;
    EditMode::image->zoomFactor = EditMode::zoomFactor;
    EditMode::image->origin     = EditMode::origin;
    EditMode::image->zoomCenter = EditMode::zoomCenter;

    painter.save();
    EditMode::canvasTransform.reset();
    EditMode::canvasTransform.translate(EditMode::origin.x(),EditMode::origin.y());


    EditMode::canvasTransform.translate(EditMode::zoomCenter.x(), EditMode::zoomCenter.y());
    EditMode::canvasTransform.scale(EditMode::zoomFactor, EditMode::zoomFactor);
    EditMode::canvasTransform.translate(-EditMode::zoomCenter.x(), -EditMode::zoomCenter.y());

    painter.setTransform(EditMode::canvasTransform);
    EditMode::canvasInverseTransform = EditMode::canvasTransform.inverted();

    auto w = EditMode::cellSize*EditMode::image->size().width();
    auto h = EditMode::cellSize*EditMode::image->size().height();
    r.setRect(0,0,w,h);
    painter.fillRect(r,QColor(253,253,237));

    // auto xc = (int) EditMode::zoomCenter.x();
    // auto yc = (int) EditMode::zoomCenter.y();
    // x = xc+100;
    // y = yc+100;
    // painter.drawLine(QPoint(x-10,y-10), QPoint(x+10,y-10));
    // painter.drawLine(QPoint(x+10,y-10), QPoint(x+10,y+10));
    // painter.drawLine(QPoint(x+10,y+10), QPoint(x-10,y+10));
    // painter.drawLine(QPoint(x-10,y+10), QPoint(x-10,y-10));

    // x = xc;
    // y = yc;
    // painter.drawLine(QPoint(x-10,y-10), QPoint(x+10,y-10));
    // painter.drawLine(QPoint(x+10,y-10), QPoint(x+10,y+10));
    // painter.drawLine(QPoint(x+10,y+10), QPoint(x-10,y+10));
    // painter.drawLine(QPoint(x-10,y+10), QPoint(x-10,y-10));

    // auto w = EditMode::cellSize*EditMode::image->size().width()/2;
    // auto h = EditMode::cellSize*EditMode::image->size().height()/2;
    // x = xc;
    // y = yc;
    // painter.drawLine(QPoint(x-w,y-h), QPoint(x+w,y-h));
    // painter.drawLine(QPoint(x+w,y-h), QPoint(x+w,y+h));
    // painter.drawLine(QPoint(x+w,y+h), QPoint(x-w,y+h));
    // painter.drawLine(QPoint(x-w,y+h), QPoint(x-w,y-h));


    drawGrid(&painter);

    drawPixels(&painter);

    if (!EditMode::image->isNull()){
        int w = EditMode::image->size().width();
        int h = EditMode::image->size().height();
        painter.drawImage(QRect(w*EditMode::cellSize+10,4,w,h), *EditMode::image, EditMode::image->rect());

        painter.drawImage(QRect(w*EditMode::cellSize+10,128,w,h), EditMode::image_bak, EditMode::image_bak.rect());

        if (!EditMode::image_cpy.isNull()){
            int cpy_w = EditMode::image_cpy.size().width();
            int cpy_h = EditMode::image_cpy.size().height();
            painter.drawImage(QRect(w*EditMode::cellSize+10,280,cpy_w,cpy_h), EditMode::image_cpy, EditMode::image_cpy.rect());
        }
    }

    //qDebug() << "EditArea Paint Event";

    curEditMode->paintEvent(event,&painter);

    painter.restore();

}

void EditArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(EditMode::image.get());
    painter.setPen(QPen(EditMode::foregroundColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void EditArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void EditArea::setForegroundColor(QColor newColor) {
    EditMode::foregroundColor = newColor;
    curEditMode->updateImage();
    update();
}

void EditArea::setBackgroundColor(QColor newColor) {
    EditMode::backgroundColor = newColor;
    curEditMode->updateImage();
    update();
}

void EditArea::updateSaveMenuItem()
{
    bool fEnableSave = ((EditMode::image->fModified) && (!EditMode::image->fileName.isEmpty()));
    emit(enableSave(fEnableSave));

}

void EditArea::setNewSpriteImage(QSharedPointer<SpriteImage> newImage)
{
    if (curEditMode==pasteMode){
        setSelectRectMode();
    }else
        curEditMode->init();
    EditMode::setImage(newImage);
    EditMode::backupImage();
    if (newImage->canvasSize!=size()){
        initZoomPan(size(), newImage->size());
        newImage->cellSize   = EditMode::cellSize;
        newImage->zoomFactor = EditMode::zoomFactor;
        newImage->origin     = EditMode::origin;
        newImage->zoomCenter = EditMode::zoomCenter;

    }else{

        EditMode::cellSize = newImage->cellSize;
        EditMode::zoomFactor = newImage->zoomFactor;
        EditMode::origin = newImage->origin;
        EditMode::zoomCenter = newImage->zoomCenter;

    }
    updateSaveMenuItem();
    update();

}

void EditArea::doCopyRectangle()
{
    qDebug() << "Copy Select Rectangle";
    QRect r = EditMode::selectRect->getPixRect();
    r.adjust(0,0,1,1);
    EditMode::image_cpy = EditMode::image->copy(r);
    curEditMode->init();

}

void EditArea::doCutRectangle()
{
    qDebug() << "Cut Select Rectangle";
    QRect r = EditMode::selectRect->getPixRect();
    r.adjust(0,0,1,1);
    EditMode::image_cpy = EditMode::image->copy(r);

    QPainter painter(EditMode::image.get());
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    r = EditMode::selectRect->getPixRect();
    r.adjust(0,0,1,1);
    painter.fillRect(r,EditMode::backgroundColor);
    //painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    curEditMode->init();
}

void EditArea::doPaste()
{
    qDebug() << "Paste image";
    if (!EditMode::image_cpy.isNull()){
        setPasteMode();
    }
}

void EditArea::setSelectRectMode()
{
    curEditMode = selectRectMode;
    curEditMode->init();
    update();
}

void EditArea::setPencilMode()
{
    curEditMode = pencilMode;
    curEditMode->init();
    update();
}

void EditArea::setRectangleMode()
{
    curEditMode = rectangleMode;
    curEditMode->init();
    update();
}

void EditArea::setEllipseMode()
{
    curEditMode = ellipseMode;
    curEditMode->init();
    update();
}

void EditArea::setPasteMode()
{
    curEditMode = pasteMode;
    curEditMode->init();
    update();

}

void EditArea::setFillMode()
{

}


