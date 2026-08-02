#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editarea.h"

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

private slots:
    void on_actionLineTrigger();
    void on_actionRectangleTrigger();
    void on_actionEllipseTrigger();

private:
    Ui::MainWindow *ui;
    EditArea *editarea;
    int nbToolbarActions = 0;
    QAction *toolbarActions[8];

};
#endif // MAINWINDOW_H