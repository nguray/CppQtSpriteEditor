
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QString>
#include <QDebug>

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
    connect(ui->actionOpen, SIGNAL(triggered()),
            this,SLOT(on_actionOpenTrigger()));
    connect(ui->actionSave, SIGNAL(triggered()),
            this,SLOT(on_actionSaveTrigger()));
    connect(ui->actionSave_As, SIGNAL(triggered()),
            this,SLOT(on_actionSaveAsTrigger()));

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


    connect(sprites,&Sprites::spriteChanged,editarea,&EditArea::setNewSpriteImage);
    connect(editarea,&EditArea::imageChanged,sprites,&Sprites::on_imageChanged);


    sprites->newSprite(32,32);


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
    sprites->newCurrentSprite();

}

void MainWindow::on_actionOpenTrigger()
{
    qDebug() << "Load current sprite image";
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
        sprites->loadCurrentSprite(fileName);

    } else {
        qDebug() << "File selection cancelled.";
    }

}

void MainWindow::on_actionSaveTrigger()
{
    qDebug() << "Save current sprite image";
    sprites->SaveCurrentSprite();

}

void MainWindow::on_actionSaveAsTrigger()
{
    qDebug() << "Save as current sprite image";
    sprites->SaveAsCurrentSprite();
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
