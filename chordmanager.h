#ifndef CHORDMANAGER_H
#define CHORDMANAGER_H

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QDir>


enum ChordKeyEnum : int8_t{
    NoSelection = -1,
    C,
    Csharp, //C#
    D,
    Eflat, //Eb
    E,
    F,
    Fsharp, //F#
    G,
    Aflat, //Ab
    A,
    Bflat, //Bb
    B,
};

enum ChordSuffixEnum : int8_t{
    noSelection = -1,
    major,
    minor,
    dim,
    dim7,
    sus2,
    sus4,
    sus2sus4,
    _7sus4,     //7sus4
    _7_G,       // 7/G
    alt,
    aug,
    _5,         //5
    _6,         //6
    _69,        // 69
    _7,         // 7
    _7b5,       // 7b5
    aug7,
    _9,         //9
    _9b5,       //9b5
    aug9,
    _7b9,       //7b9
    _7sharp9,   //7#9
    _11,        //11
    _9sharp11,  //9#11
    _13,        //13
    maj7,
    maj7b5,
    maj7sharp5, //maj7#5
    maj9
};

static const std::vector<std::string> ChordKeyStrList = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"};
static const std::vector<std::string> ChordSuffixStrList = {"major","minor","dim","dim7","sus2","sus4","sus2sus4","7sus4","7/G","alt","aug","5","6","69","7","7b5",
                                           "aug7","9","9b5","aug9","7b9","7#9","11","9#11","13","maj7","maj7b5","maj7#5","maj9"};
static const uint32_t ChordKeyCount = 12;
static const uint32_t ChordSuffixCount = 29;

static const uint32_t FindStrInVector(const std::string& str, const std::vector<std::string>& vec);


class ChordInformation;
class ChordManager;

class ChordManager
{
public:
    struct SuffixListStruct;
    struct KeyListSturct;

    ChordManager(std::string sourcePath);

    std::string getKeyStr(ChordKeyEnum key);
    std::string getSuffixStr(ChordSuffixEnum suffix);

    std::vector<ChordInformation> getChordList(ChordKeyEnum key, ChordSuffixEnum suffix);
    ChordInformation getChord(ChordKeyEnum key, ChordSuffixEnum suffix, int32_t pos = 0);

    std::vector<ChordSuffixEnum> getSuffixesOfKey(ChordKeyEnum key);

    //categorizes chords to its key and suffix
    //make sure no duplication of chord information in chordList
    std::vector<KeyListSturct> OptimizeChordInformation(std::vector<ChordInformation> chordList);


    struct SuffixListStruct{
        ChordSuffixEnum suffix = ChordSuffixEnum::noSelection;
        std::vector<ChordInformation> positionList;
    };

    struct KeyListSturct{
        ChordKeyEnum key = ChordKeyEnum::NoSelection;
        std::vector<SuffixListStruct> suffixList;
    };

private:

    void LoadFromFile(std::string sourcePath);
    std::vector<KeyListSturct> allChordsList;


};

class ChordInformation{

public:
    ChordInformation(const ChordInformation& otherObj);

    void setKey(std::string key);
    void setKey(ChordKeyEnum keyID);
    void setSuffix(std::string suffix);
    void setSuffix(ChordSuffixEnum suffixID);

    ChordKeyEnum getKey() const;
    std::string getKeyStr() const;

    ChordSuffixEnum getSuffix() const;
    std::string getSuffixStr() const;

    struct PositionInformation{
        int8_t frets[6]= {-1};
        int8_t fingers[6] = {-1};
        int8_t baseFret;
        std::vector<int8_t> barres;
        bool capo;

        void ZeroInfo(){
            std::fill_n(frets, 6, -1);
            std::fill_n(fingers, 6, -1);
            baseFret = -1;
            capo = false;
            barres.clear();
        }

    };

    bool IsEqual(const ChordInformation& other);

    ChordInformation(){ResetInformation();}

    void ResetInformation(){
        keyID = NoSelection;
        suffixID = noSelection;
        posInfo.ZeroInfo();
    }

    PositionInformation posInfo;

private:

    ChordKeyEnum keyID;
    ChordSuffixEnum suffixID;

};


#endif // CHORDMANAGER_H
