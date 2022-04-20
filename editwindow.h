#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>



#include <vector>
#include <functional>
#include <string>

#include "editwindowUi.h"
#include "lyricobject.h"
#include "chordmanager.h"
#include "chordlayout.h"
#include "section.h"
#include "sectionmanager.h"
#include "chordimage.h"



typedef QTextEdit ChordWidget;

class ChordButton : public QPushButton, public ChordInformation{

public:
    ChordButton(){};
    ChordButton(ChordInformation& chordInfo) : ChordInformation(chordInfo){};
    const ChordInformation getChordInformation();

};


class EditWindow : public QMainWindow
{
    Q_OBJECT
public:
    EditWindow(QWidget *parent = nullptr);
   ~EditWindow();

signals:
    void CloseWindow();

private:

    ChordManager* chordManager;

    QAction* MenuButtonBack;

    QAction* MenuButtonSave;

    void MenuButtonSaveFunc();
    void MenuButtonBackFunc();


    QWidget* EditPanel;
    QVBoxLayout* EditPanelContent;

    SectionManager sectionManager;

    void AddSectionEditPanel(QWidget* widget);
    void AddSectionEditPanel(ChordLayout* chordSection);
    void AddSectionEditPanel(LyricObject* lyricSection);

    void RemoveEditPanelSection(ChordLayout* chordSection);
    void RemoveEditPanelSection(LyricObject* lyricSection);

    void ButtonAddLyricsFunction();
    void ButtonAddChordsFunction();

    void DeleteEmptySections();


    Ui_EditWindow *ui;

    EditSectionEnum lastPlacedSection = EditSectionEnum::NoSelection;

    //Lyrics / Chord Attachment
    void InitOptionsLyricChordAttach();
    void OptionsLyricChordAttach();
    void ShowOptionsLyricChordAttach();
    void HideOptionsLyricChordAttach();
    void UpdateTextAttachChordButton();
    ChordButton* AttachChordButton;
    const std::string PlaceLyricChordAttachButtonText = "Attach -> ";
    void PlaceLyricChordAttachButtonFunc();
    bool isOptionsLyricChordAttachMenuOpen;

    //Lyrics
    void ShowOptionsAddLyrics();

    //Chords
    const std::vector<std::string> chords = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"};
    const std::vector<std::string> suffixes = {"major","minor","dim","dim7","sus2","sus4","sus2sus4","7sus4","7/G","alt","aug","5","6","69","7","7b5",
                                               "aug7","9","9b5","aug9","7b9","7#9","11","9#11","13","maj7","maj7b5","maj7#5","maj9"};

    std::vector<ChordButton*> OptionsAddChordsKeyButtons;
    std::vector<ChordButton*> OptionsAddChordsSuffixButtons;
    std::vector<ChordButton*> OptionsAddChordsPositionButtons;
    QScrollArea* OptionsAddChordsKeyLayoutArea;
    QScrollArea* OptionsAddChordsSuffixLayoutArea;
    QScrollArea* OptionsAddChordsPositionsLayoutArea;
    QGridLayout* KeyLayoutContents;
    QGridLayout* SuffixLayoutContents;
    QGridLayout* PositionsLayoutContents;
    ChordButton* PlaceChordButton;
    void PlaceChordButtonFunc();
    const std::string PlaceChordButtonText = "Place -> ";
    ChordInformation CurrentChordInfo;
    bool isOptionsAddChordsMenuOpen;

    void InitOptionsAddChordsButtons();
    void deInitOptionsAddChordsButtons();
    void ShowOptionsAddChordsButtons();
    void HideOptionsAddChordsButtons();
    void UpdateTextPlaceChordButton();
    void UpdateChordPositionButtons(ChordKeyEnum key, ChordSuffixEnum suffix);
    void RemoveChordPositionButtons();


};

#endif // EDITWINDOW_H
