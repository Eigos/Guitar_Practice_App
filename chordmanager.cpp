#include "chordmanager.h"

ChordManager::ChordManager(std::string sourcePath)
{
    LoadFromFile(sourcePath);
}

std::string ChordManager::getKeyStr(ChordKeyEnum key)
{
    return ChordKeyStrList[key];
}

std::string ChordManager::getSuffixStr(ChordSuffixEnum suffix)
{
    return ChordSuffixStrList[suffix];
}

std::vector<ChordInformation> ChordManager::getChordList(ChordKeyEnum key, ChordSuffixEnum suffix)
{
    //return allChordsList[key].suffixList[suffix].positionList;
    for(uint32_t i = 0; i < allChordsList.size(); i++){
        if(allChordsList[i].key == key){
            for(uint32_t j = 0; j < allChordsList[i].suffixList.size(); j++){
                if(allChordsList[i].suffixList[j].suffix == suffix){
                    return allChordsList[i].suffixList[j].positionList;
                }
            }
        }
    }
}

ChordInformation ChordManager::getChord(ChordKeyEnum key, ChordSuffixEnum suffix, int32_t pos)
{
    //return allChordsList[key].suffixList[suffix].positionList[pos];

    // -_-
    for(uint32_t i = 0; i < allChordsList.size(); i++){
        if(allChordsList[i].key == key){
            for(uint32_t j = 0; j < allChordsList[i].suffixList.size(); j++){
                if(allChordsList[i].suffixList[j].suffix == suffix){
                    if(allChordsList[i].suffixList[j].positionList.size() <= pos ){
                        return allChordsList[i].suffixList[j].positionList[pos];
                    }
                }
            }

        }
    }

}

std::vector<ChordManager::KeyListSturct> ChordManager::OptimizeChordInformation(std::vector<ChordInformation> chordList)
{
    std::vector<KeyListSturct> keyList;

    while(chordList.size() != 0){
        ChordInformation chord(*chordList.end().base());
        for(uint32_t i = 0; i < keyList.size(); i++){
            if(keyList[i].key == chord.getKey()){
                //if(keyList[i].suffixList)
            }
            else if(i == keyList.size() - 1){
                KeyListSturct newKey;
                newKey.key = chord.getKey();

                SuffixListStruct newSuffix;
                newSuffix.suffix = chord.getSuffix();
                newSuffix.positionList.push_back(chord);

                newKey.suffixList.push_back(newSuffix);
                keyList.push_back(newKey);
                chordList.pop_back();
            }
        }
    }

    return keyList;
}


void ChordManager::LoadFromFile(std::string sourcePath)
{

    QFile file(sourcePath.c_str());

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

        if(jsonObject.contains("chords")){
            QJsonObject chordObj = jsonObject.value("chords").toObject(); // ChordLayout array

            for(QJsonObject::Iterator keyIter = chordObj.begin(); keyIter != chordObj.end(); keyIter++){ //iterates through keys

                KeyListSturct newKey;
                allChordsList.push_back(newKey);

                QJsonArray suffixArray = keyIter.value().toArray();
                for(QJsonArray::Iterator suffixIter = suffixArray.begin(); suffixIter != suffixArray.end(); suffixIter++){ //iterates through suffixes
                    QJsonObject jsonChordInformation = suffixIter->toObject();

                    SuffixListStruct newSuffixList;
                    newSuffixList.suffix = static_cast<ChordSuffixEnum>(FindStrInVector(jsonChordInformation.value("suffix").toString().toStdString(), ChordSuffixStrList));
                    newKey.key = static_cast<ChordKeyEnum>(FindStrInVector(jsonChordInformation.value("key").toString().toStdString(), ChordKeyStrList));

                    QJsonArray jsonChordPositions = jsonChordInformation.value("positions").toArray();
                    for(QJsonArray::Iterator positionIter = jsonChordPositions.begin(); positionIter != jsonChordPositions.end(); positionIter++){
                        QJsonObject jsonPositionObject = positionIter->toObject();

                        ChordInformation chordInformation;

                        if(jsonChordInformation.contains("key")){
                            chordInformation.setKey(jsonChordInformation.value("key").toString().toStdString());
                        }

                        if(jsonChordInformation.contains("suffix")){
                            chordInformation.setSuffix(jsonChordInformation.value("suffix").toString().toStdString());
                        }

                        if(jsonPositionObject.contains("frets")){
                            QJsonArray fretsArray = jsonPositionObject.value("frets").toArray();
                            for(uint32_t values = 0; values < fretsArray.size(); values++)
                                chordInformation.posInfo.frets[values] = fretsArray[values].toInt();
                        }

                        if(jsonPositionObject.contains("fingers")){
                            QJsonArray fingersArray = jsonPositionObject.value("fingers").toArray();
                            for(uint32_t values = 0; values < fingersArray.size(); values++)
                                chordInformation.posInfo.fingers[values] = fingersArray[values].toInt();
                        }

                        if(jsonPositionObject.contains("barres")){
                            QJsonArray barresArray = jsonPositionObject.value("barres").toArray();
                            for(uint32_t values = 0; values < barresArray.size(); values++)
                                chordInformation.posInfo.barres.push_back(barresArray[values].toInt());
                        }

                        if(jsonPositionObject.contains("baseFret")){
                           chordInformation.posInfo.baseFret = jsonPositionObject.value("baseFret").toInt();
                        }

                        if(jsonPositionObject.contains("capo")){
                           chordInformation.posInfo.capo = jsonPositionObject.value("capo").toBool();
                        }

                        newSuffixList.positionList.push_back(chordInformation);
                    }

                    newKey.suffixList.push_back(newSuffixList);
                }
                allChordsList.push_back(newKey);
            }
        }

    }

    for(uint32_t i = 0; i< allChordsList.size(); i++){
        if(allChordsList[i].suffixList.size() == 0){
            allChordsList.erase(allChordsList.begin() + i);
        }
    }

}


ChordInformation::ChordInformation(const ChordInformation &otherObj)
{
    keyID = otherObj.keyID;
    suffixID = otherObj.suffixID;

    memcpy(posInfo.frets, otherObj.posInfo.frets, 6);
    memcpy(posInfo.fingers, otherObj.posInfo.fingers, 6);
    posInfo.baseFret = otherObj.posInfo.baseFret;
    posInfo.barres = otherObj.posInfo.barres;
    posInfo.capo = otherObj.posInfo.capo;
}

void ChordInformation::setKey(std::string key)
{
    for(uint32_t i = 0; i < ChordKeyCount; i++){
        if(ChordKeyStrList[i] == key){
            keyID = static_cast<ChordKeyEnum>(i);
        }
    }
}

void ChordInformation::setKey(ChordKeyEnum keyID)
{
    this->keyID = keyID;
}

void ChordInformation::setSuffix(std::string suffix)
{
    for(uint32_t i = 0; i < ChordSuffixCount; i++){
        if(ChordSuffixStrList[i] == suffix){
            suffixID = static_cast<ChordSuffixEnum>(i);
        }
    }
}

void ChordInformation::setSuffix(ChordSuffixEnum suffixID)
{
    this->suffixID = suffixID;
}

ChordKeyEnum ChordInformation::getKey() const
{
    return keyID;
}

std::string ChordInformation::getKeyStr() const
{
    return ChordKeyStrList[keyID];
}

ChordSuffixEnum ChordInformation::getSuffix() const
{
    return suffixID;
}

std::string ChordInformation::getSuffixStr() const
{
    return ChordSuffixStrList[suffixID];
}

bool ChordInformation::IsEqual(const ChordInformation &other)
{
    //Chord information private member comparison
    if(keyID != other.keyID)
        return false;
    if(suffixID != other.suffixID)
        return false;

    //Position information comparison
    if(posInfo.barres != other.posInfo.barres)
        return false;
    if(!std::equal(std::begin(posInfo.frets), std::end(posInfo.frets), other.posInfo.frets))
        return false;
    if(!std::equal(std::begin(posInfo.fingers), std::end(posInfo.fingers), other.posInfo.fingers))
        return false;
    if(posInfo.baseFret != other.posInfo.baseFret)
        return false;
    if(posInfo.capo != other.posInfo.capo)
        return false;

    return true;
}














const uint32_t FindStrInVector(const std::string &str, const std::vector<std::string> &vec)
{
    for(uint32_t i = 0 ; i < vec.size(); i++){
        if(vec[i] == str){
            return i;
        }
    }

    return -1;
}
