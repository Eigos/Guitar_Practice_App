#include "sectionmanager.h"


void SectionManager::AddSection(ChordLayout *data)
{
    chordList.push_back(data);
    dataCount++;
    data->setSectionID(dataCount);
    lastAddedData = data;
    lastAddedSecInfo = EditSectionEnum::Chord;
}

void SectionManager::AddSection(LyricObject *data)
{
    lyricList.push_back(data);
    dataCount++;
    data->setSectionID(dataCount);
    lastAddedData = data;
    lastAddedSecInfo = EditSectionEnum::Lyric;
}

void SectionManager::RemoveSection(EditSectionEnum sectionType, void *data)
{
    switch(sectionType){

    case EditSectionEnum::Chord:{
        for(uint32_t i= 0; i < chordList.size(); i++){
            if(chordList[i] == data){
                chordList.erase(chordList.begin() + i);
                dataCount--;
                lastAddedData = nullptr;
                lastAddedSecInfo = EditSectionEnum::NoSelection;
            }
        }
        break;
    }
    case EditSectionEnum::Lyric:{
        for(uint32_t i= 0; i < chordList.size(); i++){
            if(chordList[i] == data){
                chordList.erase(chordList.begin() + i);
                dataCount--;
                lastAddedData = nullptr;
                lastAddedSecInfo = EditSectionEnum::NoSelection;
            }
        }
        break;
    }

    }
}

void SectionManager::RemoveSection(EditSectionEnum sectionType, uint32_t index)
{
    switch(sectionType){

    case EditSectionEnum::Chord:{
        if(chordList.size() >= index){
            chordList.erase(chordList.begin() + index);
            dataCount--;
            lastAddedData = nullptr;
            lastAddedSecInfo = EditSectionEnum::NoSelection;
        }
        break;
    }
    case EditSectionEnum::Lyric:{
        if(lyricList.size() >= index){
            lyricList.erase(lyricList.begin() + index);
            dataCount--;
            lastAddedData = nullptr;
            lastAddedSecInfo = EditSectionEnum::NoSelection;
        }
        break;
    }

    }
}

void* SectionManager::getData(EditSectionEnum sectionType, uint32_t index)
{
    switch(sectionType){

    case EditSectionEnum::Chord:{
        if(chordList.size() >= index){
            return chordList[index];
        }
        break;
    }
    case EditSectionEnum::Lyric:{
        if(lyricList.size() >= index){
            return lyricList[index];
        }
        break;
    }

    }
    return nullptr;
}

EditSectionEnum SectionManager::getLastAddedSecitonInfo()
{
    return lastAddedSecInfo;
}

void *SectionManager::getLastAdded()
{
    return lastAddedData;
}

const std::vector<ChordLayout *> SectionManager::getChordList()
{
    return chordList;
}

const std::vector<LyricObject *> SectionManager::getLyricList()
{
    return lyricList;
}

uint32_t SectionManager::getCount(EditSectionEnum sectionType)
{
    return  dataCount;

}
