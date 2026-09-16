#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , editarea(new EditArea(this))
    , palette (new Palette(this))
{
    ui->setupUi(this);
    connect(ui->actionLine, SIGNAL(triggered()),this,SLOT(on_actionLineTrigger()));
    connect(ui->actionRectangle, SIGNAL(triggered()),this,SLOT(on_actionRectangleTrigger()));
    connect(ui->actionEllipse, SIGNAL(triggered()),this,SLOT(on_actionEllipseTrigger()));


    nbToolbarActions = 0;
    toolbarActions[nbToolbarActions++] = ui->actionLine;
    toolbarActions[nbToolbarActions++] = ui->actionRectangle;
    toolbarActions[nbToolbarActions++] = ui->actionEllipse;

    ui->centralwidget->setLayout(ui->verticalLayout0);

    QVBoxLayout *layout = ui->verticalLayout0;
    layout->addWidget(editarea,0);
    layout->addWidget(palette,0);
    editarea->show();
    palette->show();

    connect(palette, &Palette::foreGroundColorChanged, editarea,
          &EditArea::setForegroundColor);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete editarea;
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
