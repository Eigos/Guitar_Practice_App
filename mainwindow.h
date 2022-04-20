#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QDir>

#include "sectionmanager.h"
#include "chordlayout.h"
#include "lyricobject.h"
#include "editwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    SectionManager* LoadSongFromFile(std::string filePath);
    void ShowSong(SectionManager* sectionManager);

private:
    QVBoxLayout *verticalLayout_scrollArea;
    Ui::MainWindow *ui;

    QAction* openEditWindowAction;

    EditWindow *editWindow = nullptr;
    void ShowEditWindow();

    std::string SongsFileDir();

    SectionManager* sectionManager;

    void ClearView();
    void InitComboBoxSongs();

    void ComboBoxChangeSongFunc();
};
#endif // MAINWINDOW_H
