#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editarea.h"
#include "palette.h"
#include "sprites.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void update_toolbar(QAction *selAction);

public slots:

private slots:
    void on_actionNewTrigger();
    void on_actionOpenTrigger();
    void on_actionSaveTrigger();
    void on_actionSaveAsTrigger();
    void on_actionLineTrigger();
    void on_actionRectangleTrigger();
    void on_actionEllipseTrigger();
    void handle_enable_save(bool fEnableSave);
    void display_filename(QString fileName);

private:
    Ui::MainWindow *ui;
    EditArea *editarea= nullptr;
    Palette  *palette = nullptr;
    Sprites  *sprites = nullptr;
    int nbToolbarActions = 0;
    QAction *toolbarActions[8];

};
#endif // MAINWINDOW_H