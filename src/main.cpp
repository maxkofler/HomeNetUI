#include "mainwindow.h"

#include <QApplication>

#include "log.h"

Log* hlog;

int main(int argc, char *argv[])
{
    hlog = new Log(Log::D, false);
    FUN();

    LOGI("Starting HomeNetUI...");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
