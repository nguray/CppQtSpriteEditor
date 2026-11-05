#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QPixmap>
#include <QCursor>

#include "editmode.h"
#include "pencilmode.h"
#include "rectanglemode.h"
#include "ellipsemode.h"
#include "selectrectmode.h"
#include "pastemode.h"
#include "fillmode.h"
#include "spriteimage.h"


class EditArea : public QWidget
{

    Q_OBJECT

public:

    EditArea(QWidget *parent = nullptr);
    ~EditArea();

    bool isInSelectRectMode();
    void setSelectRectMode();
    void setPencilMode();
    void setRectangleMode();
    void setEllipseMode();
    void setPasteMode();
    void setFillMode();


public slots:
    void setForegroundColor(QColor newColor);
    void setBackgroundColor(QColor newColor);
    void toggleFlash();
    void setNewSpriteImage(QSharedPointer<SpriteImage> newImage);
    void doCopyRectangle();
    void doCutRectangle();
    void doPaste();


signals:
    void imageChanged();
    void enableSave(bool fEnableSave);
    void pickImageColor(QColor c);
    // Declare the signal here. You don't implement this in the .cpp file.
    void showStatusMessage(const QString &message, int timeout = 0);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;    
    void keyReleaseEvent(QKeyEvent *event) override;    

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void drawGrid(QPainter *painter);
    void drawPixels(QPainter *painter);
    void updateSaveMenuItem();
    void initZoomPan(QSize canvasSize, QSize imageSize);

    QTimer  *m_timer;
    QPixmap pipettePixmap;
    QCursor pipetteCursor;

    QPointF lastMousePos;

    bool    modified = false;
    int     myPenWidth = 1;

    QColor  myGridColor = Qt::black;
    QPoint  lastPoint;

    EditMode        *curEditMode=NULL;
    PencilMode      *pencilMode;
    RectangleMode   *rectangleMode;
    EllipseMode     *ellipseMode;
    SelectRectMode  *selectRectMode;
    PasteMode       *pasteMode;
    FillMode        *fillMode;
};