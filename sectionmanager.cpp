#include "sectionmanager.h"


SectionManager::~SectionManager()
{
    DeleteAll();
}

void SectionManager::AddSection(ChordLayout *data)
{
    chordList.push_back(data);
    dataCount++;
    //data->setSectionID(dataCount);
    lastAddedData = data;
    lastAddedSecInfo = EditSectionEnum::Chord;

    SectionInfoWData newInfo(EditSectionEnum::Chord);
    newInfo.setSectionID(data->getSectionID());
    newInfo.data = data;

    sectionList.push_back(newInfo);
}

void SectionManager::AddSection(LyricObject *data)
{
    lyricList.push_back(data);
    dataCount++;
    //data->setSectionID(dataCount);
    lastAddedData = data;
    lastAddedSecInfo = EditSectionEnum::Lyric;


    SectionInfoWData newInfo(EditSectionEnum::Lyric);
    newInfo.setSectionID(data->getSectionID());
    newInfo.data = data;

    sectionList.push_back(newInfo);
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

                for(uint32_t i = 0; i < sectionList.size(); i++){
                    if(sectionList[i].data == data){
                        sectionList.erase(sectionList.begin() + i);
                        break;
                    }
                }
            }
        }
        break;
    }
    case EditSectionEnum::Lyric:{
        for(uint32_t i= 0; i < lyricList.size(); i++){
            if(lyricList[i] == data){
                lyricList.erase(lyricList.begin() + i);
                dataCount--;
                lastAddedData = nullptr;
                lastAddedSecInfo = EditSectionEnum::NoSelection;

                for(uint32_t i = 0; i < sectionList.size(); i++){
                    if(sectionList[i].data == data){
                        sectionList.erase(sectionList.begin() + i);
                        break;
                    }
                }
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

            uint32_t currentSecCount = 0; // iterates through given section type
            for(uint32_t i = 0; i < sectionList.size(); i++){
                if(sectionList[i].getSectionType() == sectionType){
                    if(currentSecCount == index){
                        sectionList.erase(sectionList.begin() + i);
                    }
                    currentSecCount++;
                }
            }
        }
        break;
    }
    case EditSectionEnum::Lyric:{
        if(lyricList.size() >= index){
            lyricList.erase(lyricList.begin() + index);
            dataCount--;
            lastAddedData = nullptr;
            lastAddedSecInfo = EditSectionEnum::NoSelection;


            uint32_t currentSecCount = 0; // iterates through given section type
            for(uint32_t i = 0; i < sectionList.size(); i++){
                if(sectionList[i].getSectionType() == sectionType){
                    if(currentSecCount == index){
                        sectionList.erase(sectionList.begin() + i);
                    }
                    currentSecCount++;
                }
            }
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

const std::vector<SectionInfoWData> SectionManager::getAllList()
{
    return sectionList;
}

const std::vector<SectionInfoWData> SectionManager::getAllbyIDList()
{
    std::vector<SectionInfoWData> tempList(sectionList.size());

    for(uint32_t i = 0; i < sectionList.size(); i++){
        tempList[sectionList[i].getSectionID()].data = sectionList[i].data;
        tempList[sectionList[i].getSectionID()].setSectionType(sectionList[i].getSectionType());
        tempList[sectionList[i].getSectionID()].setSectionID(sectionList[i].getSectionID());
    }


    return tempList;
}

uint32_t SectionManager::getCount(EditSectionEnum sectionType)
{
    return  dataCount;

}

void SectionManager::DeleteAll()
{
    for(ChordLayout* chordLayout : chordList){
        delete chordLayout;
    }

    for(LyricObject* lyricObject : lyricList){
        delete lyricObject;
    }



}
