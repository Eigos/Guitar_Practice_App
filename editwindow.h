#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QTextEdit>
#include <QVBoxLayout>

#include <vector>
#include <functional>
#include <string>

#include "editwindowUi.h"


typedef QTextEdit ChordWidget;

class EditWindow : public QMainWindow
{
    Q_OBJECT
public:
    EditWindow(QWidget *parent = nullptr);
   ~EditWindow();

private:

    enum class EditSectionEnum{
        NoSelection,
        Tab,
        Lyric,
        Chord,
        Comment
    }CurrentSection;

    QWidget* EditPanel;
    QVBoxLayout* EditPanelContent;

    std::vector<ChordWidget*> ChordWidgetList;


    void ButtonAddTabFunction();
    void ButtonAddLyricsFunction();
    void ButtonAddChordsFunction();

    void ShowOptionsAddLyrics();

    Ui_EditWindow *ui;

    const std::vector<std::string> chords = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"};
    const std::vector<std::string> suffixes = {"major","minor","dim","dim7","sus2","sus4","sus2sus4","7sus4","7/G","alt","aug","5","6","69","7","7b5",
                                               "aug7","9","9b5","aug9","7b9","7#9","11","9#11","13","maj7","maj7b5","maj7#5","maj9"};
    std::vector<QPushButton*> OptionsAddChordsKeyButtons;
    std::vector<QPushButton*> OptionsAddChordsSuffixButtons;
    std::vector<QPushButton*> OptionsAddChordsPositionButtons;
    QScrollArea* OptionsAddChordsKeyLayoutArea;
    QScrollArea* OptionsAddChordsSuffixLayoutArea;
    QScrollArea* OptionsAddChordsPositionsLayoutArea;
    QGridLayout* KeyLayoutContents;
    QGridLayout* SuffixLayoutContents;
    QGridLayout* PositionsLayoutContents;

    void InitOptionsAddChordsButtons();
    void deInitOptionsAddChordsButtons();
    void ShowOptionsAddChordsButtons();
    void HideOptionsAddChordsButtons();



};

#endif // EDITWINDOW_H
