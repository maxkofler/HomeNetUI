#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "log.h"

#include "hnvalues.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    HNValues*                           values;
};
#endif // MAINWINDOW_H
