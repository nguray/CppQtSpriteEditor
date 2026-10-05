
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "newspritedlg.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , editarea(new EditArea(this))
    , palette (new Palette(this))
    , sprites(new Sprites(this))
{

    ui->setupUi(this);

    connect(ui->actionNew, SIGNAL(triggered()),
            this,SLOT(on_actionNewTrigger()));

    connect(ui->actionLine, SIGNAL(triggered()),
            this,SLOT(on_actionLineTrigger()));
    connect(ui->actionRectangle, SIGNAL(triggered()),
            this,SLOT(on_actionRectangleTrigger()));
    connect(ui->actionEllipse, SIGNAL(triggered()),
            this,SLOT(on_actionEllipseTrigger()));

    nbToolbarActions = 0;
    toolbarActions[nbToolbarActions++] = ui->actionLine;
    toolbarActions[nbToolbarActions++] = ui->actionRectangle;
    toolbarActions[nbToolbarActions++] = ui->actionEllipse;

    ui->centralwidget->setLayout(ui->verticalLayout0);

    QHBoxLayout *hlayout = ui->horizontalLayout0;
    hlayout->addWidget(editarea,0);
    hlayout->addWidget(sprites,0);

    QVBoxLayout *layout = ui->verticalLayout0;
    layout->addWidget(palette,0);
    editarea->show();
    palette->show();

    connect(palette, &Palette::foreGroundColorChanged, editarea,
          &EditArea::setForegroundColor);

    palette->load(".","myPalette.txt");

}

MainWindow::~MainWindow()
{
    if (ui) delete ui;
    if (editarea) delete editarea;
    if (palette){
        palette->save(".","myPalette.txt");
        delete palette;
    }
}

void MainWindow::update_toolbar(QAction *selAction)
{
    QAction *a;
    for(int i=0;i<nbToolbarActions;i++){
        if ((a=toolbarActions[i]) && (a!=selAction)){
            a->setDisabled(false);
            a->setChecked(false);
        }
    }
    selAction->setDisabled(true);
}

void MainWindow::on_actionNewTrigger()
{
    NewSpriteDlg newSpriteDlg(this);

    // exec() blocks execution and returns QDialog::Accepted or QDialog::Rejected
    if (newSpriteDlg.exec() == QDialog::Accepted) {
        // User clicked OK - extract data here if needed
        qDebug() << "New Sprite OK";
        qDebug() << "Width : " << newSpriteDlg.getSpriteWidth();
        qDebug() << "Height : " << newSpriteDlg.getSpriteHeight();
        auto img = QSharedPointer<QImage>::create(newSpriteDlg.getSpriteWidth(), newSpriteDlg.getSpriteHeight(), QImage::Format_ARGB32);
        img->fill(QColor(0, 0, 0, 0));
        editarea->setNewSpriteImage(img);
        //EditMode::setImage(QSharedPointer<QImage>::create(32, 32, QImage::Format_ARGB32));
        //EditMode::image->fill(QColor(0, 0, 0, 0));

    } else {
        // User clicked Cancel or closed the window
        qDebug() << "New Sprite Cancel";
    }

}


void MainWindow::on_actionLineTrigger()
{
    update_toolbar(ui->actionLine);

    qDebug() << "Draw Line mode";
    editarea->setPencilMode();

}

void MainWindow::on_actionRectangleTrigger()
{
    update_toolbar(ui->actionRectangle);

    qDebug() << "Draw Rectangle mode";
    editarea->setRectangleMode();

}

void MainWindow::on_actionEllipseTrigger()
{
    update_toolbar(ui->actionEllipse);

    qDebug() << "Draw Ellipse mode";
    editarea->setEllipseMode();

}
