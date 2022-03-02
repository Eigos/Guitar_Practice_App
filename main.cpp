#include "mainwindow.h"
#include "editwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    EditWindow w;
    w.show();
    return a.exec();
}
