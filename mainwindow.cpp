#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),  editarea(new EditArea(this))
{
    ui->setupUi(this);
    //connect(ui->btnOk, SIGNAL(clicked()),this,SLOT(on_btnOk_Clicked()));
    //onnect(ui->btnCancel, SIGNAL(clicked()),this,SLOT(on_btnCancel_Clicked()));

    ui->centralwidget->setLayout(ui->verticalLayout0);

    QVBoxLayout *layout = ui->verticalLayout0;
    layout->addWidget(editarea,0);
    editarea->show();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete editarea;
}

// void MainWindow::on_btnOk_Clicked()
// {
//     qDebug() << "Ok button clicked";
// }

// void MainWindow::on_btnCancel_Clicked()
// {
//     qDebug() << "Cancel button clicked";
// }
