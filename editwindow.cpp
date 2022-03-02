#include "editwindow.h"
#include "editwindowUi.h"

//to convert ui to h
//C:\Qt\6.2.2\mingw_64\bin\uic -o editwindowUi.h editwindow.ui

EditWindow::EditWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui_EditWindow)
{
    ui->setupUi(this);


    EditPanel = ui->scrollAreaWidgetContents;

    EditPanelContent = static_cast<QVBoxLayout*>(EditPanel->children()[0]);
//    EditPanel->setLayout(EditPanelContent);

    connect(ui->button_add_chords, &QPushButton::pressed, std::bind(&EditWindow::ButtonAddChordsFunction, this));
    connect(ui->button_add_lyrics, &QPushButton::pressed, std::bind(&EditWindow::ShowOptionsAddLyrics, this));

    InitOptionsAddChordsButtons();
}


EditWindow::~EditWindow()
{
    delete ui;

    deInitOptionsAddChordsButtons();

    for(ChordWidget* widget : ChordWidgetList){
        delete widget;
    }
}

void EditWindow::ButtonAddTabFunction()
{

}


void EditWindow::ButtonAddChordsFunction()
{

    if(CurrentSection == EditSectionEnum::Chord){
        return;
    }


    ShowOptionsAddChordsButtons();
}

void EditWindow::ShowOptionsAddLyrics()
{

    HideOptionsAddChordsButtons();

    CurrentSection = EditSectionEnum::Lyric;

    return;
}

void EditWindow::InitOptionsAddChordsButtons()
{

    //Chord section
    OptionsAddChordsKeyLayoutArea = new QScrollArea();
    OptionsAddChordsKeyLayoutArea->setWidgetResizable(true);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(OptionsAddChordsKeyLayoutArea->sizePolicy().hasHeightForWidth());
    OptionsAddChordsKeyLayoutArea->setSizePolicy(sizePolicy);

    QWidget* obj = new QWidget;
    KeyLayoutContents = new QGridLayout(obj);

    for(uint32_t i = 0; i < chords.size(); i++){
        QPushButton* newChordButton = new QPushButton;
        newChordButton->setText(chords[i].c_str());
        OptionsAddChordsKeyButtons.push_back(newChordButton);
        KeyLayoutContents->addWidget(newChordButton, i / 3, i % 3);

    }

    OptionsAddChordsKeyLayoutArea->setWidget(obj);

    //Suffix section
    OptionsAddChordsSuffixLayoutArea = new QScrollArea();
    OptionsAddChordsSuffixLayoutArea->setWidgetResizable(true);
    QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(OptionsAddChordsSuffixLayoutArea->sizePolicy().hasHeightForWidth());
    OptionsAddChordsSuffixLayoutArea->setSizePolicy(sizePolicy);

    QWidget* obj2 = new QWidget;
    SuffixLayoutContents = new QGridLayout(obj2);

    for(uint32_t i = 0; i < suffixes.size(); i++){
        QPushButton* newChordButton = new QPushButton;
        newChordButton->setText(suffixes[i].c_str());
        OptionsAddChordsSuffixButtons.push_back(newChordButton);
        SuffixLayoutContents->addWidget(newChordButton, i / 3, i % 3);

    }

    OptionsAddChordsSuffixLayoutArea->setWidget(obj2);


    ui->layout_section_options->addWidget(OptionsAddChordsKeyLayoutArea);
    ui->layout_section_options->addWidget(OptionsAddChordsSuffixLayoutArea);


}

void EditWindow::deInitOptionsAddChordsButtons()
{
     for(QPushButton* button : OptionsAddChordsKeyButtons){
         delete button;
     }

     for(QPushButton* button : OptionsAddChordsSuffixButtons){
         delete button;
     }

     delete OptionsAddChordsPositionsLayoutArea;
     delete OptionsAddChordsSuffixLayoutArea   ;
     delete OptionsAddChordsKeyLayoutArea    ;
}

void EditWindow::ShowOptionsAddChordsButtons()
{
    OptionsAddChordsSuffixLayoutArea->setVisible(true);
    OptionsAddChordsKeyLayoutArea->setVisible   (true);

    CurrentSection = EditSectionEnum::Chord;
}

void EditWindow::HideOptionsAddChordsButtons()
{
    OptionsAddChordsSuffixLayoutArea->setVisible(false);
    OptionsAddChordsKeyLayoutArea->setVisible(false);

    CurrentSection = EditSectionEnum::NoSelection;
}













