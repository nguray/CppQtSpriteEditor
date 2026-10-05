#ifndef NEWSPRITEDLG_H
#define NEWSPRITEDLG_H

#include <QDialog>

namespace Ui {
class NewSpriteDlg;
}

class NewSpriteDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NewSpriteDlg(QWidget *parent = nullptr);
    ~NewSpriteDlg();

    int getSpriteWidth();
    int getSpriteHeight();

private:
    Ui::NewSpriteDlg *ui;
};

#endif // NEWSPRITEDLG_H
