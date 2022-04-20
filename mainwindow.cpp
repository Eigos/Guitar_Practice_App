#include "mainwindow.h"
#include "mainwindowUi.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    verticalLayout_scrollArea = static_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->children()[0]);
    std::string filePath = "C:\\Users\\TULPAR\\Documents\\Guitar_Practice_App\\testjson\\testFile.txt";

    std::string songPath = SongsFileDir() + "testFile.txt";
    sectionManager = LoadSongFromFile(songPath);
    ShowSong(sectionManager);

    openEditWindowAction = new QAction("Show edit window");
    ui->menuOptions->addAction(openEditWindowAction);

    connect(openEditWindowAction, &QAction::triggered, this, &MainWindow::ShowEditWindow); // triggerlanmıyor!!!

    InitComboBoxSongs();

}

MainWindow::~MainWindow()
{
    openEditWindowAction->deleteLater();
    delete ui;
}

SectionManager* MainWindow::LoadSongFromFile(std::string filePath)
{
    QFile file(filePath.c_str());

    SectionManager* tempSectionManager = new SectionManager;

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }

    QByteArray fileBytes = file.readAll();

    QJsonParseError jsonError;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileBytes, &jsonError);

    if(jsonError.error != QJsonParseError::NoError){
        qDebug() << "fromJson failed: " << jsonError.errorString();
    }

    if(jsonDocument.isObject() ){
        QJsonObject jsonObject = jsonDocument.object();

        if(jsonObject.contains("ChordLayout")){
            QJsonArray objectArray = jsonObject.value("ChordLayout").toArray(); // ChordLayout array

            for(QJsonValue val : objectArray){ // Chord and id
                ChordLayout* chordLayout = new ChordLayout(new QWidget());

                QJsonObject chordObject = val.toObject();

                for(QJsonValue chordsVal : chordObject.value("Chord").toArray()){

                    std::string chordStr = chordsVal.toString().toStdString();
                    ChordInformation newChord;
                    newChord.setKey({chordStr.c_str()}); //HATA!

                    chordLayout->AddChord(newChord);
                }

                uint32_t objID = chordObject.value("ID").toInt();

                chordLayout->setSectionID(objID);

                tempSectionManager->AddSection(chordLayout);
            }
        }
        if(jsonObject.contains("LyricList")){
            QJsonArray objectArray = jsonObject.value("LyricList").toArray();

            for(QJsonValue objectVal : objectArray){


                QJsonObject jsonObj = objectVal.toObject();

                std::string text = jsonObj.value("Text").toString().toStdString();
                uint32_t objID = jsonObj.value("ID").toInt();

                LyricObject* lyricObject = new LyricObject(new QWidget(), false);
                lyricObject->setSectionID(objID);
                lyricObject->setText(text);


                QJsonArray jsonChords = jsonObj.value("Chords").toArray();

                for(QJsonValue chordsVal : jsonChords){
                    QJsonObject jsonChordObj = chordsVal.toObject();

                    std::string chord = jsonChordObj.value("Chord").toString().toStdString();
                    std::vector<uint32_t> pos;
                    for(uint32_t i = 0; i < jsonChordObj.value("Pos").toArray().size(); i++){
                        pos.push_back(jsonChordObj.value("Pos").toArray()[i].toInt());
                    }

                    ChordLabel* newChord = new ChordLabel(); // HATA!
                    newChord->setKey(chord);
                    if(pos.size() == 2){
                        newChord->move(pos[0],pos[1]);
                    }
                    else {
                        newChord->move(0,0);
                    }

                    lyricObject->AddChord(newChord);

                }

                tempSectionManager->AddSection(lyricObject);
            }

        }
    }


    return tempSectionManager;
}

void MainWindow::ShowSong(SectionManager* sectionManager)
{

const std::vector<SectionInfoWData> tempData = sectionManager->getAllbyIDList();
    for(SectionInfoWData section : tempData){
        switch(section.getSectionType()){
        case EditSectionEnum::Chord : {
            verticalLayout_scrollArea->addWidget(static_cast<ChordLayout*>(section.data)->getWidget());
            break;
            }
        case EditSectionEnum::Lyric : {
            verticalLayout_scrollArea->addWidget(static_cast<LyricObject*>(section.data)->getWidget());
            break;
            }
        }
    }
}

void MainWindow::ShowEditWindow()
{
    editWindow = new EditWindow();
    connect(editWindow, &EditWindow::CloseWindow, [=](){editWindow->deleteLater(); this->show();});
    this->setHidden(true);
    editWindow->show();

}

std::string MainWindow::SongsFileDir()
{
    std::string currentPath = QCoreApplication::applicationDirPath().toStdString();
    std::string songDirPath = currentPath + "/songs/";
    return songDirPath;
}

void MainWindow::ClearView()
{

}

void MainWindow::InitComboBoxSongs()
{
    QDir directory(SongsFileDir().c_str());
    QStringList songList = directory.entryList(QStringList() << "*.txt" << "*.json",QDir::Files);

    ui->comboBox_pick_a_song->addItems(songList);
    connect(ui->comboBox_pick_a_song, &QComboBox::currentIndexChanged, this, &MainWindow::ComboBoxChangeSongFunc);
}

void MainWindow::ComboBoxChangeSongFunc()
{
    std::string newSong = ui->comboBox_pick_a_song->currentText().toStdString();

    delete sectionManager;

    while(!verticalLayout_scrollArea->isEmpty()) {
      delete verticalLayout_scrollArea->takeAt(0);
    }

    std::string targetFilePath = SongsFileDir() + newSong;

    sectionManager = LoadSongFromFile(targetFilePath);
    ShowSong(sectionManager);

}

