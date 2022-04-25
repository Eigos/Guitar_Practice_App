#include "editwindow.h"
#include "editwindowUi.h"

//to convert ui to h
//C:\Qt\6.2.2\mingw_64\bin\uic -o editwindowUi.h editwindow.ui

EditWindow::EditWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui_EditWindow)
{
    ui->setupUi(this);

    MenuButtonBack = new QAction("Back to View");
    MenuButtonSave = new QAction("Save");
    ui->menubar->addAction(MenuButtonBack);
    ui->menubar->addAction(MenuButtonSave);


    MenuButtonSave->connect(MenuButtonBack, &QAction::triggered, this, &EditWindow::CloseWindow);
    MenuButtonSave->connect(MenuButtonSave, &QAction::triggered, this, &EditWindow::MenuButtonSaveFunc);


    EditPanel = ui->scrollAreaWidgetContents;

    EditPanelContent = static_cast<QVBoxLayout*>(EditPanel->children()[0]);

    connect(ui->button_add_chords, &QPushButton::pressed, std::bind(&EditWindow::ButtonAddChordsFunction, this));
    connect(ui->button_add_lyrics, &QPushButton::pressed, std::bind(&EditWindow::ButtonAddLyricsFunction, this));
    connect(ui->button_chord_lyric_attach, &QPushButton::pressed, std::bind(&EditWindow::OptionsLyricChordAttach, this));

    std::string path = "C:\\Users\\TULPAR\\Documents\\Guitar_Practice_App\\AllChords\\guitar_chords.json";
    chordManager = new ChordManager(path);


    InitOptionsAddChordsButtons();
    InitOptionsLyricChordAttach();

}


EditWindow::~EditWindow()
{
    delete ui;

    deInitOptionsAddChordsButtons();

}

void EditWindow::MenuButtonSaveFunc()
{
    DeleteEmptySections();

    //Variables for chord layout
    QJsonArray jsonChordLayoutArray;

    //Variables for Lyric list
    QJsonArray jsonLyricListArray;

    //Chords used in song
    QJsonArray jsonChordListArray;
    std::vector<ChordInformation*> chordList;


    auto chordListAdd = [&](ChordInformation* newChord) -> uint32_t{
        for(uint32_t i = 0; i < chordList.size(); i++){
            if(chordList[i]->IsEqual(*newChord)){
                return i;
            }
        }

        chordList.push_back(newChord);
        return chordList.size() - 1;
    };



    for(uint32_t i = 0; i < sectionManager.getAllList().size(); i++){
        SectionInfoWData section = sectionManager.getAllList()[i];

        QJsonValue objID(static_cast<int>(i));

        switch(section.getSectionType()){
        case EditSectionEnum::Chord:{

            QJsonArray jsonChordsArray;

            QJsonObject jsonChordLayoutObject;

            for(ChordLabel* chordLabel : static_cast<ChordLayout*> (section.data)->getAllChords()){

                jsonChordsArray.append(static_cast<int>(chordListAdd(chordLabel)));

                jsonChordLayoutObject.insert("ChordID", jsonChordsArray);
            }

            jsonChordLayoutObject.insert("ID", objID);

            jsonChordLayoutArray.append(jsonChordLayoutObject);
            break;
        }

        case EditSectionEnum::Lyric:{

            QJsonObject jsonLyricListObject; // this contains "text", "ChordObj", "ID"

            QJsonArray jsonLyricChordArray; // this contains "chordObj"

            for(ChordLabel* chordLabel : static_cast<LyricObject*> (section.data)->getAllAttachedChords()){

                QJsonObject jsonLyricChordObj; // this contains "chord", "chordPos"
                QJsonArray jsonLyricChordPos; // contains "chordPos"

                //Chord

                //QJsonObject jsonChordKey;
                //QJsonObject jsonChordSuffix;
                //QJsonArray jsonChordFrets;
                //QJsonArray jsonChordFingers;


                jsonLyricChordObj.insert("ChordID", static_cast<int>(chordListAdd(chordLabel)));

                //Chord pos
                jsonLyricChordPos.append(chordLabel->pos().x());
                jsonLyricChordPos.append(chordLabel->pos().y());
                jsonLyricChordObj.insert("Pos", jsonLyricChordPos);

                //Chords Array
                jsonLyricChordArray.append(jsonLyricChordObj);



            }

            //LyricListObj
            LyricObject* lyricObj = static_cast<LyricObject*>(section.data);
            QJsonValue lyricText(lyricObj->getText().c_str());

            jsonLyricListObject.insert("Text", lyricText);
            jsonLyricListObject.insert("Chords", jsonLyricChordArray);
            jsonLyricListObject.insert("ID", objID);

            jsonLyricListArray.append(jsonLyricListObject);

            break;
        }

            break;
        }
    }

    for(uint32_t i = 0; i < chordList.size(); i++){
        QJsonObject jsonChordListArrayObject;

        ChordInformation chord(*chordList[i]);
        QJsonValue key = chord.getKeyStr().c_str();
        QJsonValue suffix = chord.getSuffixStr().c_str();
        QJsonValue chordID = static_cast<int64_t>(i);
        QJsonObject position;

        QJsonArray frets;
        for(uint32_t i = 0; i < std::size(chord.posInfo.frets); i++){
            frets.append(chord.posInfo.frets[i]);
        }

        QJsonArray fingers;
        for(uint32_t i = 0; i < std::size(chord.posInfo.fingers); i++){
            fingers.append(chord.posInfo.fingers[i]);
        }

        QJsonValue baseFret;
        baseFret = chord.posInfo.baseFret;

        QJsonArray barres;
        for(uint32_t i = 0; i < std::size(chord.posInfo.barres); i++){
            barres.append(chord.posInfo.barres[i]);
        }


        position.insert("frets", frets);
        position.insert("fingers", fingers);
        position.insert("baseFret", baseFret);
        position.insert("barres", barres);

        jsonChordListArrayObject.insert("key", key);
        jsonChordListArrayObject.insert("suffix", suffix);
        jsonChordListArrayObject.insert("chordID", chordID);
        jsonChordListArrayObject.insert("positions", position);

        jsonChordListArray.append(jsonChordListArrayObject);
    }

    QJsonObject x;
    x.insert("ChordLayout", jsonChordLayoutArray);
    x.insert("LyricList", jsonLyricListArray);
    x.insert("chords", jsonChordListArray);

    std::string currentPath = QCoreApplication::applicationDirPath().toStdString();
    std::string songDirPath = currentPath + "/songs/";

    QJsonDocument document;
    document.setObject( x);
    QByteArray bytes = document.toJson( QJsonDocument::Indented );
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    songDirPath.c_str(),
                                                    tr("Files (*.json *.txt)"));
    QFile file(fileName);

    if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
    {
        QTextStream iStream( &file );
        //iStream.setCodec( "utf-8" );
        iStream << bytes;
        file.close();
    }
    else
    {
        qDebug() << "file open failed: " << fileName;
    }

}

void EditWindow::MenuButtonBackFunc()
{

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

    AddSectionEditPanel(new LyricObject(new QWidget(EditPanel)));

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
            sectionManager.RemoveSection(EditSectionEnum::Chord, chordLayout);
        }
    }

    for(LyricObject* lyricLayout : sectionManager.getLyricList()){
        if(lyricLayout->isEmpty()){
            RemoveEditPanelSection(lyricLayout);
            sectionManager.RemoveSection(EditSectionEnum::Lyric, lyricLayout);
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
    connect(AttachChordButton, &QPushButton::clicked, this, &EditWindow::PlaceLyricChordAttachButtonFunc);

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
    AttachChordButton->setText((PlaceLyricChordAttachButtonText + AttachChordButton->getKeyStr() + std::string{" "} + AttachChordButton->getSuffixStr()).c_str());

}

void EditWindow::PlaceLyricChordAttachButtonFunc()
{
    std::vector<LyricObject *> lyricSectionList = sectionManager.getLyricList();
    for(LyricObject* lyricSection : lyricSectionList){
        lyricSection->AddChordToPickedWord(CurrentChordInfo);
    }
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
        newChordLayout = new ChordLayout(new QWidget(EditPanel)); //LEAK!
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

        newChordButton->setKey(chords[i].c_str());

        newChordButton->setText(chords[i].c_str());

        OptionsAddChordsKeyButtons.push_back(newChordButton);

        connect(newChordButton, &QPushButton::released, [=]{
            CurrentChordInfo.setKey(newChordButton->getKeyStr());
            PlaceChordButton->setKey(CurrentChordInfo.getKeyStr());
            AttachChordButton->setKey(CurrentChordInfo.getKeyStr());
            UpdateTextAttachChordButton();
            UpdateTextPlaceChordButton();
            RemoveChordPositionButtons();
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

        newChordButton->setSuffix(suffixes[i].c_str());
        newChordButton->setText(suffixes[i].c_str());

        OptionsAddChordsSuffixButtons.push_back(newChordButton);

        connect(newChordButton, &QPushButton::released, [=]{
            CurrentChordInfo.setSuffix(newChordButton->getSuffix());
            PlaceChordButton-> setSuffix(CurrentChordInfo.getSuffix());
            AttachChordButton->setSuffix(CurrentChordInfo.getSuffix());
            UpdateTextAttachChordButton();
            UpdateTextPlaceChordButton();
            RemoveChordPositionButtons();
            UpdateChordPositionButtons(CurrentChordInfo.getKey(), CurrentChordInfo.getSuffix());
        });


        SuffixLayoutContents->addWidget(newChordButton, i / 3, i % 3);

    }

    OptionsAddChordsSuffixLayoutArea->setWidget(obj2);

    //Positions seciton
    OptionsAddChordsPositionsLayoutArea = new QScrollArea();
    OptionsAddChordsPositionsLayoutArea->setWidgetResizable(true);
    QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(OptionsAddChordsPositionsLayoutArea->sizePolicy().hasHeightForWidth());
    OptionsAddChordsPositionsLayoutArea->setSizePolicy(sizePolicy);

    QWidget* obj3 = new QWidget;
    PositionsLayoutContents = new QGridLayout(obj3);

    OptionsAddChordsPositionsLayoutArea->setWidget(obj3);


    //Place Chord Button Init
    PlaceChordButton = new ChordButton();//"Place",
    PlaceChordButton->setParent(ui->layout_section_options->widget());
    PlaceChordButton->setText("Place");

    connect(PlaceChordButton, &QPushButton::released, std::bind(&EditWindow::PlaceChordButtonFunc, this));

    //Add all widgets
    ui->layout_section_options->addWidget(OptionsAddChordsKeyLayoutArea);
    ui->layout_section_options->addWidget(OptionsAddChordsSuffixLayoutArea);
    ui->layout_section_options->addWidget(OptionsAddChordsPositionsLayoutArea);
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

    for(QPushButton* button : OptionsAddChordsPositionButtons){
        delete button;
    }

    //delete OptionsAddChordsPositionsLayoutArea;
    delete OptionsAddChordsSuffixLayoutArea   ;
    delete OptionsAddChordsKeyLayoutArea    ;
    delete OptionsAddChordsPositionsLayoutArea;
}

void EditWindow::ShowOptionsAddChordsButtons()
{
    OptionsAddChordsSuffixLayoutArea->setVisible(true);
    OptionsAddChordsKeyLayoutArea->setVisible   (true);
    OptionsAddChordsPositionsLayoutArea->setVisible(true);
    PlaceChordButton->setVisible(true);
    isOptionsAddChordsMenuOpen = true;

}

void EditWindow::HideOptionsAddChordsButtons()
{
    OptionsAddChordsSuffixLayoutArea->setVisible(false);
    OptionsAddChordsKeyLayoutArea->setVisible(false);
    OptionsAddChordsPositionsLayoutArea->setVisible(false);
    PlaceChordButton->setVisible(false);
    isOptionsAddChordsMenuOpen = false;

}

void EditWindow::UpdateTextPlaceChordButton()
{
    PlaceChordButton->setText((std::string{"Place -> "} + PlaceChordButton->getKeyStr() + std::string{" "} + PlaceChordButton->getSuffixStr()).c_str());
}

void EditWindow::UpdateChordPositionButtons(ChordKeyEnum key, ChordSuffixEnum suffix){
    std::vector<ChordInformation> chordList = chordManager->getChordList(key,suffix);
    for(uint32_t i = 0; i < chordList.size(); i++){
        ChordButton* newChordButton = new ChordButton(chordList[i]);

        OptionsAddChordsPositionButtons.push_back(newChordButton);

        connect(newChordButton, &QPushButton::released, [=]{
            CurrentChordInfo = ChordInformation(chordList[i]);
        });


        ChordImage chordImage(chordList[i]);
        QIcon newIcon(QPixmap::fromImage(chordImage.getImage()));
        //newIcon.actualSize(newChordButton->size());
        //newChordButton->setIcon(newIcon);


        newChordButton->setFixedSize(newIcon.actualSize(newIcon.availableSizes().first()));//never larger than ic.availableSizes().first()
        newChordButton->setText("");
        newChordButton->setIcon(newIcon);
        newChordButton->setIconSize(newIcon.availableSizes().first());


        PositionsLayoutContents->addWidget(newChordButton, i / 3, i % 3);

    }
}

void EditWindow::RemoveChordPositionButtons()
{
    for(uint32_t i = 0; i < OptionsAddChordsPositionButtons.size(); i++){

        PositionsLayoutContents->removeWidget(OptionsAddChordsPositionButtons[i]);

        OptionsAddChordsPositionButtons[i]->deleteLater();
    }

    OptionsAddChordsPositionButtons.clear();
}














