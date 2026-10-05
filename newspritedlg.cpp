#include "newspritedlg.h"
#include "ui_newspritedlg.h"
#include <QIntValidator>

NewSpriteDlg::NewSpriteDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewSpriteDlg)
{
    ui->setupUi(this);

    // Restrict lineEdit input to integers between 0 and 9999
    ui->WidthEdit->setValidator(new QIntValidator(0, 1024, this));
    ui->HeightEdit->setValidator(new QIntValidator(0, 1024, this));
}

NewSpriteDlg::~NewSpriteDlg()
{
    delete ui;
}

int NewSpriteDlg::getSpriteWidth()
{
    return ui->WidthEdit->text().toInt();
}

int NewSpriteDlg::getSpriteHeight()
{
    return ui->HeightEdit->text().toInt();
}
