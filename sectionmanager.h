#ifndef SECTIONMANAGER_H
#define SECTIONMANAGER_H

#include "lyricobject.h"
#include "chordlayout.h"
#include "section.h"
#include <stack>

class SectionInfoWData : public SectionInfo{
public:
    SectionInfoWData(EditSectionEnum sectionType) : SectionInfo(sectionType){data = nullptr;};
    SectionInfoWData() : SectionInfo(EditSectionEnum::NoSelection) {data = nullptr;};
    void* data;
};

class SectionManager{

public:
    SectionManager(){ dataCount = 0; lastAddedData = nullptr; lastAddedSecInfo = EditSectionEnum::NoSelection;};
    ~SectionManager();
    void AddSection(ChordLayout* data);
    void AddSection(LyricObject* data);

    void RemoveSection(EditSectionEnum sectionType, void* data);
    void RemoveSection(EditSectionEnum sectionType, uint32_t index);

    void* getData(EditSectionEnum sectionType, uint32_t index);

    EditSectionEnum getLastAddedSecitonInfo();
    void* getLastAdded();

    const std::vector<ChordLayout*> getChordList();
    const std::vector<LyricObject*> getLyricList();
    const std::vector<SectionInfoWData> getAllList(); //returns sections in correct order -top to bottom-
    const std::vector<SectionInfoWData> getAllbyIDList(); // returns elements by their id


    uint32_t getCount(EditSectionEnum sectionType);


private:

    void DeleteAll();

    std::vector<ChordLayout*> chordList;
    std::vector<LyricObject*> lyricList;
    std::vector<SectionInfoWData> sectionList; //Section list in correct order

    uint32_t dataCount; //HATA OLABİLİR DOĞRU AZALTMADI
    void* lastAddedData; //HATA OLABİLİR DOĞRU DEĞİŞİKLİK YAPILMADI
    EditSectionEnum lastAddedSecInfo; //HATA OLABİLİR DOĞRU DEĞİŞİKLİK YAPILMADI


};


#endif
