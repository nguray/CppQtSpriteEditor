
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

    // File Menu actions
    connect(ui->actionNew, SIGNAL(triggered()),
            this,SLOT(on_actionNewTrigger()));
    connect(ui->actionOpen, SIGNAL(triggered()),
            this,SLOT(on_actionOpenTrigger()));
    connect(ui->actionSave, SIGNAL(triggered()),
            this,SLOT(on_actionSaveTrigger()));
    connect(ui->actionSave_As, SIGNAL(triggered()),
            this,SLOT(on_actionSaveAsTrigger()));

    // Edit Menu actions
    connect(ui->actionCopy, SIGNAL(triggered()),
            this,SLOT(on_actionCopyTrigger()));
    connect(ui->actionCut, SIGNAL(triggered()),
            this,SLOT(on_actionCutTrigger()));
    connect(ui->actionPaste, SIGNAL(triggered()),
            this,SLOT(on_actionPasteTrigger()));

    ui->actionSave->setDisabled(true);

    // ToolBar actions
    connect(ui->actionSelection_rect, SIGNAL(triggered()),
            this,SLOT(on_actionSelectRectTrigger()));
    connect(ui->actionPencil, SIGNAL(triggered()),
            this,SLOT(on_actionPencilTrigger()));
    connect(ui->actionRectangle, SIGNAL(triggered()),
            this,SLOT(on_actionRectangleTrigger()));
    connect(ui->actionEllipse, SIGNAL(triggered()),
            this,SLOT(on_actionEllipseTrigger()));
    connect(ui->actionFill, SIGNAL(triggered()),
            this,SLOT(on_actionFillTrigger()));


    nbToolbarActions = 0;
    toolbarActions[nbToolbarActions++] = ui->actionSelection_rect;
    toolbarActions[nbToolbarActions++] = ui->actionPencil;
    toolbarActions[nbToolbarActions++] = ui->actionRectangle;
    toolbarActions[nbToolbarActions++] = ui->actionEllipse;
    toolbarActions[nbToolbarActions++] = ui->actionFill;

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


    sprites->newSprite(64,64);


    connect(palette, &Palette::foreGroundColorChanged, editarea,
            &EditArea::setForegroundColor);
    connect(editarea,&EditArea::pickImageColor, palette,
            &Palette::onSetForeGroundColor);


    palette->load(".","myPalette.txt");

    connect(editarea,&EditArea::enableSave,this,&MainWindow::handle_enable_save);
    connect(sprites,&Sprites::enableSave,this,&MainWindow::handle_enable_save);
    connect(sprites,&Sprites::fileNameChanged,this,&MainWindow::display_filename);


    connect(editarea, &EditArea::showStatusMessage,
            ui->statusbar, &QStatusBar::showMessage);


    on_actionPencilTrigger();

}

MainWindow::~MainWindow()
{

    palette->save(".","myPalette.txt");

    if (ui) delete ui;
    if (editarea) delete editarea;
    if (palette){
        palette->save(".","myPalette.txt");
        delete palette;
    }
    if (sprites) delete sprites;
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
    selAction->setDisabled(false);
    selAction->setChecked(true);
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

        statusBar()->showMessage("Image loaded ...");

    } else {
        qDebug() << "File selection cancelled.";
    }

}

void MainWindow::on_actionSaveTrigger()
{
    qDebug() << "Save current sprite image";
    sprites->SaveCurrentSprite();
    statusBar()->showMessage("Image saved");

}

void MainWindow::on_actionSaveAsTrigger()
{
    qDebug() << "Save as current sprite image";
    sprites->SaveAsCurrentSprite();
}

void MainWindow::on_actionCopyTrigger()
{
    if (editarea->isInSelectRectMode()){
        editarea->doCopyRectangle();
    }
}

void MainWindow::on_actionCutTrigger()
{
    if (editarea->isInSelectRectMode()){
        editarea->doCutRectangle();
    }
}

void MainWindow::on_actionPasteTrigger()
{
    editarea->doPaste();
}

void MainWindow::on_actionSelectRectTrigger()
{
    update_toolbar(ui->actionSelection_rect);

    editarea->setSelectRectMode();
    statusBar()->showMessage("Select rectangle mode", 3000);

}

void MainWindow::on_actionPencilTrigger()
{
    update_toolbar(ui->actionPencil);

    editarea->setPencilMode();
    statusBar()->showMessage("Pencil mode", 3000);

}

void MainWindow::on_actionRectangleTrigger()
{
    update_toolbar(ui->actionRectangle);

    editarea->setRectangleMode();
    statusBar()->showMessage("Draw rectangle mode", 3000);

}

void MainWindow::on_actionEllipseTrigger()
{
    update_toolbar(ui->actionEllipse);

    editarea->setEllipseMode();
    statusBar()->showMessage("Draw ellipse mode", 3000);

}

void MainWindow::on_actionFillTrigger()
{
    update_toolbar(ui->actionFill);

    editarea->setFillMode();
    statusBar()->showMessage("Flood fill mode", 3000);

}

void MainWindow::handle_enable_save(bool fEnableSave)
{
    ui->actionSave->setDisabled(!fEnableSave);
}

void MainWindow::display_filename(QString fileName)
{
    QString title("SpriteEditor");
    if (!fileName.isEmpty()){
        title += " : " + fileName;
    }
    this->setWindowTitle(title);
}

