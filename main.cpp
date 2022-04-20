#include "mainwindow.h"
#include "editwindow.h"

//#include "chordmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    std::string path = "C:\\Users\\TULPAR\\Documents\\Guitar_Practice_App\\AllChords\\guitar_chords.json";
    ChordManager chordManager(path);
    MainWindow w;
    w.show();
    //EditWindow w;
    //w.show();
    return a.exec();
}
