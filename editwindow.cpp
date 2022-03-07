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

    connect(ui->button_add_chords, &QPushButton::pressed, std::bind(&EditWindow::ButtonAddChordsFunction, this));
    connect(ui->button_add_lyrics, &QPushButton::pressed, std::bind(&EditWindow::ButtonAddLyricsFunction, this));
    connect(ui->button_chord_lyric_attach, &QPushButton::pressed, std::bind(&EditWindow::OptionsLyricChordAttach, this));


    InitOptionsAddChordsButtons();
    InitOptionsLyricChordAttach();

}


EditWindow::~EditWindow()
{
    delete ui;

    deInitOptionsAddChordsButtons();

}

void EditWindow::AddSectionEditPanel(QWidget *widget)
{
    EditPanelContent->insertWidget(EditPanelContent->count() - 1, widget);
}

void EditWindow::AddSectionEditPanel(ChordLayout* chordSection)
{
    DeleteEmptySections();
    sectionManager.AddSection(chordSection);
    AddSectionEditPanel(chordSection->getWidget());
    lastPlacedSection = EditSectionEnum::Chord;
}

void EditWindow::AddSectionEditPanel(LyricObject* lyricSection)
{
    DeleteEmptySections();
    sectionManager.AddSection(lyricSection);
    AddSectionEditPanel(lyricSection->getWidget());
    lastPlacedSection = EditSectionEnum::Lyric;

}

void EditWindow::RemoveEditPanelSection(ChordLayout *chordSection)
{
    sectionManager.RemoveSection(EditSectionEnum::Chord, chordSection);
    EditPanelContent->removeWidget(chordSection->getWidget());
}

void EditWindow::RemoveEditPanelSection(LyricObject *lyricSection)
{
    sectionManager.RemoveSection(EditSectionEnum::Lyric, lyricSection);
    EditPanelContent->removeWidget(lyricSection->getWidget());
}

void EditWindow::ButtonAddLyricsFunction()
{

//    AddSectionEditPanel(new LyricObject(new QWidget(EditPanel)));
  LyricObject* a =  new LyricObject(new QWidget(EditPanel));
    a->AddChorddd();
          AddSectionEditPanel(a);

    ShowOptionsAddLyrics();
}

void EditWindow::ButtonAddChordsFunction()
{

    if(isOptionsAddChordsMenuOpen == false){
        HideOptionsAddChordsButtons();
        HideOptionsLyricChordAttach();
        ShowOptionsAddChordsButtons();
    }else{
        HideOptionsAddChordsButtons();
    }
}

void EditWindow::DeleteEmptySections()
{
    for(ChordLayout* chordLayout : sectionManager.getChordList()){
        if(chordLayout->isEmpty()){
            RemoveEditPanelSection(chordLayout);
        }
    }

    for(LyricObject* lyricLayout : sectionManager.getLyricList()){
        if(lyricLayout->isEmpty()){
            RemoveEditPanelSection(lyricLayout);
        }
    }

}

void EditWindow::InitOptionsLyricChordAttach()
{
    AttachChordButton = new ChordButton();
    AttachChordButton->setText(PlaceLyricChordAttachButtonText.c_str());
    AttachChordButton->setParent(ui->layout_section_options->widget());
    AttachChordButton->setVisible(false);
    ui->layout_section_options->addWidget(AttachChordButton);

    isOptionsLyricChordAttachMenuOpen = false;

}

void EditWindow::OptionsLyricChordAttach()
{
    if(isOptionsLyricChordAttachMenuOpen == false){
        ShowOptionsLyricChordAttach();
    }else{
        HideOptionsAddChordsButtons();
        HideOptionsLyricChordAttach();
    }
}

void EditWindow::ShowOptionsLyricChordAttach()
{
    ShowOptionsAddChordsButtons();
    PlaceChordButton->setVisible(false);
    AttachChordButton->setVisible(true);
    isOptionsLyricChordAttachMenuOpen = true;
    isOptionsAddChordsMenuOpen = false;
}

void EditWindow::HideOptionsLyricChordAttach()
{
    AttachChordButton->setVisible(false);
    isOptionsLyricChordAttachMenuOpen = false;
}

void EditWindow::UpdateTextAttachChordButton()
{
    AttachChordButton->setText((PlaceLyricChordAttachButtonText + AttachChordButton->key + std::string{" "} + AttachChordButton->suffix).c_str());
}

void EditWindow::PlaceLyricChordAttachButtonFunc()
{

}

void EditWindow::ShowOptionsAddLyrics()
{

    HideOptionsAddChordsButtons();
    HideOptionsLyricChordAttach();

    return;
}

void EditWindow::PlaceChordButtonFunc()
{
    ChordLayout* newChordLayout;
    if(lastPlacedSection != EditSectionEnum::Chord){
         newChordLayout = new ChordLayout(new QWidget(EditPanel));
         AddSectionEditPanel(newChordLayout);
    }else{
        newChordLayout = static_cast<ChordLayout*>(sectionManager.getLastAdded());
    }

    newChordLayout->AddChord(CurrentChordInfo);
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
        ChordButton* newChordButton = new ChordButton;

        newChordButton->key = chords[i].c_str();

        newChordButton->setText(chords[i].c_str());

        OptionsAddChordsKeyButtons.push_back(newChordButton);

        connect(newChordButton, &QPushButton::released, [=]{
            CurrentChordInfo.key = newChordButton->key;
            PlaceChordButton->key = CurrentChordInfo.key;
            AttachChordButton->key = CurrentChordInfo.key;
            UpdateTextAttachChordButton();
            UpdateTextPlaceChordButton();
        });
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
        ChordButton* newChordButton = new ChordButton;

        newChordButton->suffix = suffixes[i].c_str();
        newChordButton->setText(suffixes[i].c_str());

        OptionsAddChordsSuffixButtons.push_back(newChordButton);

        connect(newChordButton, &QPushButton::released, [=]{
            CurrentChordInfo.suffix = newChordButton->suffix;
            PlaceChordButton->suffix = CurrentChordInfo.suffix;
            AttachChordButton->suffix = CurrentChordInfo.suffix;
            UpdateTextAttachChordButton();
            UpdateTextPlaceChordButton();
        });


        SuffixLayoutContents->addWidget(newChordButton, i / 3, i % 3);

    }

    OptionsAddChordsSuffixLayoutArea->setWidget(obj2);

    //Place Chord Button Init
    PlaceChordButton = new ChordButton();//"Place",
    PlaceChordButton->setParent(ui->layout_section_options->widget());
    PlaceChordButton->setText("Place");

    connect(PlaceChordButton, &QPushButton::released, std::bind(&EditWindow::PlaceChordButtonFunc, this));

    //Add all widgets
    ui->layout_section_options->addWidget(OptionsAddChordsKeyLayoutArea);
    ui->layout_section_options->addWidget(OptionsAddChordsSuffixLayoutArea);
    ui->layout_section_options->addWidget(PlaceChordButton);

    HideOptionsAddChordsButtons();

}

void EditWindow::deInitOptionsAddChordsButtons()
{
     for(QPushButton* button : OptionsAddChordsKeyButtons){
         delete button;
     }

     for(QPushButton* button : OptionsAddChordsSuffixButtons){
         delete button;
     }

     //delete OptionsAddChordsPositionsLayoutArea;
     delete OptionsAddChordsSuffixLayoutArea   ;
     delete OptionsAddChordsKeyLayoutArea    ;
}

void EditWindow::ShowOptionsAddChordsButtons()
{
    OptionsAddChordsSuffixLayoutArea->setVisible(true);
    OptionsAddChordsKeyLayoutArea->setVisible   (true);
    PlaceChordButton->setVisible(true);
    isOptionsAddChordsMenuOpen = true;

}

void EditWindow::HideOptionsAddChordsButtons()
{
    OptionsAddChordsSuffixLayoutArea->setVisible(false);
    OptionsAddChordsKeyLayoutArea->setVisible(false);
    PlaceChordButton->setVisible(false);
    isOptionsAddChordsMenuOpen = false;

}

void EditWindow::UpdateTextPlaceChordButton()
{
    PlaceChordButton->setText((std::string{"Place -> "} + PlaceChordButton->key + std::string{" "} + PlaceChordButton->suffix).c_str());
}





const ChordInformation ChordButton::getChordInformation()
{
    return ChordInformation{
        this->key,
        this->suffix,
        this->chordPosition
    };
}















